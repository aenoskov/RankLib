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

import java.util.List;

import org.lemurproject.sifaka.buildindex.domain.BaseObject;

/**
 * The document which has been broken into fields by the document parser and is ready to be
 * annotated and written to the Lucene index.
 * 
 * @author cmw2
 *
 */
public class ParsedDocument extends BaseObject {

  private List<ParsedDocumentField> documentFields;

  /**
   * @return the documentFields
   */
  public List<ParsedDocumentField> getDocumentFields() {
    return documentFields;
  }

  /**
   * @param documentFields the documentFields to set
   */
  public void setDocumentFields(List<ParsedDocumentField> documentFields) {
    this.documentFields = documentFields;
  }

}
