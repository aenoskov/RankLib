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
package org.lemurproject.sifaka.buildindex.service;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.miscellaneous.PerFieldAnalyzerWrapper;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.FieldType;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.Fields;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.index.MultiFields;
import org.apache.lucene.index.Term;
import org.apache.lucene.index.Terms;
import org.apache.lucene.index.TermsEnum;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.lemurproject.sifaka.buildindex.domain.IndexConstants;
import org.lemurproject.sifaka.buildindex.domain.IndexDetails;
import org.lemurproject.sifaka.buildindex.domain.IndexFieldProperty;
import org.lemurproject.sifaka.buildindex.domain.IndexProperties;
import org.lemurproject.sifaka.buildindex.domain.IndexProperty;
import org.lemurproject.sifaka.buildindex.lucene.annotator.Annotator;
import org.lemurproject.sifaka.buildindex.lucene.documentparser.DocumentParser;
import org.lemurproject.sifaka.buildindex.lucene.domain.IndexOptions;
import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocument;
import org.lemurproject.sifaka.buildindex.lucene.domain.ParsedDocumentField;
import org.lemurproject.sifaka.buildindex.lucene.factory.AnnotatorFactory;
import org.lemurproject.sifaka.buildindex.lucene.factory.ConfigurableAnalyzerFactory;
import org.lemurproject.sifaka.buildindex.lucene.factory.DocumentParserFactory;
import org.lemurproject.sifaka.buildindex.lucene.factory.IndexOptionsFactory;

public class LuceneIndexServiceImpl implements IndexService {

  private static final Logger logger = Logger.getLogger(LuceneIndexServiceImpl.class.getName());

  @Override
  public IndexDetails buildIndex(String optionsPathString)
      throws IOException, InstantiationException, IllegalAccessException, IllegalArgumentException,
      InvocationTargetException, NoSuchMethodException, SecurityException {
    logger.log(Level.FINE, "Enter");
    long startTime = System.currentTimeMillis();

    // Configuration for properties file
    IndexOptionsFactory optionsFactory = new IndexOptionsFactory();
    IndexOptions options = optionsFactory.getIndexOptions(optionsPathString);

    ConfigurableAnalyzerFactory analyzerFactory = new ConfigurableAnalyzerFactory();
    Analyzer analyzer = analyzerFactory.getConfigurableAnalyzer(options);

    DocumentParserFactory docParserFactory = new DocumentParserFactory();
    DocumentParser docParser =
        docParserFactory.getDocumentParser(options.getDocumentFormat(), options.getDataDirectory());

    AnnotatorFactory annotatorFactory = new AnnotatorFactory(analyzer);
    List<Annotator> annotators = annotatorFactory.getAnnotators(options.getAnnotations());

    File rootDir = new File(options.getIndexDirectory());
    rootDir.mkdir();

    IndexWriter iWriter = createIndexWriter(options.getIndexDirectory(), docParser, analyzer);
    FieldType fieldType = getFieldType();

    // Parse documents and add annotations
    while (docParser.hasNextDocument()) {
      ParsedDocument parsedDoc = docParser.getNextDocument();
      if (parsedDoc != null) {
        Document luceneDoc = new Document();

        // Add document to search engine
        for (ParsedDocumentField docField : parsedDoc.getDocumentFields()) {
          if (docField.getContent() != null) {
            luceneDoc.add(new Field(docField.getFieldName(), docField.getContent(), fieldType));

            // Create annotations for the field if it is annotatable
            if (docField.isAnnotatable()) {
              for (Annotator annotator : annotators) {
                List<ParsedDocumentField> annotatedFields =
                    annotator.getAnnotationsForField(docField);
                // Add annotated fields to the search engine
                for (ParsedDocumentField annotatedField : annotatedFields) {
                  luceneDoc.add(new Field(annotatedField.getFieldName(),
                      annotatedField.getContent(), fieldType));
                }
              }
            }
          }
        }
        iWriter.addDocument(luceneDoc);
      }
    }
    iWriter.close();

    // Populate Index Details
    List<String> annotations = new ArrayList<>();
    for (Annotator annotator : annotators) {
      annotations.addAll(annotator.getAnnotationTypes());
    }


    // BufferedReader reader =
    // new BufferedReader(new InputStreamReader(getClass().getResourceAsStream("/" + pathString)));
    // StringJoiner fileString = new StringJoiner("");
    // String line;
    // while ((line = reader.readLine()) != null) {
    // fileString.add(line);
    // }
    // return fileString.toString();

    Properties properties = new Properties();
    ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
    properties.load(classLoader.getResourceAsStream("version.properties"));

    IndexDetails indexDetails = new IndexDetails();
    indexDetails.setBasePath(options.getIndexDirectory());
    indexDetails.setSearchDataPath(
        Paths.get(options.getIndexDirectory(), IndexConstants.LUCENE_INDEX_DIRECTORY).toString());
    indexDetails.setName(rootDir.getName());
    indexDetails.setAllFields(docParser.getAllFields());
    indexDetails.setSearchableFields(docParser.getSearchableFields());
    indexDetails.setLabelFields(docParser.getLabelFields());
    indexDetails.setAnnotationTypes(annotations);
    indexDetails.setBuildDate(LocalDateTime.now());
    indexDetails.setBuildVersion(properties.getProperty("version"));

    long endTime = System.currentTimeMillis();
    long elapsedTime = (endTime - startTime) / 1000;

    logger.log(Level.INFO,
        "Lucene Indexing time: " + LocalTime.MIN.plusSeconds(elapsedTime).toString());
    logger.log(Level.FINE, "Exit");

    return indexDetails;
  }

