
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
// OverlappingExtentPassage
//
// 21 August 2006 -- tds
//

#include "indri/OverlappingExtentPassage.hpp"
#include "indri/Annotator.hpp"
#include "lemur/lemur-compat.hpp"

//
// FixedPassageNode
//

indri::infnet::OverlappingExtentPassage::OverlappingExtentPassage( const std::string& name,
                                                                   indri::infnet::BeliefNode* child,
                                                                   indri::infnet::FieldIteratorNode* field,
                                                                   int windowSize,
                                                                   int increment ) :
  _name(name),
  _child(child),
  _field(field),
  _windowSize(windowSize),
  _increment(increment)
{
}

//
// nextCandidateDocument
//

int indri::infnet::OverlappingExtentPassage::nextCandidateDocument() {
  return lemur_compat::max( _child->nextCandidateDocument(), _field->nextCandidateDocument() );
}

//
// maximumBackgroundScore
//

double indri::infnet::OverlappingExtentPassage::maximumBackgroundScore() {
  return INDRI_TINY_SCORE;
}

//
// maximumScore
//

double indri::infnet::OverlappingExtentPassage::maximumScore() {
  return INDRI_HUGE_SCORE;
}

//
// _scorePassage
//

void indri::infnet::OverlappingExtentPassage::_scorePassage( int documentID, int begin, int end, int documentLength, int passageBegin, int passageEnd ) {
  int scoreBegin = lemur_compat::max( passageBegin, begin );
  int scoreEnd = lemur_compat::min( passageEnd, end );

  const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& childResults = _child->score( documentID, scoreBegin, scoreEnd, documentLength );

  for( int i=0; i<childResults.size(); i++ ) {
    indri::api::ScoredExtentResult result( childResults[i].score, documentID, scoreBegin, scoreEnd );
    _scores.push_back( result );
  }
}

//
// score
//

const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& indri::infnet::OverlappingExtentPassage::score( int documentID, int begin, int end, int documentLength ) {
  _scores.clear();
  const indri::utility::greedy_vector<indri::index::Extent>& fieldExtents = _field->extents();

  // loop through all the passages
  for( int i = 0; i < fieldExtents.size() - _windowSize; i += _increment ) {
    int beginPassage = fieldExtents[i].begin;
    int endPassage = fieldExtents[i + _windowSize].end;

    _scorePassage( documentID, begin, end, documentLength, beginPassage, endPassage );
  }

  // if the last sentence wasn't included, score that:
  if( (fieldExtents.size() % _increment) != 0 && fieldExtents.size() > _windowSize ) {
    int beginPassage = fieldExtents[ fieldExtents.size() - _windowSize ].begin;
    int endPassage = fieldExtents.back().end;

    _scorePassage( documentID, begin, end, documentLength, beginPassage, endPassage );
  } else if( fieldExtents.size() < _windowSize && fieldExtents.size() > 0 ) {
    // if the document is too short to support a _windowSize passage, make a passage anyway
    int beginPassage = fieldExtents.front().begin;
    int endPassage = fieldExtents.back().end;

    _scorePassage( documentID, begin, end, documentLength, beginPassage, endPassage );
  }

  return _scores;
}

//
// annotate
//

void indri::infnet::OverlappingExtentPassage::annotate( indri::infnet::Annotator& annotator, int documentID, int begin, int end ) {
  annotator.add(this, documentID, begin, end);
  score(documentID, begin, end, end - begin);

  // for each scored region, we'll annotate:
  for( int i=0; i<_scores.size(); i++ ) {
    _child->annotate( annotator, documentID, _scores[i].begin, _scores[i].end );
  }
}

//
// hasMatch
//

bool indri::infnet::OverlappingExtentPassage::hasMatch( int documentID ) {
  return _child->hasMatch( documentID );
}

//
// hasMatch
//

const indri::utility::greedy_vector<bool>& indri::infnet::OverlappingExtentPassage::hasMatch( int documentID, const indri::utility::greedy_vector<indri::index::Extent>& extents ) {
  // the safe bet is to say that everything matches, even though that's not technically true
  _matches.clear();
  _matches.resize( extents.size(), true );
  return _matches;
}

//
// getName
//

const std::string& indri::infnet::OverlappingExtentPassage::getName() const {
  return _name;
}

//
// indexChanged
//

void indri::infnet::OverlappingExtentPassage::indexChanged( indri::index::Index& index ) {
  // do nothing
}

