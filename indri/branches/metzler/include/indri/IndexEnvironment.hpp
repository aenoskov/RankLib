/*==========================================================================
 * Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
*/


//
// IndexEnvironment
//
// 19 July 2004 -- tds
//

#ifndef INDRI_INDEXENVIRONMENT_HPP
#define INDRI_INDEXENVIRONMENT_HPP

#include <string>
#include "indri/Parameters.hpp"
#include "indri/HTMLParser.hpp"
#include "indri/Repository.hpp"
#include "indri/IndriParser.hpp"
#include "indri/DocumentIterator.hpp"
#include "indri/AnchorTextAnnotator.hpp"
#include "indri/DocumentIteratorFactory.hpp"
#include "indri/ParserFactory.hpp"
#include "indri/FileClassEnvironmentFactory.hpp"
#include <map>

struct IndexStatus {
  enum action_code {
    FileOpen,
    FileSkip,
    FileError,
    FileClose,
    DocumentCount
  };

  virtual void operator () ( int code, const std::string& documentPath, const std::string& error, int documentsIndexed, int documentsSeen ) {
    status( code, documentPath, error, documentsIndexed, documentsSeen );
  }

  virtual void status( int code, const std::string& documentPath, const std::string& error, int documentsIndexed, int documentsSeen ) {};
};

class IndexEnvironment {
private:
  IndexStatus* _callback;
  Parameters* _options;

  std::string _repositoryPath;
  Repository _repository;
  int _documents;
  std::string _error;

  std::string _anchorTextRoot;
  std::string _documentRoot;

  Parameters _parameters;
  FileClassEnvironmentFactory _fileClassFactory;

  AnchorTextAnnotator _annotator;
  std::map<std::string, FileClassEnvironment*> _environments;

  int _documentsIndexed;
  int _documentsSeen;

  void _getParsingContext( indri::Parser** parser,
                           DocumentIterator** iterator,
                           const std::string& extension );

public:
  IndexEnvironment();
  ~IndexEnvironment();

  void setAnchorTextPath( const std::string& documentRoot, const std::string& anchorTextRoot );
  void addFileClass( const std::string& name, 
                     const std::string& iterator,
                     const std::string& parser,
                     const std::string& startDocTag,
                     const std::string& endDogTag,
                     const std::string& endMetadataTag,
                     const std::vector<std::string>& include,
                     const std::vector<std::string>& exclude,
                     const std::vector<std::string>& index,
                     const std::vector<std::string>& metadata, 
                     const std::map<std::string,std::string>& conflations );
  void setIndexedFields( const std::vector<std::string>& fieldNames );
  void setNumericField( const std::string& fieldName, bool isNumeric );
  void setMetadataIndexedFields( const std::vector<std::string>& fieldNames );
  void setStopwords( const std::vector<std::string>& stopwords );
  void setStemmer( const std::string& stemmer );
  void setMemory( UINT64 memory );
  void setSegmenter( const std::string& segmenter );

  void create( const std::string& repositoryPath, IndexStatus* callback = 0 );
  void open( const std::string& repositoryPath, IndexStatus* callback = 0 );
  void close();

  void addFile( const std::string& fileName );
  void addFile( const std::string& fileName, const std::string& fileClass );
  void addString( const std::string& documentString, const std::string& fileClass, const std::vector<MetadataPair>& metadata );
  void addParsedDocument( ParsedDocument* document );
};

#endif // INDRI_INDEXENVIRONMENT_HPP

