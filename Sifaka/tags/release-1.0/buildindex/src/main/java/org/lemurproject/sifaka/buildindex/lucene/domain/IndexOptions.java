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
package org.lemurproject.sifaka.buildindex.lucene.domain;

import java.io.Serializable;

import org.lemurproject.sifaka.buildindex.domain.BaseObject;

/**
 * The domain object for user input.
 * 
 * @author cmw2
 *
 *         Nov 23, 2016
 */
public class IndexOptions extends BaseObject implements Serializable {

  private static final long serialVersionUID = -8517162310984747189L;

  private String dataDirectory;
  private String indexDirectory;

  // Defines the type of parser
  private String documentFormat;

  // Defines classes that can annotate documents - comma separated list
  private String annotations;

  // Defines the types of phrases to annotate - csv list
  private String phraseTypes;

  // Analyzer Options
  private String stemmer;
  private boolean removeStopwords;
  private boolean ignoreCase;

  /**
   * 
   * @return String documentFormat (either text or trec) which will define the parser that is used
   */
  public String getDocumentFormat() {
    return documentFormat;
  }

  public void setDocumentFormat(String documentFormat) {
    this.documentFormat = documentFormat;
  }

  public String getDataDirectory() {
    return dataDirectory;
  }

  public void setDataDirectory(String dataDirectory) {
    this.dataDirectory = dataDirectory;
  }

  public String getIndexDirectory() {
    return indexDirectory;
  }

  public void setIndexDirectory(String indexDirectory) {
    this.indexDirectory = indexDirectory;
  }

  public String getAnnotations() {
    return annotations;
  }

  public void setAnnotations(String annotations) {
    this.annotations = annotations;
  }

  public String getStemmer() {
    return stemmer;
  }

  public void setStemmer(String stemmer) {
    this.stemmer = stemmer;
  }

  public boolean isRemoveStopwords() {
    return removeStopwords;
  }

  public void setRemoveStopwords(boolean removeStopwords) {
    this.removeStopwords = removeStopwords;
  }

  public boolean isIgnoreCase() {
    return ignoreCase;
  }

  public void setIgnoreCase(boolean ignoreCase) {
    this.ignoreCase = ignoreCase;
  }

  public String getPhraseTypes() {
    return phraseTypes;
  }

  public void setPhraseTypes(String phraseTypes) {
    this.phraseTypes = phraseTypes;
  }

}
