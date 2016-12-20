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
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.textanalyzer.domain.DisplayDocument;
import org.lemurproject.sifaka.textanalyzer.factory.AnalyzerFactory;
import org.lemurproject.sifaka.textanalyzer.util.LuceneAnalysisUtilImpl;

public class SearchLuceneServiceImpl implements SearchService {

  private Analyzer analyzer;
  private LuceneAnalysisUtilImpl util;

  public SearchLuceneServiceImpl() {
    AnalyzerFactory analyzerFactory = new AnalyzerFactory();
    analyzer = analyzerFactory.getAnalyzer();
    util = new LuceneAnalysisUtilImpl();
  }

  public DisplayDocument getDocumentById(IndexDetails indexDetails, int docID) throws IOException {
    Directory directory = FSDirectory.open(Paths.get(indexDetails.getSearchDataPath()));
    DirectoryReader ireader = DirectoryReader.open(directory);
    Document doc = ireader.document(docID);

    Map<String, String> fieldsAndContent = new HashMap<>();
    for (String field : indexDetails.getSearchableFields()) {
      if (field != null && doc.getField(field) != null) {
        fieldsAndContent.put(field, doc.getField(field).stringValue());
      }
    }
    DisplayDocument displayDoc = new DisplayDocument();
    displayDoc.setDocName(doc.getField("externalId").stringValue());
    displayDoc.setDocId(doc.getField("internalId").stringValue());
    displayDoc.setFieldsAndContent(fieldsAndContent);

    directory.close();
    ireader.close();
    return displayDoc;
  }

  public List<Integer> search(IndexDetails indexDetails, List<Integer> set, String queryString)
      throws IOException, ParseException {
    Directory directory = FSDirectory.open(Paths.get(indexDetails.getSearchDataPath()));
    DirectoryReader ireader = DirectoryReader.open(directory);
    IndexSearcher isearcher = new IndexSearcher(ireader);

    QueryParser parser = new QueryParser("body", analyzer);
    org.apache.lucene.search.Query query =
        parser.parse(util.getQueryString(queryString, indexDetails.getSearchableFields()));

    int totalDocs = ireader.maxDoc();
    ScoreDoc[] hits = isearcher.search(query, totalDocs).scoreDocs;

    directory.close();
    ireader.close();

    return util.filterScoreDocsBySet(hits, set);
  }

}
