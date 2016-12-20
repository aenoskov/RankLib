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
package org.lemurproject.sifaka.buildindex.lucene.annotator;

import java.io.IOException;
import java.util.List;

import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;

/**
 * Finds entities in document fields and creates new annotation fields. All Annotation
 * implementations must implement Annotator.
 * 
 * @author cmw2
 *
 *         Dec 1, 2016
 */
public interface Annotator {

  /**
   * @return a list of entities that this annotator returns
   */
  List<String> getAnnotationTypes();

  /**
   * Finds entites for a field and returns a list of new fields with those entities.
   * 
   * @param field
   * @return
   * @throws IOException
   */
  List<ParsedDocumentField> getAnnotationsForField(ParsedDocumentField field) throws IOException;

}
