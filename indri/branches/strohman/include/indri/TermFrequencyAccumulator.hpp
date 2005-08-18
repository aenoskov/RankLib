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
// TermFrequencyAccumulator.hpp
//
// Processes simple title queries (#combine or #weight
// around only simple terms or stopwords)  The integrated
// approach is a bit more efficient, and makes it possible
// to cut out query processing in the event that topdocs
// have all the necessary information.
//

#ifndef INDRI_TERMFREQUENCYACCUMULATOR_HPP
#define INDRI_TERMFREQUENCYACCUMULATOR_HPP

#include "indri/EvaluatorNode.hpp"
#include "indri/greedy_vector"
#include "indri/TermScoreFunction.hpp"
#include "indri/BeliefNode.hpp"
#include "indri/TermFrequencyBeliefNode.hpp"
#include <queue>
#include <algorithm>

namespace indri {
  namespace infnet {
    class TermFrequencyAccumulator : public EvaluatorNode {
    private:
      struct child_type {
        struct maxscore_less {
        public:
          bool operator () ( const child_type& one, const child_type& two ) const {
            // think of these two elements as the only two elements in the
            // #wand.  What is the threshold of each ordering?  Sort by
            // the lowest threshold.

            return (one.backgroundWeightedScore) > 
                   (two.backgroundWeightedScore);
          }
        };

        BeliefNode* node;
        double weight;
        double maximumWeightedScore;
        double backgroundWeightedScore;
      };

      void _findPotentialMatches( indri::utility::greedy_vector< int >& matches,
                                  indri::utility::greedy_vector< int >& leftOut,
                                  const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all,
                                  int nodeCount );
      bool _scoreSingleTerm( indri::utility::greedy_vector< int >& matches,
                                    const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all,
                                    indri::query::TermScoreFunction& function,
                                    int maximumDocumentLength,
                                    int k );
      void _scorePotentialMatches( const indri::utility::greedy_vector< int >& matches,
                                   const indri::utility::greedy_vector< indri::index::DocListIterator::TopDocument >& all,
                                   const indri::utility::greedy_vector< indri::query::TermScoreFunction* >& functions,
                                   const indri::utility::greedy_vector< double >& weights );
      void _buildCandidatesList( const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all );
      bool _topdocsScoresAreSufficient( const indri::utility::greedy_vector< indri::infnet::TermFrequencyBeliefNode* >& nodes,
                                        const indri::utility::greedy_vector< double >& weights, 
                                        const indri::utility::greedy_vector< int >& leftOut );

      void _precomputeQuery();

      double _computeMaxScore( unsigned int start );
      void _computeQuorum();

      std::priority_queue<indri::api::ScoredExtentResult> _scores;
      EvaluatorNode::MResults _results;
      std::vector<child_type> _children;
      std::vector<int> _candidates;
      bool _complex;
      int _resultsRequested;
      int _candidatesIndex;
      int _quorumIndex;
      double _recomputeThreshold;
      double _threshold;
      std::string _name;
      
    public:
      TermFrequencyAccumulator( const std::string& name, int resultsRequested ) :
        _name(name),
        _resultsRequested(resultsRequested)
      {
      }

      bool isComplex();
    
      void addChild( double weight, indri::infnet::BeliefNode* node );
      void indexChanged( indri::index::Index& index );
      void setThreshold( double threshold );
      int nextCandidateDocument();
      void evaluate( int documentID, int documentLength );
      const std::string& getName() const;
      const EvaluatorNode::MResults& getResults();
    };
  }
}

#endif // INDRI_TERMFREQUENCYACCUMULATOR_HPP





//

