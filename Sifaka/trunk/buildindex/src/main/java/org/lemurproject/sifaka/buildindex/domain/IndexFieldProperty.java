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
 * Field specific properties in the index. Stores the field name, the type of annotation ("-" for no
 * annotation), and the number of tokens in that field.
 * 
 * @author cmw2
 *
 *         Nov 17, 2016
 */
public class IndexFieldProperty extends BaseObject implements Serializable {

  private static final long serialVersionUID = -5481917075591559377L;

  private String field;
  private String annotation;
  private Integer count;

  public String getField() {
    return field;
  }

  public void setField(String field) {
    this.field = field;
  }

  public String getAnnotation() {
    return annotation;
  }

  public void setAnnotation(String annotation) {
    this.annotation = annotation;
  }

  public Integer getCount() {
    return count;
  }

  public void setCount(Integer count) {
    this.count = count;
  }

}
