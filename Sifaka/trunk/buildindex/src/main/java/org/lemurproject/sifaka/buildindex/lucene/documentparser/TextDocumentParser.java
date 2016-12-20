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
package org.lemurproject.sifaka.buildindex.lucene.documentparser;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocument;
import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;

/**
 * An implementation of the DocumentParser which parser plain text files.
 * 
 * @author cmw2
 *
 *         Dec 1, 2016
 */
public class TextDocumentParser implements DocumentParser {

  private final static String NAME_FIELD = "externalId";
  private final static String ID_FIELD = "internalId";
  private final static String BODY_FIELD = "body";

  private int docNum;
  private File[] files;

  public TextDocumentParser(String dataDirectory) {
    files = new File(dataDirectory).listFiles();
    docNum = 0;
  }

  @Override
  public List<String> getAllFields() {
    return new ArrayList<String>(Arrays.asList(NAME_FIELD, ID_FIELD, BODY_FIELD));
  }

  @Override
  public List<String> getLabelFields() {
    return null;
  }

  @Override
  public List<String> getSearchableFields() {
    return new ArrayList<String>(Arrays.asList(BODY_FIELD));
  }

  @Override
  public boolean hasNextDocument() {
    if (docNum < files.length) {
      return true;
    }
    return false;
  }

  @Override
  public ParsedDocument getNextDocument() throws IOException {
    String content = new String(Files.readAllBytes(Paths.get(files[docNum].getPath())));

    ParsedDocument doc = new ParsedDocument();
    doc.setDocumentFields(new ArrayList<>());

    ParsedDocumentField internalIdField =
        new ParsedDocumentField(ID_FIELD, String.valueOf(docNum), false);
    doc.getDocumentFields().add(internalIdField);

    ParsedDocumentField externalIdField =
        new ParsedDocumentField(NAME_FIELD, files[docNum].getName(), false);
    doc.getDocumentFields().add(externalIdField);

    ParsedDocumentField bodyField = new ParsedDocumentField(BODY_FIELD, content, true);
    doc.getDocumentFields().add(bodyField);

    docNum++;
    return doc;
  }

}
