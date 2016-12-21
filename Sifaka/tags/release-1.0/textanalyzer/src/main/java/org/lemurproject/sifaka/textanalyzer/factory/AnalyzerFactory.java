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
package org.lemurproject.sifaka.textanalyzer.factory;

import org.lemurproject.sifaka.luceneanalyzer.EnglishAnalyzerConfigurable;

public class AnalyzerFactory {

  public EnglishAnalyzerConfigurable getAnalyzer() {
    EnglishAnalyzerConfigurable analyzer = new EnglishAnalyzerConfigurable();
    analyzer.setLowercase(true);
    analyzer.setStopwordRemoval(true);
    analyzer.setStemmer(EnglishAnalyzerConfigurable.StemmerType.KSTEM);

    return analyzer;
  }

}
