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
// DocListIteratorNode
//
// 28 July 2004 -- tds
//

#include "indri/DocListIterator.hpp"
#include "indri/DocListIteratorNode.hpp"
#include "indri/Annotator.hpp"
#include "indri/InferenceNetwork.hpp"

DocListIteratorNode::DocListIteratorNode( const std::string& name, class InferenceNetwork& network, int listID ) :
  _name(name),
  _network(network),
  _listID(listID)
{
}

int DocListIteratorNode::nextCandidateDocument() {
  if( _list ) {
    indri::index::DocListIterator::DocumentData* info = _list->currentEntry();
    if( info ) { 
      return info->document;
    }
  }

  return MAX_INT32;
}

void DocListIteratorNode::prepare( int documentID ) {
  _extents.clear();

  if( !_list )
    return;

  indri::index::DocListIterator::DocumentData* info = _list->currentEntry();

  if( !info || info->document != documentID )
    return;
  
  greedy_vector<int>& positions = info->positions;

  for( int i = 0; i < positions.size(); i++ ) {
    _extents.push_back( Extent( positions[i], positions[i]+1 ) );
  }
}

const greedy_vector<Extent>& DocListIteratorNode::extents() {
  return _extents;
}

const std::string& DocListIteratorNode::getName() const {
  return _name;
}

void DocListIteratorNode::annotate( Annotator& annotator, int documentID, int begin, int end ) {
  annotator.addMatches( _extents, this, documentID, begin, end );
}

void DocListIteratorNode::indexChanged( indri::index::Index& index ) {
  _list = _network.getDocIterator( _listID );
}

