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

#ifndef INDRI_PAGERANK_HPP
#define INDRI_PAGERANK_HPP

#include <string>
#include <vector>
#include <map>

#include "indri/UnparsedDocument.hpp"
#include "indri/Parameters.hpp"
#include "indri/FileTreeIterator.hpp"
#include "indri/TaggedDocumentIterator.hpp"
#include "indri/TaggedTextParser.hpp"
#include "indri/Path.hpp"

namespace indri
{
  namespace parse
  {
    
    class PageRank {
    private:
      indri::api::Parameters _parameters;
      const std::string _corpusPath;
      std::string _linkPath;

      double _c; // dampening parameter
      UINT64 _colLen; // collection length

      typedef std::map< std::string, float > PageRankVector;
      typedef std::map< std::string, std::pair< int, std::vector< std::string > > > Links;

      inline void _swap( std::string& a, std::string& b ) {
	std::string tmp = b;
	b = a;
	a = tmp;
      }

      float _readPageRankFromFile( std::ifstream& src, const std::string& sourceDoc );
      void _writePageRankToFile( std::ofstream& src, const std::string& destDoc, const float pr );

      void _computeOutDegrees( Links& links );
      void _doPageRankIter( const int docsPerIter, const std::string& srcFile, const std::string& destFile );
      void _updatePageRank( std::ifstream& src, std::ofstream& dest, Links& links );

    public:
      PageRank( const std::string& corpusPath, const std::string& linkPath ) : _corpusPath( corpusPath ) {
	_parameters.loadFile( indri::file::Path::combine( linkPath, "manifest" ) );
	_linkPath = indri::file::Path::combine( linkPath, "sorted" );
	_colLen = (INT64)_parameters[ "corpus.total-documents" ];
      }
      
      void computePageRank( const std::string& outputFile, const int maxIters = 10, const int docsPerIter = 1000, const double c = 0.7 );
      static void printSavedPageRanks( const std::string& dest );
    };
  }
}

#endif
