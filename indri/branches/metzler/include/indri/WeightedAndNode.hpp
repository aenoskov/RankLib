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
// WeightedAndNode
//
// 26 January 2004 - tds
//

#ifndef INDRI_WEIGHTEDANDNODE_HPP
#define INDRI_WEIGHTEDANDNODE_HPP

#include <vector>
#include "indri/BeliefNode.hpp"
#include "indri/SkippingCapableNode.hpp"
#include "indri/ScoredExtentResult.hpp"
#include <math.h>

class WeightedAndNode : public SkippingCapableNode {
private:
  struct child_type {
    struct maxscore_less {
    public:
      bool operator () ( const child_type& one, const child_type& two ) const {
        // think of these two elements as the only two elements in the
        // #wand.  What is the threshold of each ordering?  Sort by
        // the lowest threshold.

        return (one.backgroundWeightedScore + two.maximumWeightedScore) > 
               (one.maximumWeightedScore + two.backgroundWeightedScore);
      }
    };

    BeliefNode* node;
    double weight;
    double maximumWeightedScore;
    double backgroundWeightedScore;
  };

  std::vector<child_type> _children;
  greedy_vector<ScoredExtentResult> _scores;
  std::string _name;

  greedy_vector<int> _candidates;
  int _candidatesIndex;

  double _threshold;
  double _recomputeThreshold;
  int _quorumIndex;
  void _computeQuorum();
  double _computeMaxScore( unsigned int start );

public:
  WeightedAndNode( const std::string& name ) : _name(name), _threshold(-DBL_MAX), _quorumIndex(0), _recomputeThreshold(-DBL_MAX) {}

  void addChild( double weight, BeliefNode* node );
  void doneAddingChildren();

  // SkippingCapableNode
  void setThreshold( double threshold );

  // InferenceNetworkNode interface
  int nextCandidateDocument();
  double maximumScore();
  double maximumBackgroundScore();
  greedy_vector<ScoredExtentResult>& score( int documentID, int begin, int end, int documentLength );
  void annotate( class Annotator& annotator, int documentID, int begin, int end );
  bool hasMatch( int documentID );
  const std::string& getName() const;
};

#endif // INDRI_WEIGHTEDANDNODE_HPP

