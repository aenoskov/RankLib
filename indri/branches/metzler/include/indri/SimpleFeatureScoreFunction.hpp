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
// FeatureBasedScoreFunction
//
// 07 December 2005 -- dam
//

#ifndef INDRI_SIMPLEFEATURESCOREFUNCTION_HPP
#define INDRI_SIMPLEFEATURESCOREFUNCTION_HPP

#include "indri/TermScoreFunction.hpp"
#include <math.h>
namespace indri
{
  namespace query
  {
    
    class SimpleFeatureScoreFunction : public TermScoreFunction {
    private:
      double _weights[ 3 ];
      double _features[ 3 ];

      int _documentOccurrences;
      int _documentCount;
      double _avgDocLength;
	
      double* _computeFeatures( double occurrences, int contextSize ) {
	// log( c(q_i,D) )
	_features[ 0 ] = log( occurrences );
	
	// log( idf(q_i) )
	_features[ 1 ] = log( double(_documentCount) / double(_documentOccurrences) );
	
	// log( |D| / |D|_avg )
	_features[ 2 ] = -contextSize / _avgDocLength;
      }
      
    public:
      SimpleFeatureScoreFunction( double* weights, int documentOccurrences, int documentCount, double avgDocLength ) : _documentOccurrences(documentOccurrences), _documentCount(documentCount), _avgDocLength(avgDocLength) {
	memcpy( _weights, weights, 3 * sizeof( double ) );
      }

      double scoreOccurrence( double occurrences, int contextSize ) {
	if( !occurrences )
	  return _weights[ 2 ] * ( -contextSize / _avgDocLength );

	_computeFeatures( occurrences, contextSize );

	double score = 0.0;
	for( int i = 0; i < 3; i++ )
	  score += _weights[ i ] * _features[ i ];
	  
	return score;
      }

      double scoreOccurrence( double occurrences, int contextSize, double documentOccurrences, int documentLength ) {
	return scoreOccurrence( occurrences, contextSize );
      }

      // shut max score off for these nodes
      double maximumScore( int minimumDocumentLength, int maximumOccurrences ) {
	return INDRI_HUGE_SCORE;
      }
    };
  }
}

#endif // INDRI_SIMPLEFEATURESCOREFUNCTION_HPP

