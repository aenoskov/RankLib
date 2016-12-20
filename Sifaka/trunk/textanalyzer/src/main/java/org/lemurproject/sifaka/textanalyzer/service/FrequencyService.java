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

import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.textanalyzer.domain.TermFrequency;

/**
 * Service for finding the most frequent entities in an index.
 * 
 * @author cmw2
 *
 *         Dec 16, 2016
 */
public interface FrequencyService {

  /**
   * Calculates the most frequent entities of resultType in the index (or set) and returns a sorted
   * list of entities sorted from highest to lowest.
   * 
   * @param indexDetails
   * @param set - Set of documents (can be null)
   * @param resultType - The entity type to find frequencies for
   * @param numResults - The number of results returned
   * @param sortBy - docuemnt frequency or collection term frequency
   * @return a sorted list of entities and frequencies
   * @throws IOException
   */
  List<TermFrequency> getFrequencies(IndexDetails indexDetails, List<Integer> set,
      String resultType, int numResults, String sortBy) throws IOException;

}
