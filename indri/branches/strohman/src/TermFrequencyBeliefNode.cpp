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

#include "indri/TermFrequencyBeliefNode.hpp"
#include "indri/InferenceNetwork.hpp"

TermFrequencyBeliefNode::TermFrequencyBeliefNode( const std::string& name,
                                                 class InferenceNetwork& network,
                                                 int listID,
                                                 TermScoreFunction& scoreFunction )
  :
  _name(name),
  _network(network),
  _listID(listID),
  _function(scoreFunction)
{
}

TermFrequencyBeliefNode::~TermFrequencyBeliefNode() {
}

const greedy_vector<indri::index::DocListIterator::TopDocument>& TermFrequencyBeliefNode::topdocs() const {
  if( _list )
    return _list->topDocuments();

  return _emptyTopdocs;
}

int TermFrequencyBeliefNode::nextCandidateDocument() {
  if( _list ) {
    const indri::index::DocListIterator::DocumentData* entry = _list->currentEntry();
    
    if( entry ) {
      return entry->document;
    }
  }

  return MAX_INT32;
}

double TermFrequencyBeliefNode::maximumBackgroundScore() {
  // TODO: fix me
  return INDRI_HUGE_SCORE;
}

double TermFrequencyBeliefNode::maximumScore() {
  // TODO: fix me
  return INDRI_HUGE_SCORE;
}

const greedy_vector<ScoredExtentResult>& TermFrequencyBeliefNode::score( int documentID, int begin, int end, int documentLength ) {
  assert( begin == 0 && end == documentLength ); // FrequencyListCopier ensures this condition
  _extents.clear();

  if( _list ) {
    const indri::index::DocListIterator::DocumentData* entry = _list->currentEntry();
    int count = ( entry && entry->document == documentID ) ? entry->positions.size() : 0;
    double score = _function.scoreOccurrence( count, documentLength );

    _extents.push_back( ScoredExtentResult( score, documentID, begin, end ) );
  }

  return _extents;
}

bool TermFrequencyBeliefNode::hasMatch( int documentID ) {
  if( _list ) {
    const indri::index::DocListIterator::DocumentData* entry = _list->currentEntry();
    return ( entry && entry->document == documentID );
  }

  return false;
}

const std::string& TermFrequencyBeliefNode::getName() const {
  return _name;
}

void TermFrequencyBeliefNode::indexChanged( indri::index::Index& index ) {
  // fetch the next inverted list
  _list = _network.getDocIterator( _listID );
}

void TermFrequencyBeliefNode::annotate( Annotator& annotator, int documentID, int begin, int end ) {
  // can't annotate -- don't have position info
}
