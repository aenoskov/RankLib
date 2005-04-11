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
// FixedPassageNode
//
// 23 February 2005 -- tds
//

#include "indri/FixedPassageNode.hpp"
#include "indri/Annotator.hpp"
#include "lemur/lemur-compat.hpp"

indri::infnet::FixedPassageNode::FixedPassageNode( const std::string& name, indri::infnet::BeliefNode* child, int windowSize, int increment ) :
  _name(name),
  _child(child),
  _windowSize(windowSize),
  _increment(increment)
{
}

int indri::infnet::FixedPassageNode::nextCandidateDocument() {
  return _child->nextCandidateDocument();
}

double indri::infnet::FixedPassageNode::maximumBackgroundScore() {
  return INDRI_TINY_SCORE;
}

double indri::infnet::FixedPassageNode::maximumScore() {
  return INDRI_HUGE_SCORE;
}

const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& indri::infnet::FixedPassageNode::score( int documentID, int begin, int end, int documentLength ) {
  // we're going to run through the field list, etc.
  _scores.clear();

  // round down to find where the passage starts
  int beginPassage = (begin / _increment) * _increment;

  for( ; beginPassage < end; beginPassage += _increment ) {
    int endPassage = beginPassage + _windowSize;

    int scoreBegin = lemur_compat::max( beginPassage, begin );
    int scoreEnd = lemur_compat::min( endPassage, end );

    const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& childResults = _child->score( documentID, scoreBegin, scoreEnd, documentLength );

    for( int i=0; i<childResults.size(); i++ ) {
      indri::api::ScoredExtentResult result( childResults[i].score, documentID, scoreBegin, scoreEnd );
      _scores.push_back( result );
    }
  }

  return _scores;
}

void indri::infnet::FixedPassageNode::annotate( indri::infnet::Annotator& annotator, int documentID, int begin, int end ) {
  annotator.add(this, documentID, begin, end);

  // round down to find where the passage starts
  int beginPassage = (begin / _increment) * _increment;

  for( ; beginPassage < end; beginPassage += _increment ) {
    int endPassage = beginPassage + _windowSize;

    int scoreBegin = lemur_compat::max( beginPassage, begin );
    int scoreEnd = lemur_compat::min( endPassage, end );

    _child->annotate( annotator, documentID, scoreBegin, scoreEnd );
  }
}

bool indri::infnet::FixedPassageNode::hasMatch( int documentID ) {
  return _child->hasMatch( documentID );
}


const std::string& indri::infnet::FixedPassageNode::getName() const {
  return _name;
}

void indri::infnet::FixedPassageNode::indexChanged( indri::index::Index& index ) {
  // do nothing
}

