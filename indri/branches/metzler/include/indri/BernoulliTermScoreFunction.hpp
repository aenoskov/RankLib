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
// DirichletTermScoreFunction
//
// 26 January 2004 - tds
//

#ifndef INDRI_BERNOULLITERMSCOREFUNCTION_HPP
#define INDRI_BERNOULLITERMSCOREFUNCTION_HPP

#include <math.h>
namespace indri
{
  /*! Query processing, smoothing, and scoring classes. */
  namespace query
  {
    
    class BernoulliTermScoreFunction : public TermScoreFunction {
    private:
      double _mu;
      double _collectionFrequency;
      double _muTimesCollectionFrequency;

    public:
      BernoulliTermScoreFunction( double mu, double collectionFrequency ) {
        _collectionFrequency = collectionFrequency;
        _mu = mu;
        _muTimesCollectionFrequency = _mu * _collectionFrequency;
      }

      double scoreOccurrence( double occurrences, int contextSize ) {
	if( occurrences > 0 )
	  occurrences = 1.0;
        double seen = ( double(occurrences) + _muTimesCollectionFrequency ) / ( 1.0 + _mu );
        return log( seen );
      }

      double scoreOccurrence( double occurrences, int contextSize, double documentOccurrences, int documentLength ) {
        // can't two-level smooth with dirichlet
        return scoreOccurrence( occurrences, contextSize );
      }
    };
  }
}

#endif // INDRI_DIRICHLETTERMSCOREFUNCTION_HPP
