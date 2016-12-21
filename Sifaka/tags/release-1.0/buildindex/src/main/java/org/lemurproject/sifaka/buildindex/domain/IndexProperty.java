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

package org.lemurproject.sifaka.buildindex.domain;

import java.io.Serializable;

/**
 * Basic properties about the index stored in Name-Value pairs. Example properies are: stemmer,
 * number of documents, and annotations.
 * 
 * @author cmw2
 *
 *         Nov 17, 2016
 */
public class IndexProperty extends BaseObject implements Serializable {

  private static final long serialVersionUID = 3246365200608905729L;

  /**
   * @param name
   * @param value
   */
  public IndexProperty(String name, String value) {
    super();
    this.name = name;
    this.value = value;
  }

  private String name;
  private String value;

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public String getValue() {
    return value;
  }

  public void setValue(String value) {
    this.value = value;
  }

}
