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

/**
 * 
 * 
 * @author cmw2
 *
 *         Dec 17, 2016
 */
public class CoOccurrence extends BaseObject implements CSVFormatCompatable {

  private String term;
  private long termFrequency;
  private double pmi;
  private double phisquare;

  public String getTerm() {
    return term;
  }

  public void setTerm(String term) {
    this.term = term;
  }

  public long getTermFrequency() {
    return termFrequency;
  }

  public void setTermFrequency(long termFrequency) {
    this.termFrequency = termFrequency;
  }

  public double getPmi() {
    return pmi;
  }

  public void setPmi(double pmi) {
    this.pmi = pmi;
  }

  public double getPhisquare() {
    return phisquare;
  }

  public void setPhisquare(double phisquare) {
    this.phisquare = phisquare;
  }

  @Override
  public String toCSVLine() {
    return String.join(",", term, String.valueOf(termFrequency), String.valueOf(pmi).toString(),
        String.valueOf(phisquare).toString());
  }

  @Override
  public String getColumnHeaders() {
    return String.join(",", "Term", "Term Frequency", "PMI", "Phi-Square").toString();
  }

}
