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
// InferenceNetwork
//
// 24 February 2004 -- tds
//

#include "indri/InferenceNetwork.hpp"
#include "indri/SkippingCapableNode.hpp"

#include "indri/DocListIterator.hpp"
#include "indri/DocExtentListIterator.hpp"

void InferenceNetwork::_moveToDocument( int candidate ) {
  // move all document iterators
  std::vector<indri::index::DocListIterator*>::iterator iter;
  for( iter = _docIterators.begin(); iter != _docIterators.end(); iter++ ) {
    (*iter)->nextEntry( candidate );
  }

  // move all field iterators
  std::vector<indri::index::DocExtentListIterator*>::iterator fiter;
  for( fiter = _fieldIterators.begin(); fiter != _fieldIterators.end(); fiter++ ) {
    (*fiter)->nextEntry( candidate );
  }

  // prepare all extent iterator nodes
  std::vector<ListIteratorNode*>::iterator diter;
  for( diter = _listIteratorNodes.begin(); diter != _listIteratorNodes.end(); diter++ ) {
    (*diter)->prepare( candidate );
  }
}

//
// _indexFinished
//

void InferenceNetwork::_indexFinished( indri::index::Index& index ) {
  // doc iterators
  delete_vector_contents<indri::index::DocListIterator*>( _docIterators );

  // field iterators
  delete_vector_contents<indri::index::DocExtentListIterator*>( _fieldIterators );
}

//
// _indexChanged
//

void InferenceNetwork::_indexChanged( indri::index::Index& index ) {
  // doc iterators
  for( int i=0; i<_termNames.size(); i++ ) {
    indri::index::DocListIterator* iterator = index.docListIterator( _termNames[i] );
    if( iterator )
      iterator->startIteration();

    _docIterators.push_back( iterator );
  }

  // field iterators
  for( int i=0; i<_fieldNames.size(); i++ ) {
    indri::index::DocExtentListIterator* iterator = index.fieldListIterator( _fieldNames[i] );
    if( iterator )
      iterator->startIteration();

    _fieldIterators.push_back( iterator );
  }

  // extent iterator nodes
  std::vector<ListIteratorNode*>::iterator diter;
  for( diter = _listIteratorNodes.begin(); diter != _listIteratorNodes.end(); diter++ ) {
    (*diter)->indexChanged( index );
  }

  // belief nodes
  std::vector<BeliefNode*>::iterator biter;
  for( biter = _beliefNodes.begin(); biter != _beliefNodes.end(); biter++ ) {
    (*biter)->indexChanged( index );
  }

  // evaluator nodes
  std::vector<EvaluatorNode*>::iterator eiter;
  for( eiter = _evaluators.begin(); eiter != _evaluators.end(); eiter++ ) {
    (*eiter)->indexChanged( index );
  }
}

int InferenceNetwork::_nextCandidateDocument() {
  int candidate = MAX_INT32;

  for( unsigned int i=0; i<_complexEvaluators.size(); i++ ) {
    candidate = lemur_compat::min( candidate, _complexEvaluators[i]->nextCandidateDocument() );
  }
  
  return candidate;
}

void InferenceNetwork::_evaluateDocument( indri::index::Index& index, int document ) {
  int candidateLength = index.documentLength( document );

  for( unsigned int i=0; i<_complexEvaluators.size(); i++ ) {
    _complexEvaluators[i]->evaluate( document, candidateLength );
  }
}

InferenceNetwork::InferenceNetwork( Repository& repository ) :
  _repository(repository)
{
}

InferenceNetwork::~InferenceNetwork() {
  delete_vector_contents<indri::index::DocExtentListIterator*>( _fieldIterators );
  delete_vector_contents<indri::index::DocListIterator*>( _docIterators );
  delete_vector_contents<ListIteratorNode*>( _listIteratorNodes );
  delete_vector_contents<BeliefNode*>( _beliefNodes );
  delete_vector_contents<TermScoreFunction*>( _scoreFunctions );
  delete_vector_contents<EvaluatorNode*>( _evaluators );
}

indri::index::DocListIterator* InferenceNetwork::getDocIterator( int index ) {
  return _docIterators[index];
}

indri::index::DocExtentListIterator* InferenceNetwork::getFieldIterator( int index ) {
  return _fieldIterators[index];
}

int InferenceNetwork::addDocIterator( const std::string& termName ) {
  _termNames.push_back( termName );
  return _termNames.size()-1;
}

int InferenceNetwork::addFieldIterator( const std::string& fieldName ) {
  _fieldNames.push_back( fieldName );
  return _fieldNames.size()-1;
}

void InferenceNetwork::addListNode( ListIteratorNode* listNode ) {
  _listIteratorNodes.push_back( listNode );
}

void InferenceNetwork::addBeliefNode( BeliefNode* beliefNode ) {
  _beliefNodes.push_back( beliefNode );
}

void InferenceNetwork::addEvaluatorNode( EvaluatorNode* evaluatorNode ) {
  _evaluators.push_back( evaluatorNode );
}

void InferenceNetwork::addScoreFunction( TermScoreFunction* scoreFunction ) {
  _scoreFunctions.push_back( scoreFunction );
}

void InferenceNetwork::addComplexEvaluatorNode( EvaluatorNode* complexEvaluator ) {
  _complexEvaluators.push_back( complexEvaluator );
}

const std::vector<EvaluatorNode*>& InferenceNetwork::getEvaluators() const {
  return _evaluators;
}

void InferenceNetwork::_evaluateIndex( indri::index::Index& index ) {
  int lastCandidate = MAX_INT32;
  int collectionSize = index.documentBase() + index.documentCount();
  int scoredDocuments = 0;
  int candidate = 0;

  // don't need to do anything unless there are some
  // evaluators in the network that need full evaluation

  if( _complexEvaluators.size() ) {
    while(1) {
      // ask the root node for a candidate document
      // this asks the whole inference network for the
      // first document that might possibly produce a
      // usable (above the max score threshold) score
      candidate = _nextCandidateDocument();

      // if candidate is MAX_INT32, we're done
      if( candidate == MAX_INT32 || candidate > collectionSize ) {
        break;
      }

      // move all the doc info lists to this new document
      // in preparation for scoring
      if( candidate != lastCandidate ) {
        _moveToDocument( candidate );
      }

      // ask all the evaluators to evaluate this document
      _evaluateDocument( index, candidate );
      scoredDocuments++;

      // if that was the last document, we can quit now
      if( candidate+1 > collectionSize )
        break;

      // move all candidate iterators to candidate+1
      _moveToDocument( candidate+1 );
      lastCandidate = candidate+1;
    }
  }
}

//
// evaluate
//

const InferenceNetwork::MAllResults& InferenceNetwork::evaluate() {
  Repository::index_state indexes = _repository.indexes();
  
  for( int i=0; i<indexes->size(); i++ ) {
    indri::index::Index& index = *(*indexes)[i];

    // TODO: index->lockIterators();
    // TODO: index->lockStatistics();

    _indexChanged( index );

    // TODO: index->unlockStatistics()

    _evaluateIndex( index );

    // remove all the iterators
    _indexFinished( index );
  }

  _results.clear();
  for( unsigned int i=0; i<_evaluators.size(); i++ ) {
    _results[ _evaluators[i]->getName() ] = _evaluators[i]->getResults();
  }

  return _results;
}
