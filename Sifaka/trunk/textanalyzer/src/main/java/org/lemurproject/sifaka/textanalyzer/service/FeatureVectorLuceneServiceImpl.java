/*
 * ===============================================================================================
 * Copyright (c) 2016 Carnegie Mellon University and University of Massachusetts. All Rights
 * Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval is subject to the terms
 * of the software license set forth in the LICENSE file included with this software, and also
 * available at http://www.lemurproject.org/license.html
 *
 * ================================================================================================
 */
package org.lemurproject.sifaka.textanalyzer.service;

import java.io.BufferedWriter;
import java.io.IOException;
import java.math.BigDecimal;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.StringJoiner;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.Fields;
import org.apache.lucene.index.MultiFields;
import org.apache.lucene.index.Terms;
import org.apache.lucene.index.TermsEnum;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.textanalyzer.comparator.FeatureComparator;
import org.lemurproject.sifaka.textanalyzer.domain.ArffAttributeValuePair;
import org.lemurproject.sifaka.textanalyzer.domain.Feature;
import org.lemurproject.sifaka.textanalyzer.util.LuceneAnalysisUtilImpl;

import weka.core.Attribute;
import weka.core.Instances;

public class FeatureVectorLuceneServiceImpl implements FeatureVectorService {

  private LuceneAnalysisUtilImpl util;

  public FeatureVectorLuceneServiceImpl() {
    util = new LuceneAnalysisUtilImpl();
  }

  public List<String> getLabelValues(IndexDetails indexDetails, List<Integer> set,
      String labelField) throws IOException {

    Directory directory = FSDirectory.open(Paths.get(indexDetails.getSearchDataPath()));
    DirectoryReader ireader = DirectoryReader.open(directory);
    Fields fields = MultiFields.getFields(ireader);

    List<String> labelValues = new ArrayList<>();
    // Calculate the total number of documents for each label
    if (labelField == null) {
      System.out.println("This index does not contain labels");
      return null;
    } else {
      Terms labels = fields.terms(labelField);
      if (labels != null) {
        TermsEnum labelsEnum = labels.iterator();
        while (labelsEnum.next() != null) {
          String label = labelsEnum.term().utf8ToString();
          labelValues.add(label);
        }
      }
    }
    directory.close();
    ireader.close();
    return labelValues;
  }

