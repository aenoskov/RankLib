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
package org.lemurproject.sifaka.textminer.domain;

import java.io.Serializable;
import java.util.List;

import org.lemurproject.sifaka.buildindex.domain.IndexProperties;

/**
 * @author cmw2
 *
 *         Dec 11, 2016
 */
public class IndexTreeItem implements Serializable {

  private static final long serialVersionUID = -9045200770086011001L;

  private IndexProperties indexProperties;

  private String name;
  private boolean isIndex;

  // Properties for Items that are subsets
  private List<Integer> set;
  private String setQueryText;


  /**
   * @return the name
   */
  public String getName() {
    return name;
  }

  /**
   * @param name the name to set
   */
  public void setName(String name) {
    this.name = name;
  }

  /**
   * @return the isIndex
   */
  public boolean isIndex() {
    return isIndex;
  }

  /**
   * @param isIndex the isIndex to set
   */
  public void setIndex(boolean isIndex) {
    this.isIndex = isIndex;
  }

  /**
   * @return the setQueryText
   */
  public String getSetQueryText() {
    return setQueryText;
  }

  /**
   * @param setQueryText the setQueryText to set
   */
  public void setSetQueryText(String setQueryText) {
    this.setQueryText = setQueryText;
  }

  /**
   * @return the set
   */
  public List<Integer> getSet() {
    return set;
  }

  /**
   * @param set the set to set
   */
  public void setSet(List<Integer> set) {
    this.set = set;
  }

  /**
   * @return the indexProperties
   */
  public IndexProperties getIndexProperties() {
    return indexProperties;
  }

  /**
   * @param indexProperties the indexProperties to set
   */
  public void setIndexProperties(IndexProperties indexProperties) {
    this.indexProperties = indexProperties;
  }

  @Override
  public String toString() {
    String displayName = name;
    if (set != null) {
      displayName = String.join("", name, " (", String.valueOf(set.size()), ")");
    } else if (isIndex) {
      displayName = String.join("", name, " (",
          String.valueOf(indexProperties.getIndexDetails().getNumDocs()), ")");
    }
    return displayName;
  }

  @Override
  public boolean equals(Object obj) {
    IndexTreeItem item = null;
    if (obj == null || !(obj instanceof IndexTreeItem) || name == null || indexProperties == null) {
      return false;
    } else {
      item = (IndexTreeItem) obj;
    }
    if (item != null && name.equals(item.getName())
        && indexProperties.equals(item.getIndexProperties()) && isIndex == item.isIndex) {
      return true;
    }
    return false;
  }


}
