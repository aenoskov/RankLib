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
package org.lemurproject.sifaka.buildindex.util;

import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.nio.file.Path;
import java.nio.file.Paths;

import org.lemurproject.sifaka.buildindex.domain.IndexProperties;

/**
 * @author cmw2
 *
 *         Dec 11, 2016
 */
public class BuildIndexUtil {

  public void writeObjectToFile(String basePath, String fileName, Object content) {

    try {
      Path manifestPath = Paths.get(basePath, fileName);
      FileOutputStream fout = new FileOutputStream(manifestPath.toString());
      ObjectOutputStream oos = new ObjectOutputStream(fout);
      oos.writeObject(content);
      oos.close();

    } catch (Exception ex) {
      ex.printStackTrace();
    }

  }

  public void writeStringToFile(String basePath, String fileName, String content) {

    try {
      Path manifestPath = Paths.get(basePath, fileName);
      BufferedWriter writer = new BufferedWriter(new FileWriter(manifestPath.toString()));
      writer.write(content);
      writer.close();
    } catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  public IndexProperties getIndexProperties(String basePath, String fileName) {

    IndexProperties indexProperties = null;
    try {
      InputStream file = new FileInputStream(Paths.get(basePath, fileName).toString());
      InputStream buffer = new BufferedInputStream(file);
      ObjectInput input = new ObjectInputStream(buffer);
      try {
        indexProperties = (IndexProperties) input.readObject();
      } finally {
        input.close();
      }
    } catch (ClassNotFoundException | IOException e) {
      System.out.println("ClassNotFound when opening index options for: "
          + Paths.get(basePath, fileName).toString());
    }
    return indexProperties;
  }

}
