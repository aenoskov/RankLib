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
package org.lemurproject.sifaka.buildindex;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.time.LocalTime;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.lemurproject.sifaka.buildindex.domain.IndexConstants;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.buildindex.domain.IndexProperties;
import org.lemurproject.sifaka.buildindex.service.IndexService;
import org.lemurproject.sifaka.buildindex.service.LuceneIndexServiceImpl;
import org.lemurproject.sifaka.buildindex.util.BuildIndexUtil;

/**
 * Main class for building a Sifaka index. Accepts a properties file as an arg. Example property
 * file: sifakaBuildIndex.properties
 * 
 * <pre>
 * 
 * documentFormat=trec
 *
 * #data options 
 * dataDirectory=PATH_TO_DATA
 * indexDirectory=PATH_TO_INDEX 
 *
 * #analyzer options 
 * stemmer=kstem 
 * removeStopwords=true 
 * ignoreCase=true
 *
 * # CSV list of annotations # Options: [ne, noun-phrase, bigram, trigram]
 * annotation.types=ne,bigram,trigram,noun-phrase
 * </pre>
 * 
 * @author cmw2
 *
 *         Dec 10, 2016
 */
public class BuildIndex {

  private static final Logger logger = Logger.getLogger(BuildIndex.class.getName());

  /**
   * @param args
   * @throws IOException
   * @throws SecurityException
   * @throws NoSuchMethodException
   * @throws InvocationTargetException
   * @throws IllegalArgumentException
   * @throws IllegalAccessException
   * @throws InstantiationException
   */
  public static void main(String[] args)
      throws InstantiationException, IllegalAccessException, IllegalArgumentException,
      InvocationTargetException, NoSuchMethodException, SecurityException, IOException {
    logger.log(Level.FINE, "Enter");

    long startTime = System.currentTimeMillis();

    BuildIndexUtil util = new BuildIndexUtil();

    // Build the Index
    // TODO: Choose service implementation based on an arg defining which search engine to use
    IndexService luceneIndexService = new LuceneIndexServiceImpl();
    IndexDetails indexDetails = luceneIndexService.buildIndex(args[0]);

    // Creates a human readable properties file for the index
    util.writeStringToFile(indexDetails.getBasePath(),
        IndexConstants.LUCENE_INDEX_DETAILS_PROPERTIES_FILE, indexDetails.toProperties());

    // Writes an object which can be opened by the Sifaka TextMiner and displayed
    IndexProperties indexProperties = luceneIndexService.getIndexProperties(indexDetails);
    util.writeObjectToFile(indexDetails.getBasePath(), IndexConstants.LUCENE_INDEX_PROPERTIES_FILE,
        indexProperties);

    long endTime = System.currentTimeMillis();
    long elapsedTime = (endTime - startTime) / 1000;

    logger.log(Level.INFO,
        "Sifaka BuildIndex time: " + LocalTime.MIN.plusSeconds(elapsedTime).toString());
    logger.log(Level.FINE, "Exit");
  }

}
