/*==========================================================================
 * Copyright (c) 2003-2004 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
*/

//
// MboxDocumentIterator
//
// 20 May 2005 -- tds
//

#include "indri/MboxDocumentIterator.hpp"
#include "lemur/Exception.hpp"

#define MBOX_MAX_LINE_LENGTH (1024*1024)
#define MBOX_MAX_HEADER_LINE_LENGTH (16*1024)

#define MBOX_FROM       ("From:")
#define MBOX_TO         ("To:")
#define MBOX_SUBJECT    ("Subject:")
#define MBOX_DATE       ("Date:")
#define MBOX_CC         ("Cc:")
#define MBOX_EMPTY_LINE ("")

//
// open
//
// Open a file in mbox (Unix mailbox) format
//

void indri::parse::MboxDocumentIterator::open( const std::string& filename ) {
  _in.clear();
  _in.open( filename.c_str() );
  _filename = filename;

  if( !_in.good() )
    LEMUR_THROW( LEMUR_IO_ERROR, "Couldn't open file " + filename + "." );
}

//
// _copyMetadata
//
// Copy this line into the document buffer, but also into metadata
//

void indri::parse::MboxDocumentIterator::_copyMetadata( const char* headerLine, int ignoreBytes, const char* tagName ) {
  // copy the data into the document buffer
  int lineLength = strlen( headerLine );
  char* spot = _buffer.write( lineLength - ignoreBytes ); 
  strcpy( spot, headerLine + ignoreBytes );
  
  // replace the trailing '\0' with a space
  spot[ lineLength - ignoreBytes - 1 ] = ' ';

  indri::parse::MetadataPair pair;
  // just store an offset for now, get better data later
  pair.value = (void*) (spot - _buffer.front());
  pair.valueLength = lineLength - ignoreBytes - 1;
  pair.key = tagName;

  _document.metadata.push_back( pair );
}

//
// nextDocument
//
// Fetch the next e-mail message from the mbox file as an UnparsedDocument.
// The UnparsedDocument is owned by the MboxDocumentExtractor, and should
// be assumed to be valid only until the next call to any MboxDocumentExtractor 
// method.
//

indri::parse::UnparsedDocument* indri::parse::MboxDocumentIterator::nextDocument() {
  _buffer.clear();
  _document.text = 0;
  _document.textLength = 0;
  _document.metadata.clear();

  if( _in.eof() )
    return 0;

  char headerLine[ MBOX_MAX_HEADER_LINE_LENGTH ];

  // skim past all unnecessary headers
  // want to catch:
  //    recipient
  //    author
  //    subject    
  //    date
  // all of these need to be metadata and indexed content

  while( !_in.eof() ) {
    int lineLength = MBOX_MAX_HEADER_LINE_LENGTH;
    _in.getline( headerLine, lineLength );

    if( !strncmp( headerLine, MBOX_FROM, sizeof MBOX_FROM - 1 ) ) {
      _copyMetadata( headerLine, sizeof MBOX_FROM - 1, "author" );
    } else if( !strncmp( headerLine, MBOX_TO, sizeof MBOX_TO - 1 ) ) {
      _copyMetadata( headerLine, sizeof MBOX_TO - 1, "recipient" );
    } else if( !strncmp( headerLine, MBOX_CC, sizeof MBOX_CC - 1 ) ) {
      _copyMetadata( headerLine, sizeof MBOX_CC - 1, "copied" );
    } else if( !strncmp( headerLine, MBOX_SUBJECT, sizeof MBOX_SUBJECT - 1 ) ) {
      _copyMetadata( headerLine, sizeof MBOX_SUBJECT - 1, "subject" );
    } else if( !strncmp( headerLine, MBOX_DATE, sizeof MBOX_DATE - 1 ) ) {
      _copyMetadata( headerLine, sizeof MBOX_DATE - 1, "date" );
    } else if( !strcmp( headerLine, MBOX_EMPTY_LINE ) ) {
      break;
    }
  }

  //std::cout << "found empty line, moving on" << std::endl;

  // now, we're catching message text
  // we will stop (and throw out content) as soon as we see a "From" line
  while( !_in.eof() ) {
    int readChunk = 1024*1024;
    char* textSpot = _buffer.write(readChunk);
    _in.getline( textSpot, readChunk );
    _buffer.unwrite( readChunk - _in.gcount() );

    // done reading at a "From" line
    if( !strncmp( textSpot, "From", 4 ) ) {
      //std::cout << "found next message (from line)" << std::endl;
      _buffer.unwrite( _in.gcount() );
      break;
    }
  }

  // terminate string
  *_buffer.write(1) = 0;

  // fix up existing metadata
  for( int i=0; i<_document.metadata.size(); i++ ) {
    size_t offset = (size_t) _document.metadata[i].value;
    _document.metadata[i].value = _buffer.front() + offset;
  }

  // add type metadata
  indri::parse::MetadataPair pair;
  pair.key = "filetype";
  pair.value = (void*) "MBOX";
  pair.valueLength = sizeof "MBOX";
  _document.metadata.push_back( pair );

  // copy subject into docno
  for( int i=0; i<_document.metadata.size(); i++ ) {
    if( !strcmp( "subject", _document.metadata[i].key ) ) {
      pair.key = "docno";
      pair.value = _document.metadata[i].value;
      pair.valueLength = _document.metadata[i].valueLength;
      _document.metadata.push_back( pair );
      break;
    }
  }

  _document.text = _buffer.front();
  _document.textLength = _buffer.position();

  return &_document;
}

//
// close
//
// Close the mbox file
//

void indri::parse::MboxDocumentIterator::close() {
  _in.close();
}
