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
package org.lemurproject.sifaka.textanalyzer.domain;

import org.lemurproject.sifaka.buildindex.domain.BaseObject;

public class TermFrequency extends BaseObject implements CSVFormatCompatable {

  private String term;
  private long documentFrequency;
  private long collectionTermFrequency;

  public long getDocumentFrequency() {
    return documentFrequency;
  }

  public void setDocumentFrequency(long documentFrequency) {
    this.documentFrequency = documentFrequency;
  }

  public long getCollectionTermFrequency() {
    return collectionTermFrequency;
  }

  public void setCollectionTermFrequency(long collectionTermFrequency) {
    this.collectionTermFrequency = collectionTermFrequency;
  }

  public String getTerm() {
    return term;
  }

  public void setTerm(String term) {
    this.term = term;
  }

  @Override
  public String toCSVLine() {
    return String
        .join(",", term, String.valueOf(documentFrequency), String.valueOf(collectionTermFrequency))
        .toString();
  }

  @Override
  public String getColumnHeaders() {
    return String.join(",", "Term", "Document Frequency", "Collection Term Frequency").toString();
  }

}