  public Map<String, List<Feature>> getFeatureVectors(IndexDetails indexDetails, List<Integer> set,
      String labelField, List<String> featureTypes, int minFreq)
      throws IOException, ParseException {
    Map<String, List<Feature>> featureVectors = new HashMap<>();
    Map<String, Integer> labelTotals = new HashMap<>();

    // Since we are querying multiple fields we need to keep track of
    // which terms we have already evaluated
    Set<String> termsEvaluated = new HashSet<>();

    Directory directory = FSDirectory.open(Paths.get(indexDetails.getSearchDataPath()));
    DirectoryReader ireader = DirectoryReader.open(directory);

    Integer totalDocs = set != null ? set.size() : ireader.maxDoc();
    Fields fields = MultiFields.getFields(ireader);

    // Calculate the total number of documents for each label
    if (labelField == null) {
      System.out.println("This index does not contain labels");
      return null;
    } else {
      Terms labels = fields.terms(labelField);
      if (labels != null) {
        TermsEnum labelsEnum = labels.iterator();
        while (labelsEnum.next() != null) {
          String label = labelsEnum.term().utf8ToString();
          // Term labelTerm = new Term(labelField, label);
          // int labelCount = ireader.docFreq(labelTerm);

          List<Integer> labelCount2 =
              getScoreDocs(String.join("", labelField, ":", label), set, ireader);

          labelTotals.put(label, Integer.valueOf(labelCount2.size()));
          featureVectors.put(label, new ArrayList<Feature>());
        }
      }
    }

    // Iterate over the feature Types that have been selected
    for (String featureType : featureTypes) {
      List<String> featureTypeFields =
          getFieldsForFeatureType(indexDetails.getSearchableFields(), featureType);
      // Iterate over fields to find terms and calculate values
      // Only calculate values for each term once even if it is in multiple fields
      for (String field : featureTypeFields) {
        Terms terms = fields.terms(field);
        if (terms != null) {
          TermsEnum termsEnum = terms.iterator();
          while (termsEnum.next() != null) {
            String term = termsEnum.term().utf8ToString();
            if (!termsEvaluated.contains(term)) {
              termsEvaluated.add(term);
              // Query for number of documents with term
              String termQueryString = getTermQueryString(term, featureTypeFields);
              Integer termTotal = getNumQueryHits(termQueryString, set, ireader);

              if (termTotal.intValue() >= minFreq) {

                // Query number of documents with term for each label
                labelTotals.forEach((label, labelCount) -> {
                  String labelANDTermQueryString =
                      String.join("", labelField, ":", label, " AND ", termQueryString);
                  try {
                    double kappa = 0.0;
                    if (labelCount > 0) {
                      Integer labelANDtermTotal =
                          getNumQueryHits(labelANDTermQueryString, set, ireader);

                      // Calculate Cohen's Kappa
                      double termProb = termTotal.doubleValue() / totalDocs.doubleValue();
                      double NOTtermProb = 1.0 - termProb;
                      double labelProb = labelCount.doubleValue() / totalDocs.doubleValue();
                      double NOTlabelProb = 1.0 - labelProb;
                      double Pe = (termProb * labelProb) + (NOTtermProb * NOTlabelProb);
                      double NOTlabelANDterm = totalDocs.doubleValue() - (labelCount.doubleValue()
                          + termTotal.doubleValue() - labelANDtermTotal.doubleValue());
                      double Po = (labelANDtermTotal.doubleValue() + NOTlabelANDterm)
                          / totalDocs.doubleValue();
                      if (Pe != 1.0) {
                        kappa = (Po - Pe) / (1.0 - Pe);
                      } else {
                        kappa = 1.0;
                      }
                    }
                    BigDecimal value = new BigDecimal(kappa);
                    value = value.setScale(5, BigDecimal.ROUND_HALF_UP);

                    // Create feature which contains score
                    Feature feature = new Feature();
                    feature.setTerm(term.replace("_", " "));
                    feature.setType(featureType);
                    feature.setScore(value.doubleValue());

                    featureVectors.get(label).add(feature);
                  } catch (Exception e) {
                    System.out.println("Error querying: " + label + " AND " + term);
                  }

                });
              }
            }
          }
        }
      }
    }

    directory.close();
    ireader.close();

    Map<String, List<Feature>> sortedFeatureVectors = new HashMap<>();

    featureVectors.forEach((label, featureList) -> {
      Collections.sort(featureList, new FeatureComparator());
      sortedFeatureVectors.put(label, featureList);
    });

    return sortedFeatureVectors;
  }

