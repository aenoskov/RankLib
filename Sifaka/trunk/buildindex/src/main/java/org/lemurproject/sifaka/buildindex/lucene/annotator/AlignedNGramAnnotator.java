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
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringJoiner;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;
import org.lemurproject.sifaka.luceneanalyzer.EnglishAnalyzerConfigurable;

/**
 * Creates NGram annotations such as bigram and trigram.
 * 
 * @author cmw2
 *
 *         Dec 10, 2016
 */
public class AlignedNGramAnnotator implements Annotator {

  private final static String ANNOTATION_CONNECTOR = "_";

  private int n;
  private Analyzer analyzer;
  private String entityName;

  public AlignedNGramAnnotator(Analyzer analyzer, int n, String entityName) {
    this.analyzer = analyzer;
    ((EnglishAnalyzerConfigurable) this.analyzer).setStopwordRemoval(false);
    this.n = n;
    this.entityName = entityName;
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifaka.buildindex.lucene.annotator.Annotator#getAnnotationTypes()
   */
  @Override
  public List<String> getAnnotationTypes() {
    return new ArrayList<String>(Arrays.asList(entityName));
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifaka.buildindex.lucene.annotator.Annotator#getAnnotationsForField(org.
   * lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField)
   */
  @Override
  public List<ParsedDocumentField> getAnnotationsForField(ParsedDocumentField field)
      throws IOException {
    StringJoiner ngramBuffer = new StringJoiner(" ");

    TokenStream stream = null;
    try {
      stream = analyzer.tokenStream(null, new StringReader(field.getContent()));
      stream.reset();
      List<String> prevTokens = new ArrayList<>();
      while (stream.incrementToken()) {
        String token = stream.getAttribute(CharTermAttribute.class).toString();
        prevTokens.add(token);
        if (prevTokens.size() == n) {
          String ngram = String.join("_", prevTokens);
          ngram = ngram.replace(".", "");
          ngramBuffer.add(ngram);
          prevTokens.remove(0);
        }
      }

    } catch (IOException e) {
      // not thrown b/c we're using a string reader...
      throw new RuntimeException(e);
    } finally {
      stream.close();
    }

    String annotationFieldName =
        String.join(ANNOTATION_CONNECTOR, field.getFieldName(), entityName);
    ParsedDocumentField annotatedField =
        new ParsedDocumentField(annotationFieldName, ngramBuffer.toString(), false);
    List<ParsedDocumentField> annotatedFields = new ArrayList<>();
    annotatedFields.add(annotatedField);
    return annotatedFields;
  }

}