  /**
   * Creates the Lucene IndexWriter for writing document to the index.
   * 
   * @param indexDirectory
   * @param docParser
   * @param analyzer
   * @return
   * @throws IOException
   */
  private IndexWriter createIndexWriter(String indexDirectory, DocumentParser docParser,
      Analyzer analyzer) throws IOException {
    logger.log(Level.FINE, "Enter");

    Path path = Paths.get(indexDirectory, IndexConstants.LUCENE_INDEX_DIRECTORY);
    Directory directory = FSDirectory.open(path);

    // Searchable fields should use the analyzer defined in the properties file
    Map<String, Analyzer> analyzerPerField = new HashMap<String, Analyzer>();
    for (String field : docParser.getSearchableFields()) {
      analyzerPerField.put(field, analyzer);
    }

    // create a per-field analyzer wrapper using the StandardAnalyzer
    // Meta data fields such as ID should not have stopwords removed to be stemmed
    PerFieldAnalyzerWrapper perFieldAnalyzer =
        new PerFieldAnalyzerWrapper(new StandardAnalyzer(), analyzerPerField);

    IndexWriterConfig config = new IndexWriterConfig(perFieldAnalyzer);
    config.setOpenMode(OpenMode.CREATE);
    IndexWriter iwriter = new IndexWriter(directory, config);

    logger.log(Level.FINE, "Exit");
    return iwriter;
  }

  /**
   * Defines how fields are stored in Lucene.
   * 
   * @return
   */
  private FieldType getFieldType() {
    logger.log(Level.FINE, "Enter");
    FieldType fieldType = new FieldType();
    fieldType.setTokenized(true);
    fieldType.setStored(true);
    fieldType.setIndexOptions(
        org.apache.lucene.index.IndexOptions.DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS);
    fieldType.setStoreTermVectors(true);
    fieldType.setStoreTermVectorPositions(true);
    fieldType.setStoreTermVectorOffsets(true);
    logger.log(Level.FINE, "Exit");
    return fieldType;
  }

  @Override
  public IndexProperties getIndexProperties(IndexDetails indexDetails) throws IOException {
    logger.log(Level.FINE, "Enter");
    IndexProperties properties = new IndexProperties();

    properties.setIndexDetails(indexDetails);
    properties.setBasicIndexProperties(getBasicIndexProperties(indexDetails));
    properties.setIndexFieldProperties(calculateIndexFieldProperties(indexDetails));

    logger.log(Level.FINE, "Exit");
    return properties;
  }

  /**
   * Defines and calculates properties from the index that are name-value pairs.
   * 
   * @param indexDetails
   * @return
   * @throws IOException
   */
  public List<IndexProperty> getBasicIndexProperties(IndexDetails indexDetails) throws IOException {
    logger.log(Level.FINE, "Enter");
    List<IndexProperty> properties = new ArrayList<>();

    // Add properties that can be found in indexDetails
    IndexProperty version = new IndexProperty("Indexer Version", indexDetails.getBuildVersion());
    properties.add(version);
    IndexProperty date = new IndexProperty("Build Date", indexDetails.getBuildDate().toString());
    properties.add(date);
    if (indexDetails.getAnnotationTypes() != null) {
      IndexProperty annotations =
          new IndexProperty("Annotations", indexDetails.getAnnotationTypes().toString());
      properties.add(annotations);
    }
    if (indexDetails.getAllFields() != null) {
      IndexProperty allFields =
          new IndexProperty("All Fields", indexDetails.getAllFields().toString());
      properties.add(allFields);
    }
    if (indexDetails.getSearchableFields() != null) {
      IndexProperty searchableFields =
          new IndexProperty("Searchable Fields", indexDetails.getSearchableFields().toString());
      properties.add(searchableFields);
    }
    if (indexDetails.getLabelFields() != null) {
      IndexProperty labelFields =
          new IndexProperty("Label Fields", indexDetails.getLabelFields().toString());
      properties.add(labelFields);
    }

    // Add properties that need to be calculated
    Path indexPath = Paths.get(indexDetails.getBasePath(), IndexConstants.LUCENE_INDEX_DIRECTORY);
    Directory directory = FSDirectory.open(indexPath);
    DirectoryReader ireader = DirectoryReader.open(directory);

    properties.addAll(calculateBasicIndexProperties(indexDetails, ireader));
    properties.addAll(calculateLabelValues(indexDetails, ireader));
    properties.addAll(calculateLabelCounts(indexDetails, ireader));

    ireader.close();
    directory.close();

    logger.log(Level.FINE, "Exit");
    return properties;
  }

