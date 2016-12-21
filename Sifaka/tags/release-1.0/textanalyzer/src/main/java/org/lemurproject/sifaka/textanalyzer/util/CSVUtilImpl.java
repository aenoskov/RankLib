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

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.List;

import org.lemurproject.sifaka.textanalyzer.domain.CSVFormatCompatable;

public class CSVUtilImpl implements CSVUtil {

  public void writeContentToCSVFile(File file, List<? extends CSVFormatCompatable> list) {

    if (list != null && list.size() > 0 && file != null) {

      try {
        FileOutputStream fos = new FileOutputStream(file);
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));

        bw.write(list.get(0).getColumnHeaders());
        bw.newLine();

        for (CSVFormatCompatable item : list) {
          bw.write(item.toCSVLine());
          bw.newLine();
        }

        bw.close();
      } catch (IOException e) {
        System.out.println("Error saving results to file: " + file.getAbsolutePath());
      }
    }

  }

}