  public void writeARFF(IndexDetails indexDetails, List<Integer> set, String labelField,
      Map<String, List<Feature>> featureVectors, int numFeatures, String fileName)
      throws IOException, ParseException {

    // Store the top numFeatures for each label in a set
    Set<String> topFeatures = new HashSet<>();
    ArrayList<Attribute> attributes = new ArrayList<>();
    List<String> labelValues = new ArrayList<>();
    featureVectors.forEach((label, featureList) -> {
      labelValues.add(label);
      Collections.sort(featureList, new FeatureComparator());
      // Set numFeature to featureList Size if numFeatures=-1 -> user selected all features
      int selectFeatureSize = numFeatures;
      if (numFeatures < 0) {
        selectFeatureSize = featureList.size();
      }
      int i = 0;
      while (i < featureList.size() && i < selectFeatureSize) {
        boolean featureAdded = topFeatures.add(featureList.get(i).getTerm());
        if (featureAdded) {
          attributes.add(new Attribute(featureList.get(i).getTerm()));
        }
        i++;
      }
    });
    Attribute labelAttribute = new Attribute("CLASS", labelValues);
    attributes.add(labelAttribute);

    Directory directory = FSDirectory.open(Paths.get(indexDetails.getSearchDataPath()));
    DirectoryReader ireader = DirectoryReader.open(directory);
    Integer totalDocs = set != null ? set.size() : ireader.maxDoc();

    Instances data = new Instances("Data", attributes, totalDocs);

    // Map<Integer, Instance> docInstances = new HashMap<>();
    Map<Integer, List<ArffAttributeValuePair>> docAttributeMap = new HashMap<>();

    // Initialize the instances for each document
    // Set each attribute to 0 to start
    if (set != null) {
      for (int docNum : set) {
        List<ArffAttributeValuePair> docAttributes = new ArrayList<>();
        docAttributeMap.put(docNum, docAttributes);
      }
    } else {
      for (int docNum = 0; docNum < totalDocs; docNum++) {
        List<ArffAttributeValuePair> docAttributes = new ArrayList<>();
        docAttributeMap.put(docNum, docAttributes);
      }
    }

    // Iterate over attributes and search for each one to find which documents contain which
    // features
    int attributeNum = 0;
    for (Attribute attribute : attributes) {
      // Do not included label value here - different query needed
      if (!attribute.name().equals("CLASS")) {
        String queryString =
            getTermQueryString(attribute.name(), indexDetails.getSearchableFields());
        List<Integer> hits = getScoreDocs(queryString, set, ireader);
        for (Integer hit : hits) {
          // Instance docInstance = docInstances.get(hit.doc);
          List<ArffAttributeValuePair> docAttributes = docAttributeMap.get(hit);
          ArffAttributeValuePair attrValuePair = new ArffAttributeValuePair();
          attrValuePair.setAttribute(attributeNum);
          docAttributes.add(attrValuePair);
        }
        attributeNum++;
      }

    }

    // Initialize the instances for each document
    // Set each attribute to 0 to start
    if (set != null) {
      for (int docNum : set) {
        String labelValue =
            ireader.document(docNum).getField(labelField).stringValue().toLowerCase();
        ArffAttributeValuePair attrValuePair = new ArffAttributeValuePair();
        attrValuePair.setAttribute(attributeNum);
        attrValuePair.setValue(labelValue);
        docAttributeMap.get(docNum).add(attrValuePair);
      }
    } else {
      for (int docNum = 0; docNum < totalDocs; docNum++) {
        String labelValue =
            ireader.document(docNum).getField(labelField).stringValue().toLowerCase();
        ArffAttributeValuePair attrValuePair = new ArffAttributeValuePair();
        attrValuePair.setAttribute(attributeNum);
        attrValuePair.setValue(labelValue);
        docAttributeMap.get(docNum).add(attrValuePair);
      }
    }

    Path file = Paths.get(fileName);
    try {
      BufferedWriter writer = java.nio.file.Files.newBufferedWriter(file);
      writer.write(data.toString());
      docAttributeMap.forEach((docNum, attributeList) -> {
        try {
          String formattedList = attributeList.toString().replace("[", "{");
          formattedList = formattedList.replace("]", "}");
          formattedList = formattedList.replace(", ", ",");
          writer.write(formattedList);
          writer.write("\n");
        } catch (Exception e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
        }
      });
      writer.close();
    } catch (IOException e) {

    }
  }



  private List<Integer> getScoreDocs(String queryString, List<Integer> set, DirectoryReader ireader)
      throws IOException, ParseException {
    IndexSearcher isearcher = new IndexSearcher(ireader);
    QueryParser queryParser = new QueryParser("internalId", new StandardAnalyzer());
    Query query = queryParser.parse(queryString);
    ScoreDoc[] hits = isearcher.search(query, ireader.maxDoc()).scoreDocs;
    List<Integer> filteredDocs = util.filterScoreDocsBySet(hits, set);
    return filteredDocs;
  }

  private Integer getNumQueryHits(String queryString, List<Integer> set, DirectoryReader ireader)
      throws IOException, ParseException {
    List<Integer> filteredDocs = getScoreDocs(queryString, set, ireader);
    return filteredDocs.size();
  }

  /**
   * Creates a string where the query is searched in all searchable fields.
   * 
   * @param term
   * @param fields
   * @return
   */
  private String getTermQueryString(String term, List<String> fields) {
    String formattedTerm = term.replace(":", "_");
    StringJoiner queryJoiner = new StringJoiner(" OR ");
    fields.forEach((field) -> {
      queryJoiner.add(String.join(":", field, formattedTerm));
    });
    String queryString = String.join("", "(", queryJoiner.toString(), ")");
    return queryString;
  }

  private List<String> getFieldsForFeatureType(List<String> searchableFields, String featureType) {
    List<String> featureTypeFields = new ArrayList<>();
    searchableFields.forEach(searchableField -> {
      if (featureType.equals("term")) {
        featureTypeFields.add(searchableField);
      } else {
        featureTypeFields.add(String.join("_", searchableField, featureType));
      }
    });

    return featureTypeFields;
  }

}
