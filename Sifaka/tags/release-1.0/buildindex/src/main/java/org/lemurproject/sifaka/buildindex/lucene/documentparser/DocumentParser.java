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
package org.lemurproject.sifaka.buildindex.lucene.documentparser;

import java.io.IOException;
import java.util.List;

import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocument;

/**
 * Parses input into documents and defines the fields in the document type. All DocumentParser
 * implementations much implement DocumentParser.
 * 
 * @author cmw2
 *
 *         Dec 16, 2016
 */
public interface DocumentParser {

  /**
   * Returns all the fields that can occur in this document type. Not all documents need to contain
   * every field.
   * 
   * @return list of field names
   */
  List<String> getAllFields();

  /**
   * Returns the fields that can be used as labels for feature vectors or machine learning.
   * 
   * @return list of field names that are labels
   */
  List<String> getLabelFields();

  /**
   * Returns all fields that should that have lexical analysis done (such as stemming and stop word
   * removal) and should be searched.
   * 
   * @return list of field names that should be searched
   */
  List<String> getSearchableFields();

  /**
   * 
   * @return boolean defining whether another document exists
   */
  boolean hasNextDocument();

  /**
   * Examines input to find the next document and split it into fields.
   * 
   * @return
   * @throws IOException
   */
  ParsedDocument getNextDocument() throws IOException;

}
