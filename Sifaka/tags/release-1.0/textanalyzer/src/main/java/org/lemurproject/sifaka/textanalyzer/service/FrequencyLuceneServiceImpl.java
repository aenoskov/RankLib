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
package org.lemurproject.sifaka.textanalyzer.service;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.Terms;
import org.apache.lucene.index.TermsEnum;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.textanalyzer.comparator.CollectionFrequencyComparator;
import org.lemurproject.sifaka.textanalyzer.comparator.DocumentFrequencyComparator;
import org.lemurproject.sifaka.textanalyzer.domain.TermFrequency;

public class FrequencyLuceneServiceImpl implements FrequencyService {

  public List<TermFrequency> getFrequencies(IndexDetails indexDetails, List<Integer> set,
      String resultType, int numResults, String sortBy) throws IOException {
    Directory directory = FSDirectory.open(Paths.get(indexDetails.getSearchDataPath()));
    DirectoryReader ireader = DirectoryReader.open(directory);

    Map<String, TermFrequency> combinedEntityHits = new HashMap<>();
    List<String> resultFields = new ArrayList<>();
    for (String field : indexDetails.getSearchableFields()) {
      String resultField = String.join("_", field, resultType);
      resultFields.add(resultField);
    }

    for (int i = 0; i < ireader.maxDoc(); i++) {
      Set<String> docTerms = new HashSet<>(); // Store terms in doc so they are not double counted
                                              // if they are in multiple fields
      if (set == null || set.contains(Integer.valueOf(i))) {
        for (String field : resultFields) {
          Terms terms = ireader.getTermVector(i, field);
          if (terms != null) {
            TermsEnum termsEnum = terms.iterator();
            while (termsEnum.next() != null) {
              String term = termsEnum.term().utf8ToString();
              boolean termAdded = docTerms.add(term);
              combinedEntityHits.putIfAbsent(term, new TermFrequency());
              TermFrequency termFreq = combinedEntityHits.get(termsEnum.term().utf8ToString());
              String termString = termsEnum.term().utf8ToString().replace("_", " ");
              termFreq.setTerm(termString);
              if (termAdded) {
                termFreq.setDocumentFrequency(termFreq.getDocumentFrequency() + 1);
              }
              termFreq.setCollectionTermFrequency(
                  termFreq.getCollectionTermFrequency() + termsEnum.totalTermFreq());
              combinedEntityHits.put(termsEnum.term().utf8ToString(), termFreq);
            }
          }
        }
      }
    }

    directory.close();
    ireader.close();

    int maxResults = Math.min(combinedEntityHits.size(), numResults);
    return sortMapByTermFrequency(combinedEntityHits, sortBy).subList(0, maxResults);
  }

  private List<TermFrequency> sortMapByTermFrequency(Map<String, TermFrequency> map,
      String sortBy) {
    Comparator<String> comparator = null;
    if (sortBy.equalsIgnoreCase("document frequency")) {
      comparator = new DocumentFrequencyComparator(map);
    } else if (sortBy.equalsIgnoreCase("collection term frequency")) {
      comparator = new CollectionFrequencyComparator(map);
    }

    TreeMap<String, TermFrequency> result = new TreeMap<>();
    if (comparator != null) {
      result = new TreeMap<>(comparator);
    }

    result.putAll(map);
    List<TermFrequency> termList = new ArrayList<>(result.values());
    return termList;
  }

}
