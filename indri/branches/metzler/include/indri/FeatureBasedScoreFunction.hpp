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
// 22 July 2005 -- dam
//

#ifndef INDRI_FEATUREBASEDSCOREFUNCTION_HPP
#define INDRI_FEATUREBASEDSCOREFUNCTION_HPP

#include "indri/TermScoreFunction.hpp"
#include <math.h>
namespace indri
{
  namespace query
  {
    
    class FeatureBasedScoreFunction : public TermScoreFunction {
    private:
      double _weights[ 6 ];
      double _features[ 6 ];

      double _collectionOccurrences;
      double _collectionContextSize;
      int _documentOccurrences;
      int _documentCount;
	
      double* _computeFeatures( double occurrences, int contextSize ) {
	// log( c(q_i,D) ) -- Nallapati feature 1
	_features[ 0 ] = log( occurrences );
	
	// log( 1 + c(q_i,D) / |D| ) -- Nallapti feature 2
	_features[ 1 ] = log( 1.0 + 1.0 * occurrences / contextSize );
	
	// log( idf(q_i) ) -- Nallapati feature 3
	_features[ 2 ] = log( 1.0 * _documentCount / _documentOccurrences );
	
	// log( |C| / c(q_i,C) / ) -- Nallapti feature 4
	_features[ 3 ] = log( 1.0 * _collectionContextSize / _collectionOccurrences );
	
	// log( 1 + ( c(q_i,D) / |D| ) * idf(q_i) ) -- Nallapti feature 5
	_features[ 4 ] = log( 1.0 + ( 1.0 * occurrences / contextSize ) * ( 1.0 * _documentCount / _documentOccurrences ) );
	
	// log( 1 + ( c(q_i,D) / |D| ) * ( |C| / c(q_i,C) ) ) -- Nallapti feature 6
	_features[ 5 ] = log( 1.0 + ( 1.0 * occurrences / contextSize ) * ( 1.0 * _collectionContextSize / _collectionOccurrences ) );
      }
      
    public:
      FeatureBasedScoreFunction( double* weights, double collectionOccurrences, double collectionContextSize, int documentOccurrences, int documentCount ) : _collectionOccurrences(collectionOccurrences), _collectionContextSize(collectionContextSize), _documentOccurrences(documentOccurrences), _documentCount(documentCount) {
	memcpy( _weights, weights, 6 * sizeof( double ) );
      }

      double scoreOccurrence( double occurrences, int contextSize ) {
	if( !occurrences )
	  return 0.0;

	_computeFeatures( occurrences, contextSize );

	double score = 0.0;
	for( int i = 0; i < 6; i++ )
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

#endif // INDRI_FEATUREBASEDSCOREFUNCTION_HPP

