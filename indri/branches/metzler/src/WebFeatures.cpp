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
// WebFeatures
//
// 24 July 2005 -- dam
//

#include "indri/WebFeatures.hpp"

int indri::parse::WebFeatures::_computeURLDepth( const std::string& url ) {
  int depth = 0;
  int pos = url.find( '/', 0 );
  while( pos != std::string::npos ) {
    if( pos > 0 && url[ pos-1 ] != '/' )
      depth++;
    pos = url.find( '/', pos+1 );
  }
  return depth;
}

void indri::parse::WebFeatures::_nextLinkDoc() {
  char docno[512];
  char docUrl[4096];
  char linkCountText[256];
  char text[65536];

  if( !_linkFile.good() || _linkFile.eof() ) {
    _curLinkDocno = "";
    _curLinkCount = 0;
    return;
  }    
  
  _linkFile.getline( docno, sizeof( docno ) );
  _curLinkDocno = docno + sizeof "DOCNO=" - 1;

  _linkFile.getline( docUrl, sizeof docUrl );
  _linkFile.getline( linkCountText, sizeof linkCountText );
  
  _curLinkCount = atoi( linkCountText + sizeof "LINKS=" - 1 );

  for( int i = 0; i < _curLinkCount; i++ ) {
    _linkFile.getline( text, sizeof( text ) ); // LINKDOCNO=
    _linkFile.getline( text, sizeof( text ) ); // LINKFROM=
    _linkFile.getline( text, sizeof( text ) ); // TEXT=
  }
}

int indri::parse::WebFeatures::_getInlinkCount( const std::string& docno ) {
  while( true ) {
    if( _curLinkDocno == "" )
      return 0;
    else if( _curLinkDocno != docno )
      return 0;
    else { // _curLinkDocno == docno
      int count = _curLinkCount;
      _nextLinkDoc();
      return count;
    }
  }  
}

void indri::parse::WebFeatures::computeFeatures() {
  indri::file::FileTreeIterator input( _corpusPath );

  // iterate over collection, extracting features for every document
  for( ; input != indri::file::FileTreeIterator::end(); input++ ) {
    std::string filePath = *input;

    std::cerr << "Extracting features for file: " << filePath << std::endl;
    
    indri::parse::TaggedDocumentIterator iterator;
    iterator.open( filePath );
    iterator.setTags( 
		     "<DOC>",             // startDocTag
		     "</DOC>\n",          // endDocTag
		     "</DOCHDR>"          // endMetadataTag
		     );
    
    indri::parse::UnparsedDocument* unparsed;

    std::string relative = indri::file::Path::relative( _corpusPath, filePath );
    std::string linkFile = indri::file::Path::combine( _linkPath, relative );

    _linkFile.open( linkFile.c_str(), std::ios::in );
    _nextLinkDoc();
    
    while( (unparsed = iterator.nextDocument()) != 0 ) {
      std::string docnoValue = "MISSING";
      std::string urlValue = "MISSING";
      int urlDepthValue = -1;
      std::string contentTypeValue = "MISSING";
      int contentLengthValue = -1;
      int inlinksValue = -1;
	
      indri::utility::greedy_vector<indri::parse::MetadataPair> metadata = unparsed->metadata;
      for( int i = 0; i < metadata.size(); i++ ) {
	std::string key = (char *)metadata[i].key;
	std::string value = (char *)metadata[i].value;
	if( key == "docno" ) {
	  docnoValue = value;
	}
	else if( key == "dochdr" ) {
	  transform( value.begin(), value.end(), value.begin(), tolower ); // lowercase the header
	  int pos = value.find( "\n" );
	  if( pos >= 0 )
	    urlValue = value.substr( 0, pos );
	  pos = value.find( "content-type: " );
	  if( pos >= 0 ) {
	    contentTypeValue = value.substr( pos + sizeof "content-type: " - 1, value.find( "\n", pos ) - pos - sizeof "conent-type: " );
	    pos = contentTypeValue.rfind( '\r' ); // pesky carriage return removal
	    if( pos != std::string::npos )
	      contentTypeValue.erase( pos );
	    pos = contentTypeValue.find( ";" ); // ignore charset attributes
	    if( pos != std::string::npos )
	      contentTypeValue.erase( pos );
	  }
	}
      }

      contentLengthValue = unparsed->textLength;
      urlDepthValue = _computeURLDepth( urlValue );
      inlinksValue = _getInlinkCount( docnoValue );
      
      std::cout << docnoValue << " " << urlValue << " " << urlDepthValue << " "
		<< contentTypeValue << " " << contentLengthValue << " " << inlinksValue << std::endl;

    }
    
    // close up everything
    iterator.close();

    _linkFile.close();
    _linkFile.clear();
  }  
}
