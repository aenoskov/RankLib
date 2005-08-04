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
// TermFrequencyAccumulator.cpp
//
// Processes simple title queries (#combine or #weight
// around only simple terms or stopwords)  The integrated
// approach is a bit more efficient, and makes it possible
// to cut out query processing in the event that topdocs
// have all the necessary information.
//

#include "indri/TermFrequencyAccumulator.hpp"
#include <algorithm>
#include "lemur/lemur-platform.h"
#include <iostream>
#include "indri/Annotator.hpp"
#include "indri/TermFrequencyBeliefNode.hpp"
#include "indri/CachedFrequencyBeliefNode.hpp"
#include "indri/greedy_vector"
#include "indri/delete_range.hpp"
#include "indri/Parameters.hpp"
#include <math.h>


//
// _buildSingleTermMatches
//

bool indri::infnet::TermFrequencyAccumulator::_scoreSingleTerm( indri::utility::greedy_vector< int >& matches,
                                                                       const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all,
                                                                       indri::query::TermScoreFunction& function,
                                                                       int maximumDocumentLength,
                                                                       int k )
{
  // argh; have to consider dirichlet here too, so have to run down the list until it
  // isn't possible that a lower document might have a higher dirichlet score

  // need at least k documents for this to work
  if( all.size() < k )
    return false;
  
  // suppose this document was the longest document in the
  // collection -- what frequency would it need in order to
  // surpass this one in score function order?
  double bigDocumentFrequency = 1;
  
  for( int i=all.size()-1; i>=0; i-- ) {
    matches.push_back( i );
  
    double score = function.scoreOccurrence( all[i].count, all[i].length );
    
    if( score >= _scores.top().score || _scores.size() < k ) {
      _scores.push( indri::api::ScoredExtentResult( score, all[i].document, 0, all[i].length ) ); 
    
      while( _scores.size() > k ) {
        _scores.pop();
        bigDocumentFrequency = function.equivalentFraction( _scores.top().score, maximumDocumentLength );      
      }
    }
    
    double frequency = double(all[i].count) / double(all[i].length);

    if( frequency < bigDocumentFrequency && bigDocumentFrequency != 1.0 )
      return true; 
  }
  
  return false;
}

//
// _buildCandidatesList
//

void indri::infnet::TermFrequencyAccumulator::_buildCandidatesList( const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all ) {
  int lastDocument = 0;

  for( int i=0; i<all.size(); i++ ) {
    if( all[i].document != lastDocument ) {
      _candidates.push_back( i );
    }

    lastDocument = all[i].document;
  }
}

//
// _findPotentialMatches
//

void indri::infnet::TermFrequencyAccumulator::_findPotentialMatches( indri::utility::greedy_vector< int >& matches, 
                                                                     const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all,
                                                                     int nodeCount )
{
  int lastDocument = 0;
  int sameDocCount = 0;

  for( int i=0; i<all.size(); i++ ) {
    if( lastDocument == all[i].document ) {
      sameDocCount++;
    } else {
      lastDocument = all[i].document;
      sameDocCount = 1;
    }

    if( sameDocCount == nodeCount ) {
      matches.push_back( i-nodeCount+1 );
    }
  }
}

//
// _scorePotentialMatches
//

void indri::infnet::TermFrequencyAccumulator::_scorePotentialMatches( const indri::utility::greedy_vector< int >& matches,
                                                                      const indri::utility::greedy_vector< indri::index::DocListIterator::TopDocument>& all,
                                                                      const indri::utility::greedy_vector< indri::query::TermScoreFunction* >& functions,
                                                                      const indri::utility::greedy_vector< double >& weights )
{
  // clear scores
  while( _scores.size() )
    _scores.pop();

  for( int i=0; i<matches.size(); i++ ) {
    // score this match
    double score = 0;
    int index = matches[i];

    for( int j=0; j<functions.size(); j++ ) {
      score += weights[j] * functions[j]->scoreOccurrence( all[index+j].count, all[index+j].length );
    }
    
    _scores.push( indri::api::ScoredExtentResult( score, all[index].document, 0, all[index].length ) );
  }
}

