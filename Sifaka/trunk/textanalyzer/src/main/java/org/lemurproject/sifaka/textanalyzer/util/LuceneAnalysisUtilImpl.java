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
package org.lemurproject.sifaka.textanalyzer.util;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringJoiner;

import org.apache.lucene.search.ScoreDoc;

public class LuceneAnalysisUtilImpl implements AnalysisUtil {

  public String getQueryString(String query, List<String> fields) {
    StringJoiner queryJoiner = new StringJoiner(" OR ");
    fields.forEach((field) -> {
      queryJoiner.add(String.join("", field, ":(", query, ")"));
    });
    String queryString = String.join("", "(", queryJoiner.toString(), ")");
    return queryString;
  }

  public List<Integer> convertScoreDocsToDocIdList(ScoreDoc[] hits) {
    List<ScoreDoc> hitsList = new ArrayList<>(Arrays.asList(hits));
    List<Integer> hitDocIDs = new ArrayList<>();
    hitsList.forEach(hit -> hitDocIDs.add(Integer.valueOf(hit.doc)));
    return hitDocIDs;
  }

  public List<Integer> filterScoreDocsBySet(ScoreDoc[] hits, List<Integer> setDocs) {
    List<Integer> hitDocIDs = convertScoreDocsToDocIdList(hits);

    if (setDocs != null) {
      hitDocIDs.retainAll(setDocs);
    }

    return hitDocIDs;
  }

}
