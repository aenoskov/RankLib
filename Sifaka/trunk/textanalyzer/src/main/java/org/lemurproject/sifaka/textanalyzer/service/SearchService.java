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
import org.lemurproject.sifaka.textanalyzer.domain.DisplayDocument;

/**
 * 
 * @author cmw2
 *
 *         Dec 16, 2016
 */
public interface SearchService {

  DisplayDocument getDocumentById(IndexDetails indexDetails, int docID) throws IOException;

  List<Integer> search(IndexDetails indexDetails, List<Integer> set, String queryString)
      throws IOException, ParseException;

}