//
// _topdocsScoresAreSufficient
//

bool indri::infnet::TermFrequencyAccumulator::_topdocsScoresAreSufficient(
                    const indri::utility::greedy_vector< indri::infnet::TermFrequencyBeliefNode* >& nodes,
                    const indri::utility::greedy_vector< double >& weights ) 
{
  double smallestDifference = MAX_INT32;

  while( _scores.size() > _resultsRequested )
    _scores.pop();

  if( _scores.size() < _resultsRequested )
    return false;

  // we want to know if there is any possible document that might have a higher score
  // than the what we're currently seeing.  Therefore, we put a bound on what any 
  // document not in _scores could possibly score.

  double bound = 0;
  double maximum = 0;
  int minimumDocumentLength = MAX_INT32;

  for( int i=0; i<nodes.size(); i++ ) {
    indri::index::TermData* termData = nodes[i]->termData();
    minimumDocumentLength = lemur_compat::min<int>( termData->minDocumentLength, minimumDocumentLength );
  }
  
  for( int i=0; i<nodes.size(); i++ ) {
    indri::query::TermScoreFunction* function = nodes[i]->scoreFunction();
    indri::index::TermData* termData = nodes[i]->termData();
    const indri::utility::greedy_vector< indri::index::DocListIterator::TopDocument >& topdocs = nodes[i]->topdocs();

    const indri::index::DocListIterator::TopDocument& maxDoc = topdocs[topdocs.size()-1];
    const indri::index::DocListIterator::TopDocument& maxUnseenDoc = topdocs[0];
    
    double maximumFractionScore = function->scoreOccurrence( maxDoc.count, maxDoc.length );
    double maximumFraction = function->equivalentFraction( maximumFractionScore, termData->maxDocumentLength );

    double maximumUnseenScore = function->scoreOccurrence( maxUnseenDoc.count, maxUnseenDoc.length );    
    double maximumUnseenFraction = function->equivalentFraction( maximumUnseenScore, termData->maxDocumentLength );

    double maxOccurrences = ceil( double(termData->maxDocumentLength) * maximumFraction );
    double maxUnseenOccurrences = ceil( double(termData->maxDocumentLength) * maximumUnseenFraction );

    double nodeMaximum = weights[i] * function->scoreOccurrence( maxOccurrences, termData->maxDocumentLength );
    double nodeMaximumUnseen = weights[i] * function->scoreOccurrence( maxUnseenOccurrences, termData->maxDocumentLength );

    double difference = nodeMaximum - nodeMaximumUnseen;
    smallestDifference = lemur_compat::min<double>( smallestDifference, difference );
    maximum += nodeMaximum;
  }

  return ( maximum - smallestDifference ) < _scores.top().score;
}

//
// _precomputeQuery
//

