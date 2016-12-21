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
import java.util.List;
import java.util.Map;

import org.apache.lucene.queryparser.classic.ParseException;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.textanalyzer.domain.Feature;

/**
 * This service can be used for labeled datasets. It provides an interface for finding features that
 * most highly correlate with each label value. It also provide the ability to export to feature
 * vectors to be used with machine learning classification software.
 * 
 * @author cmw2
 *
 *         Dec 16, 2016
 */
public interface FeatureVectorService {

  /**
   * Look at the index to find the possible label values for a field.
   * 
   * @param indexDetails - details needed to open and read the index
   * @param set - set of documents (can be null)
   * @param labelField - the field for which to find the possible values (such as class, location,
   *        or topic)
   * @return
   * @throws IOException
   */
  List<String> getLabelValues(IndexDetails indexDetails, List<Integer> set, String labelField)
      throws IOException;

  /**
   * Calculates Cohen's Kappa for each feature for each label value. Returns a map where the key is
   * the label value and the value is a list of features sorted from highest to lowest kappa score
   * for that label value.
   * 
   * @param indexDetails - details needed to open and read the index
   * @param set - set of documents (can be null)
   * @param labelField - the field for which to find the possible values
   * @param featureTypes - types of features to calculates score for such as term and noun-phrase
   * @param minFreq - the minimum number of times the feature must appear to caculate a score
   * @return - a map where the key is the label value and the value is a list of features sorted
   *         from highest to lowest kappa score for that label value
   * @throws IOException
   * @throws ParseException
   */
  Map<String, List<Feature>> getFeatureVectors(IndexDetails indexDetails, List<Integer> set,
      String labelField, List<String> featureTypes, int minFreq) throws IOException, ParseException;

  /**
   * Uses the feature scores to write feature vectors in ARFF format which can be consumed by WEKA
   * for machine learning classification.
   * 
   * @param indexDetails - details needed to open and read the index
   * @param set - set of documents (can be null)
   * @param labelField - the field for which to find the possible values
   * @param featureVectors - the map returned from getFeatureVectors()
   * @param numFeatures - number of features to use for each class
   * @param fileName - file name to write ARFF file
   * @throws IOException
   * @throws ParseException
   */
  void writeARFF(IndexDetails indexDetails, List<Integer> set, String labelField,
      Map<String, List<Feature>> featureVectors, int numFeatures, String fileName)
      throws IOException, ParseException;

}
