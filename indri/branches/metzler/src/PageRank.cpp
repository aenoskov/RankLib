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
// PageRank
//
// 17 July 2005 -- dam
//

#include "indri/PageRank.hpp"

// TODO: at some point update this to use some sort of buffered reader
float indri::parse::PageRank::_readPageRankFromFile( std::ifstream& src, const std::string& sourceDoc ) {
  int lengthBuf[1];
  char docBuf[4096];
  float prBuf[1];

  if( !src.good() )
    return 1.0 / (double)_colLen;
  
  while( !src.eof() ) {
    src.read( (char *)lengthBuf, sizeof( int ) ); // get length of document name
    src.read( docBuf, lengthBuf[0] ); // get document name
    src.read( (char *)prBuf, sizeof( float ) ); // read page rank

    std::string docName = docBuf;

    if( docName > sourceDoc ) {
      int relPos = -( sizeof( int ) + lengthBuf[0] + sizeof( float ) );
      src.seekg( relPos, std::ios::cur );
      return ( 1.0 - _c ) / (double)_colLen;
    }
    
    if( docName == sourceDoc )
      return prBuf[0];
  }

  return ( 1.0 - _c ) / (double)_colLen;
}

void indri::parse::PageRank::_writePageRankToFile( std::ofstream& src, const std::string& destDoc, const float pr ) {
  int lengthBuf[1];
  float prBuf[1];
  
  lengthBuf[0] = destDoc.length();
  src.write( (char *)lengthBuf, sizeof( int ) ); // document name length
  src.write( destDoc.c_str(), lengthBuf[0] ); // document name
  
  prBuf[0] = pr;
  src.write( (char *)prBuf, sizeof( float ) ); // page rank
}

void indri::parse::PageRank::_updatePageRank( std::ifstream& src, std::ofstream& dest, Links& links ) {
  src.seekg( 0, std::ios::beg );

  PageRankVector destPR;

  // update PageRank for each destination in links
  Links::iterator iter;
  for( iter = links.begin(); iter != links.end(); ++iter ) {
    std::string sourceDoc = iter->first;
    int outDegree = iter->second.first;
    std::vector< std::string > destDocs = iter->second.second;

    float srcPR = _readPageRankFromFile( src, sourceDoc );
    std::cout << "Updating PageRanks from source " << sourceDoc << " (" << srcPR << ")" << std::endl;
        
    std::vector< std::string >::iterator destIter;
    for( destIter = destDocs.begin(); destIter != destDocs.end(); ++destIter )
      destPR[ *destIter ] += srcPR / double( outDegree );
  }

  // dampen PageRanks
  PageRankVector::iterator PRiter;
  for( PRiter = destPR.begin(); PRiter != destPR.end(); ++PRiter ) {
    PRiter->second = _c * PRiter->second + ( 1.0 - _c ) / (double)_colLen;
    _writePageRankToFile( dest, PRiter->first, PRiter->second );
  }
}

// TODO: this could be made even faster by computing this once
// by writing the results in a compact format to disk
void indri::parse::PageRank::_computeOutDegrees( Links& links ) {
  char docno[512];
  char docUrl[4096];
  char linkCountText[256];
  char text[65536];
  char linkDocno[512];

  indri::file::FileTreeIterator input( _linkPath );
  Links::iterator iter;

  for( ; input != indri::file::FileTreeIterator::end(); input++ ) {
    std::string path = *input;
    std::ifstream in;

    in.open( path.c_str() );

    // fill in the links structure for the documents currently under consideration
    while( !in.eof() ) {
      in.getline( docno, sizeof docno );
      in.getline( docUrl, sizeof docUrl );
      in.getline( linkCountText, sizeof linkCountText );
      
      if( strncmp( docno, "DOC", 3 ) != 0 )
	break;      

      int linkCount = atoi( linkCountText + sizeof "LINKS=" - 1 );

      for( int i=0; i<linkCount; i++ ) {
	in.getline( linkDocno, sizeof linkDocno );
	in.getline( text, sizeof text ); // ignore LINKFROM
	in.getline( text, sizeof text ); // ignore TEXT

	// see if this is one of our sources
	iter = links.find( linkDocno + sizeof "LINKDOCNO=" - 1 );

	// if so, update outdegree count
        if( iter != links.end() )
          iter->second.first++;
      }
    }
    
    in.close();
  }
}

