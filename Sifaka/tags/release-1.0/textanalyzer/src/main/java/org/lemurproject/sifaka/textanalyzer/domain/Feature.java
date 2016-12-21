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

public class Feature extends BaseObject {

  private String term;
  private String type;
  private Double score;

  public String getTerm() {
    return term;
  }

  public void setTerm(String term) {
    this.term = term;
  }

  public Double getScore() {
    return score;
  }

  public void setScore(Double score) {
    this.score = score;
  }

  @Override
  public String toString() {
    return String.join(" ", term, type, score.toString());
  }

  public String getType() {
    return type;
  }

  public void setType(String type) {
    this.type = type;
  }

}
