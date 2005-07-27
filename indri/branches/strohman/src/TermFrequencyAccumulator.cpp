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

#include "indri/WeightedAndNode.hpp"
#include <algorithm>
#include "lemur/lemur-platform.h"
#include <iostream>
#include "indri/Annotator.hpp"
#include "indri/TermFrequencyBeliefNode.hpp"
#include "indri/CachedFrequencyBeliefNode.hpp"
#include "indri/greedy_vector"
#include "indri/delete_range.hpp"
#include "indri/Parameters.hpp"

void indri::infnet::TermFrequencyAccumulator::_findPotentialMatches( indri::utility::greedy_vector< std::pair< int, int > >& matches, const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all ) {
  int lastDocument = 0;
  int matchCount = 0;
  
  for( int i=0; i<all.size(); i++ ) {
    if( lastDocument == all[i].document ) {
      matchCount++;
      if( matchCount == totalNodes )
        matches.push_back( std::make_pair( i, lastDocument ) );
    } else {
      lastDocument = all[i].document;
      matchCount = 1;
      if( matchCount == totalNodes )
        matches.push_back( std::make_pair( i, lastDocument ) );
    }
  }
}

void indri::infnet::TermFrequencyAccumulator::_buildSingleTermMatches( indri::utility::greedy_vector< std::pair< int, int > >& matches, const indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument>& all, TermScoreFunction& function, int k ) {
  // argh; have to consider dirichlet here too, so have to run down the list until it
  // isn't possible that a lower document might have a higher dirichlet score

  // need at least k documents for this to work
  if( all.size() < k )
    return;
  
  // pick out the k^th document by frequency:
  double frequency = all[k-1].count / all[k-1].length;
  
  // suppose this document was the longest document in the
  // collection -- what frequency would it need in order to
  // surpass this one in score function order?
  double bigDocumentFrequency = 0; // not sure how to do this--maybe change score function API?
  
  for( int i=0; i<all.size(); i++ ) {
    matches.push_back( std::make_pair( i, all[i].document ) );
    
    frequency = all[i].count / all[i].length;
    
    if( frequency < bigDocumentFrequency )
      break; 
  }
}

void indri::infnet::TermFrequencyAccumulator::precomputeQuery( ) {
  // precondition: if we're in this accumulator, some preprocessing
  // step has told us that all our children are termfrequency nodes or stopwords
  
  // we're going to add all the documents to a single vector called 'all'.  
  indri::utility::greedy_vector<indri::index::DocListIterator::TopDocument> all;
  
  int totalNodes = 0;
  
    // get all the relevant topdocs lists
  for( unsigned int i=0; i<_children.size(); i++ ) {
    indri::infnet::TermFrequencyBeliefNode* node = dynamic_cast<indri::infnet::TermFrequencyBeliefNode*>(_children[i].node);

    if( node ) {
      totalNodes++;
    
      all.append( node->topdocs().begin(), node->topdocs().end() );
    
      _children[i].maximumWeightedScore = node->maximumScore() * _children[i].weight;
      _children[i].backgroundWeightedScore = node->maximumBackgroundScore() * _children[i].weight;
    }    
  }

  // should have a special, ultra-fast pipeline for single term queries;
  // maybe just by prepopulating the matches array with the first k docs of 'all'
  // and not sorting 'all'.
    
  // sort all extents by documentID, while maintaining node order
  std::stable_sort( all.begin(), all.end(), indri::index::DocListIterator::TopDocument::docid_less() );
    
  // run down the 'all' list, looking for potential matches
  // a match is a document that contains all the query terms
  indri::utility::greedy_vector< std::pair< int, int > > matches;
  _findPotentialMatches( matches, all );
  
  
  
  
  // run down the frequency lists, scoring as we go
  // might want to vary the number of candidates we consider based on number of terms, etc.
  
  // need an accumulator table that can handle frequency sorted scoring





}

bool indri::infnet::TermFrequencyAccumulator::isComplex() {
  // return true if we need to actually score the query, 
  // return false otherwise
}