void indri::parse::PageRank::_doPageRankIter( const int docsPerIter, const std::string& srcFile, const std::string& destFile ) {
  char docno[512];
  char docUrl[4096];
  char linkCountText[256];
  char linkDocno[512];
  char text[65536];

  int docsAdded = 0;
  Links links;

  std::ifstream src( srcFile.c_str(), std::ios::in | std::ios::binary );
  std::ofstream dest( destFile.c_str(), std::ios::out | std::ios::binary );

  // we need to iterate over the corpus path directories to ensure that we
  // keep things in the correct sorted order
  indri::file::FileTreeIterator input( _corpusPath );

  for( ; input != indri::file::FileTreeIterator::end(); input++ ) {
    std::string filePath = *input;
    std::ifstream in;

    std::string relative = indri::file::Path::relative( _corpusPath, filePath );
    std::string linkFile = indri::file::Path::combine( _linkPath, relative );

    in.open( linkFile.c_str() );
     
    // grab the doc ids of the source documents we're interested in
    while( !in.eof() ) {
      in.getline( docno, sizeof docno );
      in.getline( docUrl, sizeof docUrl );
      in.getline( linkCountText, sizeof linkCountText );
      
      if( strncmp( docno, "DOC", 3 ) != 0 )
	break;      

      std::cout << "Adding document " << ( docno + sizeof "DOCNO=" - 1 ) << " to destinations..." << std::endl;
      docsAdded++;

      int linkCount = atoi( linkCountText + sizeof "LINKS=" - 1 );

      // skip this stuff
      for( int i = 0; i < linkCount; i++ ) {
	in.getline( linkDocno, sizeof linkDocno );
	in.getline( text, sizeof text ); // ignore LINKFROM
	in.getline( text, sizeof text ); // ignore TEXT

	links[ linkDocno + sizeof "LINKDOCNO=" - 1 ].first = 0;
	links[ linkDocno + sizeof "LINKDOCNO=" - 1 ].second.push_back( docno + sizeof "DOCNO=" - 1 );
      }

      if( docsAdded == docsPerIter ) {
	std::cout << "Computing source outdegrees..." << std::endl;
	_computeOutDegrees( links );
	std::cout << "Total destinations: " << docsAdded << std::endl;
	std::cout << "Total sources: " << links.size() << std::endl;
	_updatePageRank( src, dest, links );
	links.clear();
	docsAdded = 0;
      }      
    }
    
    in.close();
  }
  
  // do the remainder of the documents
  if( docsAdded > 0 ) {
    std::cout << "Computing source outdegrees..." << std::endl;
    _computeOutDegrees( links );
    std::cout << "Total destinations: " << docsAdded << std::endl;
    std::cout << "Total sources: " << links.size() << std::endl;
    _updatePageRank( src, dest, links );
  }

  src.close();
  dest.close();
}

void indri::parse::PageRank::printSavedPageRanks( const std::string& dest ) {
  std::ifstream in( dest.c_str(), std::ios::in | std::ios::binary );

  int lengthBuf[1];
  char docBuf[4096];
  float prBuf[1];

  std::cout.setf( std::ios_base::fixed );
  std::cout.precision( 32 );

  while( !in.eof() ) {
    in.read( (char *)lengthBuf, sizeof( int ) ); // get length of document name
    in.read( docBuf, lengthBuf[0] ); // get document name
    docBuf[lengthBuf[0]] = 0;
    in.read( (char *)prBuf, sizeof( float ) ); // read page rank

    std::cout << docBuf << " " << prBuf[0] << std::endl;
  }

  in.close();
}

void indri::parse::PageRank::computePageRank( const std::string& outputFile, const int maxIters, const int docsPerIter, const double c ) {
  static const std::string _tmpFile = "PageRank.tmp";
  _c = c;
  
  std::string src = _tmpFile;
  std::string dest = outputFile;

  // ensure outputFile is always the final destination 
  if( maxIters % 2 == 1 )
    _swap( src, dest );
  
  // TODO: implement early stopping ability based on residual
  for( int iter = 0; iter < maxIters; iter++ ) {
    _swap( src, dest ); // alternate between source and destination files
    _doPageRankIter( docsPerIter, src, dest );
  }

  // delete the temp file
  ::remove( _tmpFile.c_str() );
}
