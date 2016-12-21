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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringJoiner;

import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;

import edu.stanford.nlp.ie.AbstractSequenceClassifier;
import edu.stanford.nlp.ie.crf.CRFClassifier;
import edu.stanford.nlp.ling.CoreAnnotations;
import edu.stanford.nlp.ling.CoreLabel;

/**
 * Creates named entity annotations such as: person, location, organization.
 * 
 * @author cmw2
 *
 *         Dec 1, 2016
 */
public class AlignedNEAnnotator implements Annotator {

  private static final String serializedClassifier =
      "classifiers/english.all.3class.distsim.crf.ser.gz";
  private AbstractSequenceClassifier<CoreLabel> classifier;

  private final static String PERSON_TYPE = "person";
  private final static String LOCATION_TYPE = "location";
  private final static String ORGANIZATION_TYPE = "organization";
  private final static String ANNOTATION_CONNECTOR = "_";
  private static final String PADDED_TOKEN = "a";

  public AlignedNEAnnotator() {
    // ClassLoader classLoader = getClass().getClassLoader();
    // String classifierPath = classLoader.getResource(serializedClassifier).toString();
    // classifier = CRFClassifier.getClassifierNoExceptions(classifierPath);
    classifier = CRFClassifier.getClassifierNoExceptions(serializedClassifier);
  }

  @Override
  public List<String> getAnnotationTypes() {
    return new ArrayList<String>(Arrays.asList(PERSON_TYPE, LOCATION_TYPE, ORGANIZATION_TYPE));
  }

  @Override
  public List<ParsedDocumentField> getAnnotationsForField(ParsedDocumentField field) {
    List<List<CoreLabel>> out = classifier.classify(field.getContent());
    List<ParsedDocumentField> annotationFields = new ArrayList<>();

    for (String annotationType : getAnnotationTypes()) {
      String annotationFieldName =
          String.join(ANNOTATION_CONNECTOR, field.getFieldName(), annotationType);
      String annotationFieldContent = createEntityField(out, annotationType);
      ParsedDocumentField annotationField =
          new ParsedDocumentField(annotationFieldName, annotationFieldContent, false);
      annotationFields.add(annotationField);
    }

    return annotationFields;
  }

  /**
   * Creates a String that is the field content for each entityType. This string contains a padded
   * token for each word that is not the entity type. It also combines multiword entities using an
   * "_" and adjusts the padding.
   * 
   * @param classifierOutput
   * @param entityType
   * @return
   */
  private String createEntityField(List<List<CoreLabel>> classifierOutput, String entityType) {
    List<String> fieldTokens = new ArrayList<>();

    for (List<CoreLabel> sentence : classifierOutput) {
      for (int i = 0; i < sentence.size(); i++) {
        CoreLabel word = sentence.get(i);
        String tokenclass = word.get(CoreAnnotations.AnswerAnnotation.class).toLowerCase();
        String token = word.word().toLowerCase();

        if (!tokenclass.equals(entityType)) {
          // Add padding (don't add if it is punctuation)
          if (token.length() > 1 || token.matches("[a-zA-Z0-9]")) {
            fieldTokens.add(PADDED_TOKEN);
          }
        } else { // We have found an entity of matching type.
          // If we are at the end of a sentence, just add the token since we know we are at the end
          // of the entity
          if (i >= sentence.size() - 1) {
            fieldTokens.add(token);
          } else {
            // Iterate over the tokens ahead and combine them if they are the same entity
            String nexttokenclass = null;
            StringJoiner joiner = new StringJoiner(ANNOTATION_CONNECTOR);
            joiner.add(token);
            do {
              CoreLabel nextWord = sentence.get(i + 1);
              nexttokenclass =
                  nextWord.getString(CoreAnnotations.AnswerAnnotation.class).toLowerCase();
              String nexttoken = nextWord.word().toLowerCase();
              if (nexttokenclass.equals(entityType)) {
                joiner.add(nexttoken);
                i++;
              }

            } while (i < sentence.size() - 1 && nexttokenclass.equals(entityType));
            fieldTokens.add(joiner.toString().replace(".", ""));
            // Add padding to compensate for combined entities
            int numWords = joiner.toString().split(ANNOTATION_CONNECTOR).length;
            for (int j = 1; j < numWords; j++) {
              fieldTokens.add(PADDED_TOKEN);
            }
          }

        }
      }
    }

    return String.join(" ", fieldTokens);
  }

}
