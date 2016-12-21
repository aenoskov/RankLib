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

import java.util.Map;

import org.lemurproject.sifaka.buildindex.domain.BaseObject;

public class DisplayDocument extends BaseObject {

  private String docName;
  private String docId;
  private Map<String, String> fieldsAndContent;

  /**
   * @return the fieldsAndContent
   */
  public Map<String, String> getFieldsAndContent() {
    return fieldsAndContent;
  }

  /**
   * @param fieldsAndContent the fieldsAndContent to set
   */
  public void setFieldsAndContent(Map<String, String> fieldsAndContent) {
    this.fieldsAndContent = fieldsAndContent;
  }

  /**
   * @return the docName
   */
  public String getDocName() {
    return docName;
  }

  /**
   * @param docName the docName to set
   */
  public void setDocName(String docName) {
    this.docName = docName;
  }

  /**
   * @return the docId
   */
  public String getDocId() {
    return docId;
  }

  /**
   * @param docId the docId to set
   */
  public void setDocId(String docId) {
    this.docId = docId;
  }

}