void indri::infnet::TermFrequencyAccumulator::_precomputeQuery( ) {
  // precondition: if we're in this accumulator, some preprocessing
  // step has told us that all our children are termfrequency nodes or stopwords
  
  // we're going to add all the documents to a single vector called 'all'.  
  indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument> all;
  indri::utility::greedy_vector<int> matches;
  indri::utility::greedy_vector< indri::query::TermScoreFunction* > functions;
  indri::utility::greedy_vector< indri::index::TermData* > termDatas;
  indri::utility::greedy_vector< double > weights;
  indri::utility::greedy_vector< indri::infnet::TermFrequencyBeliefNode* > nodes;
  int totalNodes = 0;
  _complex = true;
  
    // get all the relevant topdocs lists
  for( unsigned int i=0; i<_children.size(); i++ ) {
    indri::infnet::TermFrequencyBeliefNode* node = dynamic_cast<indri::infnet::TermFrequencyBeliefNode*>(_children[i].node);

    if( node ) {
      if( node->scoreFunction()->optimizable() == false ) {
        _complex = true;
        return;
      }
      
      totalNodes++;
    
      all.append( node->topdocs().begin(), node->topdocs().end() );
    
      _children[i].maximumWeightedScore = node->maximumScore() * _children[i].weight;
      _children[i].backgroundWeightedScore = node->maximumBackgroundScore() * _children[i].weight;
      functions.push_back( node->scoreFunction() );
      termDatas.push_back( node->termData() );
      weights.push_back( _children[i].weight );
      nodes.push_back( node );
    }    
  }

  // should have a special, ultra-fast pipeline for single term queries;
  // maybe just by prepopulating the matches array with the first k docs of 'all'
  // and not sorting 'all'.

  if( totalNodes > 1 ) {
    // sort all extents by documentID, while maintaining node order
    std::stable_sort( all.begin(), all.end(), indri::index::DocListIterator::TopDocument::docid_less() );
      
    // run down the 'all' list, looking for potential matches
    // a match is a document that contains all the query terms
    indri::utility::greedy_vector< int > matches;
    _findPotentialMatches( matches, all, totalNodes );
    
    // score all of our candidates
    _scorePotentialMatches( matches, all, functions, weights );
  
    // find out if this made a difference
    _complex = (_topdocsScoresAreSufficient( nodes, weights ) == false);
  } else {
    _complex = (_scoreSingleTerm( matches, all, *functions[0], termDatas[0]->maxDocumentLength, _resultsRequested ) == false);
  }
    
  if( !_complex )
    std::cout << "scoring entirely from topdocs" << std::endl;

  if( _complex ) {
    std::cout << "using full eval anyway" << std::endl;
    
    while( _scores.size() )
      _scores.pop();

    if( totalNodes == 1 ) {
      std::stable_sort( all.begin(), all.end(), indri::index::DocListIterator::TopDocument::docid_less() );
    }

    _buildCandidatesList( all );
  }
}

//
// isComplex
//

bool indri::infnet::TermFrequencyAccumulator::isComplex() {
  return _complex;
}


double indri::infnet::TermFrequencyAccumulator::_computeMaxScore( unsigned int start ) {
  // first, find the maximum score of the first few columns
  double maxScoreSum = 0;

  for( unsigned int i=0; i<start+1; i++ ) {
    maxScoreSum += _children[i].maximumWeightedScore;
  }

  // then add in the background score of the last columns
  double minScoreSum = 0;

  for( unsigned int i=start+1; i<_children.size(); i++ ) {
    minScoreSum += _children[i].backgroundWeightedScore;
  }

  return maxScoreSum + minScoreSum;
}

void indri::infnet::TermFrequencyAccumulator::_computeQuorum() {
  double maximumScore = -DBL_MAX;
  unsigned int i;

  // keep going until we find a necessary term
  for( i=0; i<_children.size() && maximumScore < _threshold; i++ ) {
    maximumScore = _computeMaxScore(i);
  }

  _quorumIndex = i-1;
  if( _quorumIndex < 0 )
    _quorumIndex = 0;

  if( _quorumIndex > _children.size()-1 )
    _recomputeThreshold = DBL_MAX;
  else
    _recomputeThreshold = maximumScore;
}

void indri::infnet::TermFrequencyAccumulator::addChild( double weight, BeliefNode* node ) {
  child_type child;

  child.node = node;
  child.weight = weight;
  child.backgroundWeightedScore = 0;
  child.maximumWeightedScore = 0;

  _children.push_back( child );
}

struct double_greater {
  bool operator() ( double one, double two ) const {
    return one > two;
  }
};

//
// indexChanged
//

