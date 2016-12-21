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
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Properties;

import org.apache.lucene.benchmark.byTask.feeds.DocData;
import org.apache.lucene.benchmark.byTask.feeds.NoMoreDataException;
import org.apache.lucene.benchmark.byTask.feeds.TrecContentSourceExtended;
import org.apache.lucene.benchmark.byTask.utils.Config;
import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocument;
import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;

/**
 * An implementation of the DocumentParser that parses this trec format.
 * 
 * <pre>
 * 
 * &lt;DOC&gt;
 * &lt;HEADLINE&gt;SAMPLE HEADLINE&lt;/HEADLINE&gt;
 * &lt;TEXT&gt;Sample text body.&lt;/TEXT&gt;
 * &lt;CLASS&gt;sample&lt;/CLASS&gt;
 * &lt;/DOC&gt;
 * 
 * </pre>
 * 
 * Note: The class field can be used as a label for classification experiments.
 * 
 * @author cmw2
 *
 *         Dec 11, 2016
 */
public class TrecDocumentParser implements DocumentParser {

  private final static String EXTERNALID_FIELD = "externalId";
  private final static String INTERNALID_FIELD = "internalId";
  private final static String HEADLINE_FIELD = "headline";
  private final static String CLASS_FIELD = "class";
  private final static String BODY_FIELD = "body";

  private boolean hasNextDocument = true;
  private TrecContentSourceExtended dcsr;
  private int docNum;

  /**
   * @throws IOException
   * 
   */
  public TrecDocumentParser(String dataDirectory) throws IOException {
    docNum = 0;
    dcsr = new TrecContentSourceExtended();
    Properties pr = new Properties();
    pr.setProperty("work.dir", (new File(dataDirectory)).getAbsolutePath());
    pr.setProperty("docs.dir", (new File(dataDirectory)).getAbsolutePath());
    pr.setProperty("trec.doc.parser", "org.apache.lucene.benchmark.byTask.feeds.TrecToyDocParser");
    pr.setProperty("content.source.forever", "false");
    pr.setProperty("content.source.log.step", "100");
    pr.setProperty("content.source.verbose", "true");
    pr.setProperty("content.source.excludeIteration", "true");
    Config cr = new Config(pr);
    dcsr.setConfig(cr);
    dcsr.resetInputs();
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifaka.buildindex.lucene.documentparser.DocumentParser#getAllFields()
   */
  @Override
  public List<String> getAllFields() {
    return new ArrayList<String>(
        Arrays.asList(EXTERNALID_FIELD, INTERNALID_FIELD, CLASS_FIELD, HEADLINE_FIELD, BODY_FIELD));
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifaka.buildindex.lucene.documentparser.DocumentParser#getLabelField()
   */
  @Override
  public List<String> getLabelFields() {
    return new ArrayList<String>(Arrays.asList(CLASS_FIELD));
  }

  /*
   * (non-Javadoc)
   * 
   * @see
   * org.lemurproject.sifaka.buildindex.lucene.documentparser.DocumentParser#getSearchableFields()
   */
  @Override
  public List<String> getSearchableFields() {
    return new ArrayList<String>(Arrays.asList(HEADLINE_FIELD, BODY_FIELD));
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifaka.buildindex.lucene.documentparser.DocumentParser#hasNextDocument()
   */
  @Override
  public boolean hasNextDocument() {
    return hasNextDocument;
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifaka.buildindex.lucene.documentparser.DocumentParser#getNextDocument()
   */
  @Override
  public ParsedDocument getNextDocument() throws IOException {
    ParsedDocument doc = null;

    DocData d = new DocData();
    try {
      d = dcsr.getNextDocData(d);
      doc = new ParsedDocument();
      doc.setDocumentFields(new ArrayList<>());

      String externalId = String.valueOf(docNum);
      if (d.getProps().getProperty("headline") != null
          && d.getProps().getProperty("headline").length() > 0) {
        externalId = d.getProps().getProperty("headline");
      }

      ParsedDocumentField externalIdField =
          new ParsedDocumentField(EXTERNALID_FIELD, externalId, false);
      doc.getDocumentFields().add(externalIdField);

      ParsedDocumentField internalIdField =
          new ParsedDocumentField(INTERNALID_FIELD, String.valueOf(docNum), false);
      doc.getDocumentFields().add(internalIdField);

      ParsedDocumentField headlineField =
          new ParsedDocumentField(HEADLINE_FIELD, d.getProps().getProperty("headline"), true);
      doc.getDocumentFields().add(headlineField);

      ParsedDocumentField classField =
          new ParsedDocumentField(CLASS_FIELD, d.getProps().getProperty("class"), false);
      doc.getDocumentFields().add(classField);

      ParsedDocumentField bodyField = new ParsedDocumentField(BODY_FIELD, d.getBody(), true);
      doc.getDocumentFields().add(bodyField);

      docNum++;
    } catch (NoMoreDataException e) {
      hasNextDocument = false;
      dcsr.close();
    }

    return doc;
  }

}
