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

import java.io.FileNotFoundException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocument;
import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;
import org.lemurproject.sifaka.buildindex.lucene.domain.Review;

import com.google.gson.Gson;

/**
 * 
 * A Json implementation of a DocumentParser used for the McAuley data.
 * 
 * @author cmw2
 *
 *         Dec 16, 2016
 */
public class JsonDocumentParser implements DocumentParser {

  private final static String EXTERNALID_FIELD = "externalId";
  private final static String INTERNALID_FIELD = "internalId";
  private final static String CLASS_FIELD = "class";
  private final static String BODY_FIELD = "body";

  private Scanner scanner;
  private Gson gson;
  private int reviewNum;

  public JsonDocumentParser(String dataDirectory) throws FileNotFoundException {
    scanner = new Scanner(Paths.get(dataDirectory).toFile());
    gson = new Gson();
    reviewNum = 1;
  }

  @Override
  public List<String> getAllFields() {
    return new ArrayList<String>(
        Arrays.asList(EXTERNALID_FIELD, INTERNALID_FIELD, CLASS_FIELD, BODY_FIELD));
  }

  @Override
  public List<String> getLabelFields() {
    return new ArrayList<String>(Arrays.asList(CLASS_FIELD));
  }

  @Override
  public List<String> getSearchableFields() {
    return new ArrayList<String>(Arrays.asList(BODY_FIELD));
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifakaluceneindexer.documentparser.DocumentParser#getNextDocument()
   */
  public ParsedDocument getNextDocument() {
    ParsedDocument doc = null;
    while (scanner.hasNext() && reviewNum % 7 != 0) {
      reviewNum++;
      scanner.nextLine();
    }
    if (scanner.hasNext()) {
      String reviewJsonString = scanner.nextLine();
      Review review = gson.fromJson(reviewJsonString, Review.class);

      // Chunk reviews into larger categories
      // 1-2 stars is poor
      // 3 stars is ok
      // 4-5 stars is good
      String ratingCategory = "poor";
      if (review.getOverall() >= 4) {
        ratingCategory = "good";
      } else if (review.getOverall() == 3) {
        ratingCategory = "neutral";
      }

      doc = new ParsedDocument();
      doc.setDocumentFields(new ArrayList<>());

      ParsedDocumentField externalIdField =
          new ParsedDocumentField(EXTERNALID_FIELD, String.valueOf(reviewNum), false);
      doc.getDocumentFields().add(externalIdField);

      ParsedDocumentField internalIdField =
          new ParsedDocumentField(INTERNALID_FIELD, String.valueOf(reviewNum), false);
      doc.getDocumentFields().add(internalIdField);

      ParsedDocumentField classField = new ParsedDocumentField(CLASS_FIELD, ratingCategory, false);
      doc.getDocumentFields().add(classField);

      ParsedDocumentField bodyField =
          new ParsedDocumentField(BODY_FIELD, review.getReviewText(), true);
      doc.getDocumentFields().add(bodyField);

      reviewNum++;
    } else {
      scanner.close();
    }
    return doc;
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifakaluceneindexer.documentparser.DocumentParser#hasNextDocument()
   */
  public boolean hasNextDocument() {
    if (scanner.hasNext()) {
      return true;
    }
    scanner.close();
    return false;
  }

}