void indri::infnet::TermFrequencyAccumulator::indexChanged( indri::index::Index& index ) {
  _candidates.clear();
  _candidatesIndex = 0;

  _precomputeQuery();
  
  if( _complex ) {
    indri::utility::greedy_vector< indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>* > lists;
  
    // get all the relevant topdocs lists
    for( unsigned int i=0; i<_children.size(); i++ ) {
      indri::infnet::TermFrequencyBeliefNode* node = dynamic_cast<indri::infnet::TermFrequencyBeliefNode*>(_children[i].node);
  
      if( node && indri::api::Parameters::instance().get( "topdocs", true ) ) {
        indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>* copy = new indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>( node->topdocs() );
        lists.push_back( copy );
        std::sort( copy->begin(), copy->end(), indri::index::DocListIterator::TopDocument::docid_less() );
  
        _children[i].maximumWeightedScore = node->maximumScore() * _children[i].weight;
        _children[i].backgroundWeightedScore = node->maximumBackgroundScore() * _children[i].weight;
      }
    }
  
    std::sort( _children.begin(), _children.end(), child_type::maxscore_less() );
  
    // TODO: could compute an initial threshold here, but that may not be necessary
    indri::utility::greedy_vector<int> indexes;
  
    for( unsigned int i=0; i<lists.size(); i++ ) {
      if( lists[i]->size() )
        indexes.push_back( 0 );
      else
        indexes.push_back( -1 );
    }
  
    while( true ) {
      // find the smallest document
      int smallestDocument = MAX_INT32;
  
      for( unsigned int i=0; i<lists.size(); i++ ) {
        indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& currentList = *lists[i];      
  
        if( indexes[i] >= 0 )
          smallestDocument = lemur_compat::min( smallestDocument, currentList[indexes[i]].document );
      }
  
      if( smallestDocument == MAX_INT32 )
        break;
  
      _candidates.push_back( smallestDocument );
  
      // increment indexes
      for( unsigned int i=0; i<lists.size(); i++ ) {
        indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& currentList = *lists[i];      
  
        if( indexes[i] >= 0 && currentList[indexes[i]].document == smallestDocument ) {
          indexes[i]++;
          
          if( indexes[i] == currentList.size() ) {
            indexes[i] = -1;
          }
        }
      }
    }
  
    for( int i=0; i<lists.size(); i++ )
      delete lists[i];
  
    // compute quorum
    _computeQuorum();
  }
}

//
// setThreshold
//

void indri::infnet::TermFrequencyAccumulator::setThreshold( double threshold ) {
  _threshold = threshold;

  if( _threshold >= _recomputeThreshold ) {
    _computeQuorum();
  }
}
  
//
// nextCandidateDocument
//

int indri::infnet::TermFrequencyAccumulator::nextCandidateDocument() {
  std::vector<child_type>::iterator iter;
  int minDocument = MAX_INT32;
  int currentCandidate;

  if( _candidatesIndex < _candidates.size() ) {
    minDocument = _candidates[_candidatesIndex];
  }

  for( iter = _children.begin() + _quorumIndex; iter != _children.end(); iter++ ) {
    currentCandidate = (*iter).node->nextCandidateDocument();

    if( currentCandidate < minDocument ) {
      minDocument = currentCandidate;
    }
  }

  return minDocument;
}

//
// evaluate
//

void indri::infnet::TermFrequencyAccumulator::evaluate( int documentID, int documentLength ) {
  std::vector<child_type>::iterator iter;
  double score = 0;

  for( iter = _children.begin(); iter != _children.end(); iter++ ) {
    const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& childResults =
          (*iter).node->score( documentID, 0, documentLength, documentLength );

    double childScore = 0;
    for( unsigned int j=0; j<childResults.size(); j++ ) {
      childScore += (*iter).weight * childResults[j].score;
    }

    score += childScore;
  }

  _scores.push( indri::api::ScoredExtentResult( score, documentID, 0, documentLength ) );

  // advance candidates
  while( _candidatesIndex < _candidates.size() && _candidates[_candidatesIndex] <= documentID )
    _candidatesIndex++;
}

//
// getName
//

const std::string& indri::infnet::TermFrequencyAccumulator::getName() const {
  return _name;
}

//
// getResults
//

const indri::infnet::EvaluatorNode::MResults& indri::infnet::TermFrequencyAccumulator::getResults() {
  _results.clear();

  if( !_scores.size() )
    return _results;

  // making a copy of the heap here so the method can be const
  std::priority_queue<indri::api::ScoredExtentResult> heapCopy = _scores;
  std::vector<indri::api::ScoredExtentResult>& scoreVec = _results["scores"];

  // puts scores into the vector in descending order
  scoreVec.reserve( heapCopy.size() );
  for( int i=(int)heapCopy.size()-1; i>=0; i-- ) {
    scoreVec.push_back( heapCopy.top() );
    heapCopy.pop();
  }

  return _results;
}