  /**
   * Calculates basic index properties such as number of documents in the index.
   * 
   * @param indexDetails
   * @param ireader
   * @return
   * @throws IOException
   */
  public List<IndexProperty> calculateBasicIndexProperties(IndexDetails indexDetails,
      DirectoryReader ireader) throws IOException {
    logger.log(Level.FINE, "Enter");
    List<IndexProperty> properties = new ArrayList<>();

    int totalDocs = ireader.maxDoc();
    indexDetails.setNumDocs(totalDocs);
    IndexProperty totalDocsProp =
        new IndexProperty("Number of Documents", String.valueOf(totalDocs));
    properties.add(totalDocsProp);

    logger.log(Level.FINE, "Exit");
    return properties;
  }

  /**
   * Calculates the the possible categories for each label type.
   * 
   * @param indexDetails
   * @param ireader
   * @return
   * @throws IOException
   */
  public List<IndexProperty> calculateLabelValues(IndexDetails indexDetails,
      DirectoryReader ireader) throws IOException {
    logger.log(Level.FINE, "Enter");
    List<IndexProperty> properties = new ArrayList<>();

    if (indexDetails.getLabelFields() != null) {
      Fields fields = MultiFields.getFields(ireader);
      for (String labelField : indexDetails.getLabelFields()) {
        List<String> labelValues = new ArrayList<>();
        // Calculate the total number of documents for each label
        Terms labels = fields.terms(labelField);
        if (labels != null) {
          TermsEnum labelsEnum = labels.iterator();
          while (labelsEnum.next() != null) {
            String label = labelsEnum.term().utf8ToString();
            labelValues.add(label);
          }
        }
        IndexProperty labelValuesProp =
            new IndexProperty("Label values for: " + labelField, labelValues.toString());
        properties.add(labelValuesProp);
      }
    }

    logger.log(Level.FINE, "Exit");
    return properties;
  }

  /**
   * Calculates how many documents each label category contains.
   * 
   * @param indexDetails
   * @param ireader
   * @return
   * @throws IOException
   */
  public List<IndexProperty> calculateLabelCounts(IndexDetails indexDetails,
      DirectoryReader ireader) throws IOException {
    logger.log(Level.FINE, "Enter");
    List<IndexProperty> properties = new ArrayList<>();

    if (indexDetails.getLabelFields() != null) {
      Fields fields = MultiFields.getFields(ireader);
      for (String labelField : indexDetails.getLabelFields()) {
        Terms labels = fields.terms(labelField);
        if (labels != null) {
          TermsEnum labelsEnum = labels.iterator();
          while (labelsEnum.next() != null) {
            String label = labelsEnum.term().utf8ToString();
            Term labelTerm = new Term(labelField, label);
            int labelCount = ireader.docFreq(labelTerm);

            IndexProperty labelCountProp = new IndexProperty(
                "Number of Documents for: " + labelField + "-" + label, String.valueOf(labelCount));
            properties.add(labelCountProp);

          }
        }
      }
    }

    logger.log(Level.FINE, "Exit");
    return properties;
  }

  /**
   * Calculates how many terms are in each Lucene document field.
   * 
   * @param indexDetails
   * @return
   */
  public List<IndexFieldProperty> calculateIndexFieldProperties(IndexDetails indexDetails) {
    logger.log(Level.FINE, "Enter");
    List<IndexFieldProperty> properties = new ArrayList<>();

    try {
      Path path = Paths.get(indexDetails.getBasePath(), IndexConstants.LUCENE_INDEX_DIRECTORY);
      Directory directory = FSDirectory.open(path);
      DirectoryReader ireader = DirectoryReader.open(directory);

      Fields fields = MultiFields.getFields(ireader);
      for (String field : fields) {
        Terms terms = fields.terms(field);
        TermsEnum termsEnum = terms.iterator();
        int count = 0;
        while (termsEnum.next() != null) {
          count++;
        }
        IndexFieldProperty property = new IndexFieldProperty();

        // Determine the field and annotation based on the field name
        // An underscore in the field name means that it is an annotation field
        String fieldName = field;
        String annotationName = "-";
        if (field.contains("_")) {
          String[] fieldNameParts = fieldName.split("_");
          fieldName = fieldNameParts[0];
          annotationName = fieldNameParts[1];
        }
        property.setField(fieldName);
        property.setAnnotation(annotationName);
        property.setCount(count);
        properties.add(property);
      }
      directory.close();
      ireader.close();
    } catch (IOException e) {
      System.out.print("Could not open index fields: " + indexDetails.getBasePath());
    }
    logger.log(Level.FINE, "Exit");
    return properties;
  }

}
