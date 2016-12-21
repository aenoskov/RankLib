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

import java.io.IOException;
import java.math.BigDecimal;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.Terms;
import org.apache.lucene.index.TermsEnum;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.textanalyzer.comparator.CoPMIComparator;
import org.lemurproject.sifaka.textanalyzer.comparator.CoPhiSquareComparator;
import org.lemurproject.sifaka.textanalyzer.comparator.CoTermFrequencyComparator;
import org.lemurproject.sifaka.textanalyzer.domain.CoOccurrence;
import org.lemurproject.sifaka.textanalyzer.factory.AnalyzerFactory;
import org.lemurproject.sifaka.textanalyzer.util.LuceneAnalysisUtilImpl;

public class CoOccurrenceLuceneServiceImpl implements CoOccurrenceService {

  private Analyzer analyzer;
  private LuceneAnalysisUtilImpl util;

  public CoOccurrenceLuceneServiceImpl() {
    AnalyzerFactory analyzerFactory = new AnalyzerFactory();
    analyzer = analyzerFactory.getAnalyzer();
    util = new LuceneAnalysisUtilImpl();
  }

  public List<CoOccurrence> coOccurrence(IndexDetails indexDetails, List<Integer> set,
      String queryInput, String resultType, int searchDepthInput, int minFrequencyInput,
      int numResultsInput, String sortByInput) throws IOException, ParseException {

    Directory directory = FSDirectory.open(Paths.get(indexDetails.getSearchDataPath()));
    DirectoryReader ireader = DirectoryReader.open(directory);
    IndexSearcher isearcher = new IndexSearcher(ireader);

    QueryParser parser = new QueryParser("body", analyzer);
    String queryString = util.getQueryString(queryInput, indexDetails.getSearchableFields());
    org.apache.lucene.search.Query query = parser.parse(queryString);

    int totalDocs = ireader.maxDoc();
    int searchDepth = Math.min(searchDepthInput, totalDocs);
    ScoreDoc[] hits = isearcher.search(query, searchDepth).scoreDocs;
    ScoreDoc[] totalHits = isearcher.search(query, totalDocs).scoreDocs;
    double queryHits = (double) totalHits.length;
    Map<String, CoOccurrence> combinedEntityHits = new HashMap<>();

    // Find result field names
    List<String> resultFields = new ArrayList<>();
    for (String field : indexDetails.getSearchableFields()) {
      String resultField = String.join("_", field, resultType);
      resultFields.add(resultField);
    }

    // Find the document frequency of each entity
    List<Integer> hitDocIDs = util.filterScoreDocsBySet(hits, set);
    for (Integer hitDocID : hitDocIDs) {
      Set<String> docTerms = new HashSet<>();
      for (String resultField : resultFields) {
        Terms terms = ireader.getTermVector(hitDocID.intValue(), resultField);
        if (terms != null) {
          TermsEnum termsEnum = terms.iterator();
          while (termsEnum.next() != null) {
            String term = termsEnum.term().utf8ToString();
            boolean termAdded = docTerms.add(term);
            if (termAdded) {
              combinedEntityHits.putIfAbsent(term, new CoOccurrence());
              CoOccurrence co = combinedEntityHits.get(term);
              String termString = term.replace("_", " ");
              co.setTerm(termString);
              co.setTermFrequency(co.getTermFrequency() + 1);
              combinedEntityHits.put(term, co);
            }
          }
        }
      }
    }

    // Remove all terms below the minimum frequency threshold
    Iterator<Map.Entry<String, CoOccurrence>> iter = combinedEntityHits.entrySet().iterator();
    while (iter.hasNext()) {
      Map.Entry<String, CoOccurrence> entry = iter.next();
      if (entry.getValue().getTermFrequency() < minFrequencyInput) {
        iter.remove();
      }
    }

    // Calculate Co-occurence metrics
    combinedEntityHits.forEach((entity, count) -> {
      QueryParser entityParser = new QueryParser("body", analyzer);
      try {
        if (!entity.contains(":")) {
          String entityQueryString = util.getQueryString(entity, resultFields);
          org.apache.lucene.search.Query entityQuery = entityParser.parse(entityQueryString);
          ScoreDoc[] personHits = isearcher.search(entityQuery, totalDocs).scoreDocs;
          List<Integer> filteredHits = util.filterScoreDocsBySet(personHits, set);

          double numPersonHits = (double) filteredHits.size();
          double AandB = (double) count.getTermFrequency();
          double AandNotB = queryHits - AandB;
          double BandNotA = numPersonHits - AandB;
          double notAandB = (double) totalDocs - AandB - AandNotB - BandNotA;

          // calculate PMI
          if (((AandB + BandNotA) * (AandB + AandNotB)) > 0) {
            double pmi = Math.log((AandB * (AandB + BandNotA + AandNotB + notAandB))
                / ((AandB + BandNotA) * (AandB + AandNotB)));
            pmi = truncateAndRound(pmi);
            count.setPmi(pmi);
            // entitiesAndPMI.put(entity, pmi);
          }

          // caluculate Phi-Square
          double denom = (AandB + AandNotB) * (BandNotA + notAandB) * (AandNotB + notAandB)
              * (AandB + BandNotA);
          if (denom > 0) {
            double nom = Math.pow(AandB * notAandB - AandNotB * BandNotA, 2.0);
            double phisquare = nom / denom;
            phisquare = truncateAndRound(phisquare);
            count.setPhisquare(phisquare);
          }
        }
      } catch (Exception e) {
        e.printStackTrace();
      }
    });

    directory.close();
    ireader.close();

    int maxResults = Math.min(combinedEntityHits.size(), numResultsInput);
    return sortMapByCoOccurance(combinedEntityHits, sortByInput).subList(0, maxResults);
  }

  private double truncateAndRound(double d) {
    BigDecimal value = new BigDecimal(d);
    value = value.setScale(5, BigDecimal.ROUND_HALF_UP);
    return value.doubleValue();
  }

  private List<CoOccurrence> sortMapByCoOccurance(Map<String, CoOccurrence> map, String sortBy) {
    Comparator<String> comparator = null;
    if (sortBy == null || sortBy.equalsIgnoreCase("term frequency")) {
      comparator = new CoTermFrequencyComparator(map);
    } else if (sortBy.equalsIgnoreCase("pmi")) {
      comparator = new CoPMIComparator(map);
    } else if (sortBy.equalsIgnoreCase("phi-square")) {
      comparator = new CoPhiSquareComparator(map);
    }

    TreeMap<String, CoOccurrence> result = new TreeMap<>();
    if (comparator != null) {
      result = new TreeMap<>(comparator);
    }

    result.putAll(map);
    List<CoOccurrence> termList = new ArrayList<>(result.values());
    return termList;
  }

}
