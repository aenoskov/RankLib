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

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringJoiner;

import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;

import edu.stanford.nlp.ling.CoreLabel;
import edu.stanford.nlp.ling.HasWord;
import edu.stanford.nlp.ling.TaggedWord;
import edu.stanford.nlp.process.CoreLabelTokenFactory;
import edu.stanford.nlp.process.DocumentPreprocessor;
import edu.stanford.nlp.process.PTBTokenizer;
import edu.stanford.nlp.process.TokenizerFactory;
import edu.stanford.nlp.tagger.maxent.MaxentTagger;

/**
 * Creates Noun Phrase annotations.
 * 
 * @author cmw2
 *
 *         Dec 11, 2016
 */
public class AlignedPhraseAnnotator implements Annotator {

  private static final String taggerName = "models/english-left3words-distsim.tagger";

  private static final String ANNOTATION_TYPE = "noun-phrase";
  private static final String ANNOTATION_CONNECTOR = "_";
  private static final String PADDED_TOKEN = "a";

  private MaxentTagger tagger;
  private List<String> phraseTypes;
  private TokenizerFactory<CoreLabel> ptbTokenizerFactory;

  public AlignedPhraseAnnotator(List<String> phraseTypes) {
    // ClassLoader classLoader = getClass().getClassLoader();
    // String modelPath = classLoader.getResource(taggerName).toString();
    // this.tagger = new MaxentTagger(modelPath);
    this.tagger = new MaxentTagger(taggerName);
    this.phraseTypes = phraseTypes;
    ptbTokenizerFactory =
        PTBTokenizer.factory(new CoreLabelTokenFactory(), "untokenizable=noneKeep");
  }

  /*
   * (non-Javadoc)
   * 
   * @see org.lemurproject.sifaka.buildindex.lucene.annotator.Annotator#getAnnotationTypes()
   */
  @Override
  public List<String> getAnnotationTypes() {
    return new ArrayList<String>(Arrays.asList(ANNOTATION_TYPE));
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
    List<String> annotationSentences = new ArrayList<>();

    InputStream is = new ByteArrayInputStream(field.getContent().getBytes());
    BufferedReader r = new BufferedReader(new InputStreamReader(is));
    DocumentPreprocessor documentPreprocessor = new DocumentPreprocessor(r);
    documentPreprocessor.setTokenizerFactory(ptbTokenizerFactory);
    for (List<HasWord> sentence : documentPreprocessor) {
      if (sentence.size() > 1) {
        List<TaggedWord> tSentence = tagger.tagSentence(sentence);
        annotationSentences.add(createPhraseSentence(phraseTypes, tSentence));
      }
    }

    String annotationFieldName =
        String.join(ANNOTATION_CONNECTOR, field.getFieldName(), ANNOTATION_TYPE);
    ParsedDocumentField annotatedField =
        new ParsedDocumentField(annotationFieldName, String.join(" ", annotationSentences), false);
    List<ParsedDocumentField> annotatedFields = new ArrayList<>();
    annotatedFields.add(annotatedField);
    return annotatedFields;
  }

  /**
   * Takes a sentence that is tagged by the noun phrase tagger and a phrase type and returns that
   * sentence with stopwords in the place of all words that are not the noun phrase and noun phrases
   * joined by underscores. This data will be indexed in the noun phrase field in Lucene. The
   * stopwords enable position information to stored without enlarging the index.
   * 
   * @param phraseType
   * @param tSentence
   * @return
   */
  private String createPhraseSentence(List<String> phraseTypes, List<TaggedWord> tSentence) {
    StringJoiner sentenceBuffer = new StringJoiner(" ");
    for (int i = 0; i < tSentence.size(); i++) {
      TaggedWord tw = tSentence.get(i);
      if (!phraseTypes.contains(tw.tag())) {
        // Add padding
        if (tw.word().length() > 1 || tw.word().matches("[a-zA-Z0-9]")) {
          sentenceBuffer.add(PADDED_TOKEN);
        }
      } else if (tw.word().length() == 1 && !tw.word().matches("[a-zA-Z0-9]")) {
        sentenceBuffer.add(PADDED_TOKEN);
      } else { // We have found an entity of matching type.
        // If we are at the end of a sentence, just add the token since we know we are at the end
        // of the entity
        if (i >= tSentence.size() - 1) {
          // sentenceBuffer.add(tw.word().toLowerCase());
          sentenceBuffer.add(PADDED_TOKEN);
        } else {
          // Iterate over the tokens ahead and combine them if they are the same entity
          String nextwordclass = null;
          StringJoiner phraseBuffer = new StringJoiner(ANNOTATION_CONNECTOR);
          phraseBuffer.add(tw.word().toLowerCase());
          do {
            TaggedWord nextWord = tSentence.get(i + 1);
            nextwordclass = nextWord.tag();
            String nextword = nextWord.word().toLowerCase();
            if (phraseTypes.contains(nextwordclass)) {
              if (tw.word().length() > 1 || tw.word().matches("[a-zA-Z0-9]")) {
                phraseBuffer.add(nextword);
                i++;
              } else {
                nextwordclass = "";
              }
            }

          } while (i < tSentence.size() - 1 && phraseTypes.contains(nextwordclass));

          int numWords = phraseBuffer.toString().split(ANNOTATION_CONNECTOR).length;

          // Only use phrases great in length than 2
          int start = 0;
          if (numWords >= 2) {
            sentenceBuffer.add(phraseBuffer.toString().replace(".", "").replace("-", "_")
                .replace("/", "_per_").replace("&", "_and_"));
            start = 1;
          }

          for (int j = start; j < numWords; j++) {
            sentenceBuffer.add(PADDED_TOKEN);
          }
        }
      }

    }
    return sentenceBuffer.toString();
  }

}
