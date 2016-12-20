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
package org.lemurproject.sifaka.buildindex.lucene.factory;

import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.lemurproject.sifaka.buildindex.lucene.documentparser.DocumentParser;
import org.lemurproject.sifaka.buildindex.lucene.documentparser.JsonDocumentParser;
import org.lemurproject.sifaka.buildindex.lucene.documentparser.TextDocumentParser;
import org.lemurproject.sifaka.buildindex.lucene.documentparser.TrecDocumentParser;
import org.lemurproject.sifaka.buildindex.lucene.documentparser.WSJDocumentParser;

/**
 * Instantiates the correct document parser based on the user input for property: documentFormat. To
 * add an additional document parser, add the implementation class for that parser to the
 * docParserMap in the constructor.
 * 
 * @author cmw2
 *
 *         Nov 30, 2016
 */
public class DocumentParserFactory {

  private Map<String, Class<? extends DocumentParser>> docParserMap;

  public DocumentParserFactory() {
    docParserMap = new HashMap<>();
    docParserMap.put("json", JsonDocumentParser.class);
    docParserMap.put("text", TextDocumentParser.class);
    docParserMap.put("trec", TrecDocumentParser.class);
    docParserMap.put("wsj", WSJDocumentParser.class);
  }

  public Set<String> getDocumentFormatTypes() {
    return docParserMap.keySet();
  }

  public DocumentParser getDocumentParser(String documentFormatString, String dataDirectory)
      throws InstantiationException, IllegalAccessException, IllegalArgumentException,
      InvocationTargetException, NoSuchMethodException, SecurityException {
    Class<? extends DocumentParser> clazz = docParserMap.get(documentFormatString);
    DocumentParser docParser = null;
    if (clazz != null) {
      docParser = clazz.getDeclaredConstructor(String.class).newInstance(dataDirectory);
    } else {
      System.out.println("ERROR: No such document parser: " + documentFormatString);
      System.out.println("Please define one of these annotator types: " + getDocumentFormatTypes());
      throw new IllegalArgumentException();
    }
    return docParser;
  }

}
