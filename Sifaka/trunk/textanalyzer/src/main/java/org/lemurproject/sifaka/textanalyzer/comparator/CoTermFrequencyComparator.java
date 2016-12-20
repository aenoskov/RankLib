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
package org.lemurproject.sifaka.textanalyzer.comparator;

import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

import org.lemurproject.sifaka.textanalyzer.domain.CoOccurrence;

public class CoTermFrequencyComparator implements Comparator<String> {

  Map<String, CoOccurrence> map = new HashMap<>();

  public CoTermFrequencyComparator(Map<String, CoOccurrence> map) {
    this.map.putAll(map);
  }

  @Override
  public int compare(String s1, String s2) {
    if (map.get(s1).getTermFrequency() >= map.get(s2).getTermFrequency()) {
      return -1;
    } else {
      return 1;
    }
  }

}