double indri::infnet::WeightedAndNode::_computeMaxScore( unsigned int start ) {
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

void indri::infnet::WeightedAndNode::_computeQuorum() {
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

void indri::infnet::WeightedAndNode::addChild( double weight, BeliefNode* node ) {
  child_type child;

  child.node = node;
  child.weight = weight;
  child.backgroundWeightedScore = node->maximumBackgroundScore() * weight;
  child.maximumWeightedScore = node->maximumScore() * weight;

  _children.push_back( child );
  std::sort( _children.begin(), _children.end(), child_type::maxscore_less() );
  _computeQuorum();
}

struct double_greater {
  bool operator() ( double one, double two ) const {
    return one > two;
  }
};

void indri::infnet::WeightedAndNode::doneAddingChildren() {
  // should be removed
}

void indri::infnet::WeightedAndNode::indexChanged( indri::index::Index& index ) {
  _candidates.clear();
  _candidatesIndex = 0;

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

void indri::infnet::WeightedAndNode::setThreshold( double threshold ) {
  _threshold = threshold;

  if( _threshold >= _recomputeThreshold ) {
   // std::cout << "threshold hit recompute " << _recomputeThreshold << std::endl;
    _computeQuorum();
  }
}
  
int indri::infnet::WeightedAndNode::nextCandidateDocument() {
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

void indri::infnet::WeightedAndNode::annotate( indri::infnet::Annotator& annotator, int documentID, int begin, int end ) {
  std::vector<child_type>::iterator iter;
  annotator.add( this, documentID, begin, end );

  for( iter = _children.begin(); iter != _children.end(); iter++ ) {
    (*iter).node->annotate( annotator, documentID, begin, end );
  }
}

double indri::infnet::WeightedAndNode::maximumBackgroundScore() {
  std::vector<child_type>::iterator iter;
  double minimum = 0.0;

  for( iter = _children.begin(); iter != _children.end(); iter++ ) {
    minimum += (*iter).weight * (*iter).node->maximumBackgroundScore();
  }

  return minimum;
}

double indri::infnet::WeightedAndNode::maximumScore() {
  std::vector<child_type>::iterator iter;
  double maximum = 0.0;

  for( iter = _children.begin(); iter != _children.end(); iter++ ) {
    maximum += (*iter).weight * (*iter).node->maximumScore();
  }

  return maximum;
}

indri::utility::greedy_vector<indri::api::ScoredExtentResult>& indri::infnet::WeightedAndNode::score( int documentID, int begin, int end, int documentLength ) {
  std::vector<child_type>::iterator iter;
  double score = 0;

  for( iter = _children.begin(); iter != _children.end(); iter++ ) {
    const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& childResults = (*iter).node->score( documentID, begin, end, documentLength );

    double childScore = 0;
    for( unsigned int j=0; j<childResults.size(); j++ ) {
      childScore += (*iter).weight * childResults[j].score;
    }

    score += childScore;
  }

  _scores.clear();
  _scores.push_back( indri::api::ScoredExtentResult(score, documentID, begin, end) );

  // advance candidates
  while( _candidatesIndex < _candidates.size() && _candidates[_candidatesIndex] <= documentID )
    _candidatesIndex++;

  return _scores;
}

//
// hasMatch
//

bool indri::infnet::WeightedAndNode::hasMatch( int documentID ) {
  // advance candidates
  while( _candidatesIndex < _candidates.size() && _candidates[_candidatesIndex] <= documentID )
    _candidatesIndex++;

  for( unsigned int i=0; i<_children.size(); i++ ) {
    if( _children[i].node->hasMatch( documentID ) )
      return true;
  }

  return false;
}

//
// hasMatch
//

const indri::utility::greedy_vector<bool>& indri::infnet::WeightedAndNode::hasMatch( int documentID, const indri::utility::greedy_vector<indri::index::Extent>& extents ) {
  // advance candidates
  while( _candidatesIndex < _candidates.size() && _candidates[_candidatesIndex] <= documentID )
    _candidatesIndex++;

  _matches.clear();
  _matches.resize( extents.size(), false );

  for( unsigned int i=0; i<_children.size(); i++ ) {
    const indri::utility::greedy_vector<bool>& kidMatches = _children[i].node->hasMatch( documentID, extents );

    for( unsigned int j=0; j<kidMatches.size(); j++ ) {
      if( kidMatches[j] ) {
        _matches[j] = true;
      }
    }
  }

  return _matches;
}

//
// getName
//

const std::string& indri::infnet::WeightedAndNode::getName() const {
  return _name;
}



