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
// DependenceModelQueryFormulator
//
// 27 February 2005 -- dam
//

#include "indri/DependenceModelQueryFormulator.hpp"

indri::query::DependenceModelQueryFormulator::DependenceModelQueryFormulator( double termWt,
									      double orderedWt,
									      double unorderedWt,
									      const std::string& type ) :
  _termWt( termWt ),
  _orderedWt( orderedWt ),
  _unorderedWt( unorderedWt ),
  _type( type ) { }

std::string indri::query::DependenceModelQueryFormulator::formulate( const std::string& query ) {
  // simplest case -- full independence model
  if( _type == "fi" ) {
    std::stringstream ret;
    ret << "#combine( " << query << " )";
    return ret.str();
  }
  
  std::stringstream termQuery;
  std::stringstream orderedQuery;
  std::stringstream unorderedQuery;
  
  std::vector<std::string> terms = _extractTerms( query );

  termQuery << "#combine( ";
  orderedQuery << "#combine(";
  unorderedQuery << "#combine(";
    
  // add term features
  for( int i = 0; i < terms.size(); i++ )
    termQuery << terms[ i ] << " ";
  
  int combinations = (int)pow( 2.0, terms.size() );
  int start = 1;
  if( _type == "sd" )
    start = 3; // 00000011 bit pattern
  for( int i = start; i < combinations; i++ ) {

    std::stringstream extractedTerms;
    bool contiguous = true;
    int numExtracted = 0;
    int lastAdded = -1;
    for( int j = 0; j < terms.size(); j++ ) {
      if( i & ( 1 << j ) ) {
	if( lastAdded != -1 && ( j - lastAdded > 1 ) )
	  contiguous = false;
	extractedTerms << terms[j] << " ";
	numExtracted++;
	lastAdded = j;
      }
    }

    if( numExtracted == 1 ) // skip these, since we already took care of these
      continue;

    // add ordered window feature for contiguous subsets of terms
    if( contiguous )
      orderedQuery << " #1( " << extractedTerms.str() << ")";

    // add unordered window feature for all subsets of terms
    unorderedQuery << " #" << 4*numExtracted << "( " << extractedTerms.str() << ")";
    
    if( _type == "sd" ) {
      i *= 2;;
      i--;
    }
  }

  // construct query
  std::stringstream ret;
  ret << "#weight(";
  if( _termWt != 0.0 && termQuery.str() != "#combine( " )
    ret << " " << _termWt << " " << termQuery.str() << ")";
  if( _orderedWt != 0.0 && orderedQuery.str() != "#combine(" )
    ret << " " << _orderedWt << " " << orderedQuery.str() << " )";
  if( _unorderedWt != 0.0 && unorderedQuery.str() != "#combine(" )
    ret << " " << _unorderedWt << " " << unorderedQuery.str() << " )";
  ret << " )";

  return ret.str();
}

std::vector<std::string> indri::query::DependenceModelQueryFormulator::_extractTerms( const std::string query ) {
  std::vector<std::string> terms;
  int startPos = -1;
  int endPos = -1;
  for( int i = 0; i < query.length(); i++ ) {
    if( isspace( query[i] ) ) {
      if( endPos > startPos )
	terms.push_back( query.substr( startPos+1, endPos - startPos ) );
      startPos = i;
    }
    else
      endPos = i;
  }
  if( endPos > startPos )
    terms.push_back( query.substr( startPos+1, endPos - startPos ) );
  
  return terms;
}
