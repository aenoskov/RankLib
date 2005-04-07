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
// ExtentRestrictionNode
//
// 6 July 2004 -- tds
//

#include "indri/ExtentRestrictionNode.hpp"
#include "indri/Annotator.hpp"
#include "lemur/lemur-compat.hpp"

ExtentRestrictionNode::ExtentRestrictionNode( const std::string& name, BeliefNode* child, ListIteratorNode* field ) :
  _name(name),
  _child(child),
  _field(field)
{
}

int ExtentRestrictionNode::nextCandidateDocument() {
  return _child->nextCandidateDocument();
}

double ExtentRestrictionNode::maximumBackgroundScore() {
  return INDRI_TINY_SCORE;
}

double ExtentRestrictionNode::maximumScore() {
  return INDRI_HUGE_SCORE;
}

const greedy_vector<ScoredExtentResult>& ExtentRestrictionNode::score( int documentID, int begin, int end, int documentLength ) {
  // we're going to run through the field list, etc.
  const greedy_vector<bool>& matches = _child->hasMatch( documentID, _field->extents() );
  assert( matches.size() == _field->extents().size() );

  greedy_vector<Extent>::const_iterator iter;
  _scores.clear();


  for( size_t i = 0; i < matches.size(); i++ ) {
    if( !matches[i] )
      continue;

    iter = &_field->extents()[i];

    if( iter->end < begin )
      continue; // this one isn't relevant to our cause

    if( end < iter->begin )
      break; // we've passed all the relevant fields

    if( iter->end - iter->begin == 0 )
      continue; // this field has no text in it

    int scoreBegin = lemur_compat::max( iter->begin, begin );
    int scoreEnd = lemur_compat::min( iter->end, end );

    const greedy_vector<ScoredExtentResult>& childResults = _child->score( documentID, scoreBegin, scoreEnd, documentLength );

    for( int i=0; i<childResults.size(); i++ ) {
      ScoredExtentResult result( childResults[i].score, documentID, scoreBegin, scoreEnd );
      _scores.push_back( result );
    }
  }

  return _scores;
}

void ExtentRestrictionNode::annotate( Annotator& annotator, int documentID, int begin, int end ) {
  annotator.add(this, documentID, begin, end);
  // we're going to run through the field list, etc.
  greedy_vector<Extent>::const_iterator fieldEnd = _field->extents().end();
  greedy_vector<Extent>::const_iterator fieldBegin = _field->extents().begin();
  greedy_vector<Extent>::const_iterator iter;

  for( iter = fieldBegin; iter != fieldEnd; iter++ ) {
    if( iter->end < begin )
      continue; // this one isn't relevant to our cause

    if( end < iter->begin )
      break; // we've passed all the relevant fields

    if( iter->end - iter->begin == 0 )
      continue; // this field has no text in it

    int scoreBegin = lemur_compat::max( iter->begin, begin );
    int scoreEnd = lemur_compat::min( iter->end, end );

    _child->annotate( annotator, documentID, scoreBegin, scoreEnd );
  }
}

//
// hasMatch
//

bool ExtentRestrictionNode::hasMatch( int documentID ) {
  return _child->hasMatch( documentID );
}

//
// hasMatch
// 

const greedy_vector<bool>& ExtentRestrictionNode::hasMatch( int documentID, const greedy_vector<Extent>& extents ) {
  // just delegate -- not perfect, but close
  return _child->hasMatch( documentID, extents );
}

//
// getName
//

const std::string& ExtentRestrictionNode::getName() const {
  return _name;
}

//
// indexChanged
//

void ExtentRestrictionNode::indexChanged( indri::index::Index& index ) {
  // do nothing
}

