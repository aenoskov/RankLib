/*==========================================================================
  Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.

  Use of the Lemur Toolkit for Language Modeling and Information Retrieval
  is subject to the terms of the software license set forth in the LICENSE
  file included with this software, and also available at
  http://www.cs.cmu.edu/~lemur/license.html 
  as well as the conditions below.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  3. The names "Indri", "Center for Intelligent Information Retrieval", 
  "CIIR", and "University of Massachusetts" must not be used to
  endorse or promote products derived from this software without
  prior written permission. To obtain permission, contact
  indri-info@ciir.cs.umass.edu.

  4. Products derived from this software may not be called "Indri" nor 
  may "Indri" appear in their names without prior written permission of 
  the University of Massachusetts. To obtain permission, contact 
  indri-info@ciir.cs.umass.edu.

  THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF MASSACHUSETTS AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
  ==========================================================================
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

ContextCountAccumulator::ContextCountAccumulator( const std::string& name, ListIteratorNode* matches, ListIteratorNode* context ) :
  _matches(matches),
  _context(context)
{
}

ContextCountAccumulator::~ContextCountAccumulator() {
}

const std::string& ContextCountAccumulator::getName() const {
  return _name;
}

UINT64 ContextCountAccumulator::getOccurrences() const {
  return _occurrences;
}

UINT64 ContextCountAccumulator::getContextSize() const {
  return _contextSize;
}

const EvaluatorNode::MResults& ContextCountAccumulator::getResults() {
  // we must be finished, so now is a good time to add our results to the ListCache
  _results.clear();

  _results[ "occurrences" ].push_back( ScoredExtentResult( _occurrences, 0 ) );
  _results[ "contextSize" ].push_back( ScoredExtentResult( _contextSize, 0 ) );

  return _results;
}

const ListIteratorNode* ContextCountAccumulator::getContextNode() const {
  return _context;
}

const ListIteratorNode* ContextCountAccumulator::getMatchesNode() const {
  return _matches;
}

void ContextCountAccumulator::evaluate( int documentID, int documentLength ) {
  UINT64 documentOccurrences; 
  UINT64 documentContextSize;

  if( !_context ) {
    documentOccurrences = _matches->extents().size();
    documentContextSize = documentLength;
  } else {
    documentOccurrences = 0;
    documentContextSize = 0;

    const greedy_vector<Extent>& matches = _matches->extents();
    const greedy_vector<Extent>& extents = _context->extents();
    unsigned int ex = 0;
    
    for( unsigned int i=0; i<matches.size() && ex < extents.size(); i++ ) {
      while( ex < extents.size() && matches[i].begin < extents[ex].begin )
        ex++;

      if( ex < extents.size() &&
        matches[i].begin >= extents[ex].begin &&
        matches[i].end <= extents[ex].end ) {
        documentOccurrences++;
      }
    }

    for( unsigned int i=0; i<extents.size(); i++ ) {
      documentContextSize += extents[i].end - extents[i].begin;
    }

    _contextSize += documentContextSize;
  } 

  if( documentOccurrences ) {
    _occurrences += documentOccurrences;
  }
}

int ContextCountAccumulator::nextCandidateDocument() {
  int candidate = _matches->nextCandidateDocument();

  if( _context ) {
    candidate = lemur_compat::min( candidate, _context->nextCandidateDocument() );
  }

  return candidate;
}

//
// indexChanged
//

void ContextCountAccumulator::indexChanged( indri::index::Index& index ) {
  // do nothing
}




