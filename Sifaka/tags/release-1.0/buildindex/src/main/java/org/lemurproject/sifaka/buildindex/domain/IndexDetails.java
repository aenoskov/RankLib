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
import java.time.LocalDateTime;
import java.util.List;
import java.util.StringJoiner;

/**
 * @author cmw2
 *
 *         Dec 9, 2016
 */
public class IndexDetails extends BaseObject implements Serializable {

  private static final long serialVersionUID = -6998514731717069877L;

  private String name;
  private String basePath;
  private String searchDataPath;
  private int numDocs;

  private List<String> annotationTypes;
  private List<String> allFields;
  private List<String> searchableFields;
  private List<String> labelFields;

  private LocalDateTime buildDate;
  private String buildVersion;

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
   * @return the basePath
   */
  public String getBasePath() {
    return basePath;
  }

  /**
   * @param basePath the basePath to set
   */
  public void setBasePath(String basePath) {
    this.basePath = basePath;
  }

  /**
   * @return the annotationTypes
   */
  public List<String> getAnnotationTypes() {
    return annotationTypes;
  }

  /**
   * @param annotationTypes the annotationTypes to set
   */
  public void setAnnotationTypes(List<String> annotationTypes) {
    this.annotationTypes = annotationTypes;
  }

  /**
   * @return the allFields
   */
  public List<String> getAllFields() {
    return allFields;
  }

  /**
   * @param allFields the allFields to set
   */
  public void setAllFields(List<String> allFields) {
    this.allFields = allFields;
  }

  /**
   * @return the searchableFields
   */
  public List<String> getSearchableFields() {
    return searchableFields;
  }

  /**
   * @param searchableFields the searchableFields to set
   */
  public void setSearchableFields(List<String> searchableFields) {
    this.searchableFields = searchableFields;
  }

  /**
   * @return the labelFields
   */
  public List<String> getLabelFields() {
    return labelFields;
  }

  /**
   * @param labelFields the labelFields to set
   */
  public void setLabelFields(List<String> labelFields) {
    this.labelFields = labelFields;
  }

  /**
   * @return the buildDate
   */
  public LocalDateTime getBuildDate() {
    return buildDate;
  }

  /**
   * @param buildDate the buildDate to set
   */
  public void setBuildDate(LocalDateTime buildDate) {
    this.buildDate = buildDate;
  }

  /**
   * @return the buildVersion
   */
  public String getBuildVersion() {
    return buildVersion;
  }

  /**
   * @param buildVersion the buildVersion to set
   */
  public void setBuildVersion(String buildVersion) {
    this.buildVersion = buildVersion;
  }

  public String toProperties() {
    StringJoiner buffer = new StringJoiner("\n");
    buffer.add(String.join("=", "Name", this.name));
    buffer.add(String.join("=", "Path", this.basePath));
    buffer.add(String.join("=", "Indexer Version", this.buildVersion));
    buffer.add(String.join("=", "Build Date", this.buildDate.toString()));
    buffer.add(String.join("=", "Annotations", this.annotationTypes.toString()));
    buffer.add(String.join("=", "All Fields", this.allFields.toString()));
    buffer.add(String.join("=", "Searchable Fields", this.searchableFields.toString()));
    return buffer.toString();
  }

  /**
   * @return the numDocs
   */
  public int getNumDocs() {
    return numDocs;
  }

  /**
   * @param numDocs the numDocs to set
   */
  public void setNumDocs(int numDocs) {
    this.numDocs = numDocs;
  }

  /**
   * @return the searchDataPath
   */
  public String getSearchDataPath() {
    return searchDataPath;
  }

  /**
   * @param searchDataPath the searchDataPath to set
   */
  public void setSearchDataPath(String searchDataPath) {
    this.searchDataPath = searchDataPath;
  }

}
