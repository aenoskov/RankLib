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

import org.lemurproject.sifaka.buildindex.domain.BaseObject;

/**
 * A single field of a document which has been parsed by the document parser. Fields such as body
 * can be annotatable whereas meta-data fields such as externalId and internalId should not.
 * 
 * @author cmw2
 *
 *         Nov 17, 2016
 */
public class ParsedDocumentField extends BaseObject {

  private String fieldName;
  private String content;
  private boolean annotatable;

  public ParsedDocumentField() {

  }

  /**
   * @param fieldName
   * @param content
   * @param annotatable
   */
  public ParsedDocumentField(String fieldName, String content, boolean annotatable) {
    super();
    this.fieldName = fieldName;
    this.content = content;
    this.annotatable = annotatable;
  }

  public String getContent() {
    return content;
  }

  public void setContent(String content) {
    this.content = content;
  }

  /**
   * @return the fieldName
   */
  public String getFieldName() {
    return fieldName;
  }

  /**
   * @param fieldName the fieldName to set
   */
  public void setFieldName(String fieldName) {
    this.fieldName = fieldName;
  }

  /**
   * @return the annotatable
   */
  public boolean isAnnotatable() {
    return annotatable;
  }

  /**
   * @param annotatable the annotatable to set
   */
  public void setAnnotatable(boolean annotatable) {
    this.annotatable = annotatable;
  }

}
