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

import org.lemurproject.sifaka.textanalyzer.domain.Feature;

public class FeatureComparator implements Comparator<Feature> {

  @Override
  public int compare(Feature f1, Feature f2) {
    if (f1.getScore().doubleValue() > f2.getScore().doubleValue()) {
      return -1;
    } else if (f1.getScore().doubleValue() == f2.getScore().doubleValue()) {
      return 0;
    } else {
      return 1;
    }
  }

}
