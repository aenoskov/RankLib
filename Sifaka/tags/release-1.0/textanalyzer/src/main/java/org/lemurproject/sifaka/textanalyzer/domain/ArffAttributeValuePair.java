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
 * Domain object used to write ARFF files.
 * 
 * @author cmw2
 *
 *         Dec 17, 2016
 */
public class ArffAttributeValuePair extends BaseObject {

  private int attribute;
  private String value = "1";

  public int getAttribute() {
    return attribute;
  }

  public void setAttribute(int attribute) {
    this.attribute = attribute;
  }

  public String getValue() {
    return value;
  }

  public void setValue(String value) {
    this.value = value;
  }

  public String toString() {
    return String.join(" ", String.valueOf(attribute), value);
  }

}

