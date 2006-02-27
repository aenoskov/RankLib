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
// F2EXPTermScoreFunction
//
// F2-EXP score function based on
// Fang, et. al.'s axiomatic approach
// to information retrieval
//
// 30 November 2005 -- dam
//

#ifndef INDRI_F2EXPTERMSCOREFUNCTION_HPP
#define INDRI_F2EXPTERMSCOREFUNCTION_HPP

#include "indri/TermScoreFunction.hpp"
#include <math.h>
namespace indri
{
  namespace query
  {
    
    class F2EXPTermScoreFunction : public TermScoreFunction {
    private:
      /// inverse document frequency (IDF) for this term
      double _inverseDocumentFrequency; 
      /// average document length in the collection
      double _averageDocumentLength;

      double _termWeight;

      // These are F2-EXP parameters
      double _s;

      // The following values are precomputed so that score computation will go faster
      double _sOverAvgDocLength;
      double _termWeightTimesIDF;

      void _precomputeConstants() {
        _sOverAvgDocLength = _s / _averageDocumentLength;
        _termWeightTimesIDF = _termWeight * _inverseDocumentFrequency;
      }

    public:
      F2EXPTermScoreFunction( double idf, double averageDocumentLength, double s = 0.5 ) {
        _inverseDocumentFrequency = idf;
        _averageDocumentLength = averageDocumentLength;

        _s = s;

        _termWeight = 1.0;
        _precomputeConstants();
      }

      double scoreOccurrence( double occurrences, int documentLength ) {
        //
        // Score function is:
        //                                                 occurrences
        // score = termWeight * IDF * ---------------------------------------------------------
        //                             occurrences + s + documentLength * ( s / avgDocLength )
        //

        double numerator = _termWeightTimesIDF * occurrences;
        double denominator = occurrences + _s + documentLength * _sOverAvgDocLength;

        return numerator / denominator;
      }

      double scoreOccurrence( double occurrences, int contextSize, double documentOccurrences, int documentLength ) {
	return scoreOccurrence( occurrences, contextSize );
      }
      
      double maximumScore( int minimumDocumentLength, int maximumOccurrences ) {
        return scoreOccurrence( maximumOccurrences, minimumDocumentLength );
      }
    };
  }
}

#endif // INDRI_F2EXPTERMSCOREFUNCTION_HPP

