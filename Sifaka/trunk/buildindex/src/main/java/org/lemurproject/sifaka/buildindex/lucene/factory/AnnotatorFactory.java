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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.lucene.analysis.Analyzer;
import org.lemurproject.sifaka.buildindex.lucene.annotator.AlignedNEAnnotator;
import org.lemurproject.sifaka.buildindex.lucene.annotator.AlignedNGramAnnotator;
import org.lemurproject.sifaka.buildindex.lucene.annotator.AlignedPhraseAnnotator;
import org.lemurproject.sifaka.buildindex.lucene.annotator.Annotator;

/**
 * Instatiates a Set of annotators bases on the user input for property: annotationTypes.
 * 
 * @author cmw2
 *
 *         Nov 30, 2016
 */
public class AnnotatorFactory {

  private final static List<String> nounPhraseTypes =
      new ArrayList<String>(Arrays.asList("NN", "NNP", "NNS", "NNPS"));

  private Map<String, Annotator> annotatorMap;

  public AnnotatorFactory(Analyzer analyzer) {
    annotatorMap = new HashMap<>();
    annotatorMap.put("ne", new AlignedNEAnnotator());
    annotatorMap.put("bigram", new AlignedNGramAnnotator(analyzer, 2, "bigram"));
    annotatorMap.put("trigram", new AlignedNGramAnnotator(analyzer, 3, "trigram"));
    annotatorMap.put("noun-phrase", new AlignedPhraseAnnotator(nounPhraseTypes));
  }

  public Set<String> getAnnotatorTypes() {
    return annotatorMap.keySet();
  }

  public List<Annotator> getAnnotators(String annotatorTypes) {
    List<Annotator> annotators = new ArrayList<>();

    String[] annotatorTypeNames = annotatorTypes.split(",");
    for (String annotatorTypeName : annotatorTypeNames) {
      if (annotatorMap.containsKey(annotatorTypeName.trim())) {
        annotators.add(annotatorMap.get(annotatorTypeName));
      } else {
        System.out.println("ERROR: No Annotator named: " + annotatorTypeName);
        System.out.println("Please define one of these annotator types: " + getAnnotatorTypes());
      }
    }

    return annotators;
  }

}
