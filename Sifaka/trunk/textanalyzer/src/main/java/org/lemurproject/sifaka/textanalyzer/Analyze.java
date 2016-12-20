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
package org.lemurproject.sifaka.textanalyzer;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.lemurproject.sifaka.buildindex.domain.IndexConstants;
import org.lemurproject.sifaka.buildindex.domain.IndexProperties;
import org.lemurproject.sifaka.buildindex.util.BuildIndexUtil;
import org.lemurproject.sifaka.textanalyzer.domain.TermFrequency;
import org.lemurproject.sifaka.textanalyzer.service.FrequencyLuceneServiceImpl;

public class Analyze {

  public static void main(String[] args) throws IOException {
    FrequencyLuceneServiceImpl freqService = new FrequencyLuceneServiceImpl();
    List<String> fields = new ArrayList<>();
    fields.add("body");
    BuildIndexUtil indexUtil = new BuildIndexUtil();
    IndexProperties indexProperties = indexUtil.getIndexProperties(
        "C:\\dev\\Indexes_1.0\\toyTextDocs\\", IndexConstants.LUCENE_INDEX_PROPERTIES_FILE);
    List<TermFrequency> freqs = freqService.getFrequencies(indexProperties.getIndexDetails(), null,
        "organization", 100, "document frequency");

    for (TermFrequency freq : freqs) {
      System.out.println(freq.toCSVLine());
    }

  }

}
