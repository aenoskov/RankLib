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
// ContextCountAccumulator
//
// 24 February 2004 -- tds
//

#include "indri/ContextCountAccumulator.hpp"
#include <assert.h>
#include <vector>
#include "indri/QuerySpec.hpp"
#include "indri/EvaluatorNode.hpp"
#include "indri/DocumentCount.hpp"

indri::infnet::ContextCountAccumulator::ContextCountAccumulator( const std::string& name,
                                                                 ListIteratorNode* matches,
                                                                 ListIteratorNode* context,
                                                                 indri::lang::ListCache* cache, 
                                                                 indri::lang::ListCache::CachedList* list ) :
  _name(name),
  _matches(matches),
  _context(context),
  _occurrences(0),
  _contextSize(0),
  _cache(cache),
  _list(list)
{
}

indri::infnet::ContextCountAccumulator::~ContextCountAccumulator() {
}

const std::string& indri::infnet::ContextCountAccumulator::getName() const {
  return _name;
}

double indri::infnet::ContextCountAccumulator::getOccurrences() const {
  return _occurrences;
}

double indri::infnet::ContextCountAccumulator::getContextSize() const {
  return _contextSize;
}

const indri::infnet::EvaluatorNode::MResults& indri::infnet::ContextCountAccumulator::getResults() {
  // we must be finished, so now is a good time to add our results to the ListCache
  _list->occurrences = _occurrences;
  _list->contextSize = (UINT64)  _contextSize;

  _cache->insert( _list );

  _results.clear();

  _results[ "occurrences" ].push_back( indri::api::ScoredExtentResult( _occurrences, 0 ) );
  _results[ "contextSize" ].push_back( indri::api::ScoredExtentResult( _contextSize, 0 ) );

  return _results;
}

const indri::infnet::ListIteratorNode* indri::infnet::ContextCountAccumulator::getContextNode() const {
  return _context;
}

const indri::infnet::ListIteratorNode* indri::infnet::ContextCountAccumulator::getMatchesNode() const {
  return _matches;
}

void indri::infnet::ContextCountAccumulator::evaluate( int documentID, int documentLength ) {
  double documentOccurrences = 0;
  double documentContextSize = 0;

  if( !_context ) {
    // we're not evaluating a context expression, so our job is just to find
    // out how many times this expression occurs
    for( size_t i=0; i<_matches->extents().size(); i++ ) {
      const indri::index::Extent& extent = _matches->extents()[i];
      documentOccurrences += extent.weight;
    }

    _occurrences += documentOccurrences;

    // if we're making a CachedList, update the list
    if( _list ) {
      if( documentOccurrences > 0 ) {
        double fraction = double(documentOccurrences) / double(documentLength);
        _list->maximumContextFraction = lemur_compat::max<double>( fraction, _list->maximumContextFraction );

        _list->entries.push_back( indri::index::DocumentContextCount( documentID, documentOccurrences, documentLength ) );
      }
  
      _list->maximumContextSize = lemur_compat::max<int>( (int) documentLength, _list->maximumContextSize );
      _list->minimumContextSize = lemur_compat::min<int>( (int) documentLength, _list->minimumContextSize );
    }
  } else {
    // this is a context expression, so we need to know how many
    // time the inner expression occurs within the context.
    // at the same time, we want to know how big the context is
    const indri::utility::greedy_vector<indri::index::Extent>& matches = _matches->extents();
    const indri::utility::greedy_vector<indri::index::Extent>& extents = _context->extents();
    unsigned int ex = 0;

    for( unsigned int i=0; i<matches.size() && ex < extents.size(); i++ ) {
      // find a context extent that might possibly contain this match
      // here we're relying on the following invariants: 
      //    both arrays are sorted by beginning position
      //    the extents array may have some that are inside others, like:
      //      [1, 10] and [4, 6], or even [1,10] and [1,4]
      //      but it will never have overlapping extents, such as:
      //      [1, 10] and [5, 15]
      //    also, in the event that two inner extents start at the
      //      same position, the largest end position comes first
      //      (e.g. [1,10] comes before [1,4])
      // Therefore, if a match [a,b] is in any extent, it will be
      //   in the first one [c,d] such that d>=a.
      // Proof is by contradiction: if the match is in a context extent,
      //   but it's not the first one such that d>=a, then that context
      //   extent must overlap the first extent such that d>=a (which
      //   is not allowed).
      while( extents[ex].end < matches[i].begin ) {
        ex++;

        if( ex >= extents.size() ) break;
      }

      if( ex < extents.size() &&
        matches[i].begin >= extents[ex].begin &&
        matches[i].end <= extents[ex].end ) {
        documentOccurrences += matches[i].weight;
      }
    }

    for( unsigned int i=0; i<extents.size(); i++ ) {
      documentContextSize += extents[i].end - extents[i].begin;
    }

    // if we're making a CachedList, update the list
    if( _list && documentOccurrences > 0 ) {
      if( documentOccurrences > 0 ) {
        double fraction = double(documentOccurrences) / double(documentContextSize);
        _list->maximumContextFraction = lemur_compat::max<double>( fraction, _list->maximumContextFraction );

        _list->entries.push_back( indri::index::DocumentContextCount( documentID, documentOccurrences, (int) documentContextSize ) );
      }
      _list->maximumContextSize = lemur_compat::max<int>( (int) documentContextSize, _list->maximumContextSize );
      _list->minimumContextSize = lemur_compat::min<int>( (int) documentContextSize, _list->minimumContextSize );
    }

    _occurrences += documentOccurrences;
    _contextSize += documentContextSize;
  }
}

int indri::infnet::ContextCountAccumulator::nextCandidateDocument() {
  int candidate = _matches->nextCandidateDocument();

  if( _context ) {
    candidate = lemur_compat::min( candidate, _context->nextCandidateDocument() );
  }

  return candidate;
}

//
// indexChanged
//

void indri::infnet::ContextCountAccumulator::indexChanged( indri::index::Index& index ) {
  if( ! _context ) {
    _contextSize += index.termCount();
  }
}

//
// isComplex
//

bool indri::infnet::ContextCountAccumulator::isComplex() {
  return true;
}



