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

import org.apache.lucene.queryparser.classic.ParseException;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.textanalyzer.domain.CoOccurrence;

/**
 * Service for calculating entities that co-occur.
 * 
 * @author cmw2
 *
 *         Dec 16, 2016
 */
public interface CoOccurrenceService {

  /**
   * Calculates which entities co-occur with a query using term frequency, PMI, and phi-squared
   * co-occurrence metrics.
   * 
   * @param indexDetails - details needed to open and query index.
   * @param set - Set of documents (can be null)
   * @param queryInput - The query string for finding co-occurring entities
   * @param resultType - The entity type to return
   * @param searchDepthInput - How many documents to return from the original query
   * @param minFrequencyInput - How many times the entity must appear to be included
   * @param numResultsInput - How many entities to be returned
   * @param sortByInput - the co-occurrence metric to sort by: term frequency, pmi, phi-square
   * @return List of CoOccurrences sorted from highest to lowest
   * @throws IOException
   * @throws ParseException
   */
  List<CoOccurrence> coOccurrence(IndexDetails indexDetails, List<Integer> set, String queryInput,
      String resultType, int searchDepthInput, int minFrequencyInput, int numResultsInput,
      String sortByInput) throws IOException, ParseException;

}
