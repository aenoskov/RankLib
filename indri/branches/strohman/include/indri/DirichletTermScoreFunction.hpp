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

#ifndef INDRI_DIRICHLETTERMSCOREFUNCTION_HPP
#define INDRI_DIRICHLETTERMSCOREFUNCTION_HPP

#include <math.h>
namespace indri
{
  /*! Query processing, smoothing, and scoring classes. */
  namespace query
  {
    
    class DirichletTermScoreFunction : public TermScoreFunction {
    private:
      double _mu;
      double _collectionFrequency;
      double _muTimesCollectionFrequency;

    public:
      DirichletTermScoreFunction( double mu, double collectionFrequency ) {
        _collectionFrequency = collectionFrequency;
        _mu = mu;
        _muTimesCollectionFrequency = _mu * _collectionFrequency;
      }

      double scoreOccurrence( double occurrences, int contextSize ) {
        double seen = ( double(occurrences) + _muTimesCollectionFrequency ) / ( double(contextSize) + _mu );
        return log( seen );
      }

      double scoreOccurrence( double occurrences, int contextSize, double documentOccurrences, int documentLength ) {
        // can't two-level smooth with dirichlet
        return scoreOccurrence( occurrences, contextSize );
      }

      // what is the fraction (count/length) necessary to achieve a score of <score>
      // in a document of length maximumDocumentLength?
      double equivalentFraction( double score, int maximumDocumentLength ) {
        // Mark says Dirichlet is equivalent to linear smoothing with a lambda equal to
        // [1 - |D|/(|D|+mu)].
        // Let the foreground be f, and background be g.  We want to solve for f' in:
        //    ( 1 - \lambda' ) f' + \lambda' g = ( 1 - \lambda ) f + \lambda g
        // Solving, we get:
        //    f' = (( 1 - \lambda ) f + \lambda g + \lambda' g) / ( 1 - \lambda' )
        // Equivalently:
        //    f' = (exp(score) + \lambda' g) / ( 1 - \lambda' )

        double lambdaPrime = 1.0 - double(maximumDocumentLength) / ( double(maximumDocumentLength) + _mu );
        return exp(score) + lambdaPrime * _collectionFrequency / ( 1.0 - lambdaPrime );
      }
    };
  }
}

#endif // INDRI_DIRICHLETTERMSCOREFUNCTION_HPP
