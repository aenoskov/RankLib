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

import org.apache.lucene.analysis.Analyzer;
import org.lemurproject.sifaka.buildindex.lucene.domain.IndexOptions;
import org.lemurproject.sifaka.luceneanalyzer.EnglishAnalyzerConfigurable;
import org.lemurproject.sifaka.luceneanalyzer.EnglishAnalyzerConfigurable.StemmerType;

/**
 * Instantiates a Lucene analyzer based on the user input for properties: stemmer, removeStopwords,
 * and ignoreCase.
 * 
 * @author cmw2
 *
 *         Nov 30, 2016
 */
public class ConfigurableAnalyzerFactory {

  public Analyzer getConfigurableAnalyzer(IndexOptions options) {
    EnglishAnalyzerConfigurable analyzer = new EnglishAnalyzerConfigurable();
    analyzer.setLowercase(options.isIgnoreCase());
    analyzer.setStopwordRemoval(options.isRemoveStopwords());
    StemmerType stemmerType = EnglishAnalyzerConfigurable.StemmerType.NONE;
    if (options.getStemmer().equalsIgnoreCase("kstem")) {
      stemmerType = EnglishAnalyzerConfigurable.StemmerType.KSTEM;
    } else if (options.getStemmer().equalsIgnoreCase("porter")) {
      stemmerType = EnglishAnalyzerConfigurable.StemmerType.PORTER;
    }
    analyzer.setStemmer(stemmerType);
    return analyzer;
  }

}
