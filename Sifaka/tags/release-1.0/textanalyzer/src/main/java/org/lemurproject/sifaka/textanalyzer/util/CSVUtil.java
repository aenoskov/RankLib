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

import java.io.File;
import java.util.List;

import org.lemurproject.sifaka.textanalyzer.domain.CSVFormatCompatable;

/**
 * Writes any list of CSVCompatable object to a CSV file.
 * 
 * @author cmw2
 *
 *         Dec 17, 2016
 */
public interface CSVUtil {

  /**
   * Writes any list of CSVCompatable object to a CSV file.
   * 
   * @param file
   * @param list
   */
  void writeContentToCSVFile(File file, List<? extends CSVFormatCompatable> list);

}
