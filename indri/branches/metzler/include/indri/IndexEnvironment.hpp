/*==========================================================================
  Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.

  Use of the Lemur Toolkit for Language Modeling and Information Retrieval
  is subject to the terms of the software license set forth in the LICENSE
  file included with this software, and also available at
  http://www.cs.cmu.edu/~lemur/license.html 
  as well as the conditions below.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  3. The names "Indri", "Center for Intelligent Information Retrieval", 
  "CIIR", and "University of Massachusetts" must not be used to
  endorse or promote products derived from this software without
  prior written permission. To obtain permission, contact
  indri-info@ciir.cs.umass.edu.

  4. Products derived from this software may not be called "Indri" nor 
  may "Indri" appear in their names without prior written permission of 
  the University of Massachusetts. To obtain permission, contact 
  indri-info@ciir.cs.umass.edu.

  THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF MASSACHUSETTS AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
  ==========================================================================
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

