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
import java.util.List;

/**
 * Contains a list of basic IndexProperties and a list of field specific IndexFieldProperties.
 * 
 * @author cmw2
 *
 *         Nov 17, 2016
 */
public class IndexProperties extends BaseObject implements Serializable {

  private static final long serialVersionUID = 1569080360335040446L;

  private IndexDetails indexDetails;
  private List<IndexProperty> basicIndexProperties;
  private List<IndexFieldProperty> indexFieldProperties;

  public List<IndexProperty> getBasicIndexProperties() {
    return basicIndexProperties;
  }

  public void setBasicIndexProperties(List<IndexProperty> basicIndexProperties) {
    this.basicIndexProperties = basicIndexProperties;
  }

  public List<IndexFieldProperty> getIndexFieldProperties() {
    return indexFieldProperties;
  }

  public void setIndexFieldProperties(List<IndexFieldProperty> indexFieldProperties) {
    this.indexFieldProperties = indexFieldProperties;
  }

  /**
   * @return the indexDetails
   */
  public IndexDetails getIndexDetails() {
    return indexDetails;
  }

  /**
   * @param indexDetails the indexDetails to set
   */
  public void setIndexDetails(IndexDetails indexDetails) {
    this.indexDetails = indexDetails;
  }

}
