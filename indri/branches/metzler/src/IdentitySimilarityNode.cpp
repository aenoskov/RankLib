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
// IdentitySimilarityNode
//
// 21 Oct 2004 -- dam
//

#include "indri/IdentitySimilarityNode.hpp"
#include "indri/ListBeliefNode.hpp"
#include "lemur/lemur-compat.hpp"
#include "indri/ScoredExtentResult.hpp"
#include "indri/Annotator.hpp"
#include "indri/CachedFrequencyBeliefNode.hpp"

IdentitySimilarityNode::IdentitySimilarityNode( const std::string& name, int variation ) :
  _name( name ),
  _variation( variation )
{
}

IdentitySimilarityNode::IdentitySimilarityNode( const std::string& name, const std::vector<StatisticsBeliefNode*>& children, int variation ) :
  _children( children ),
  _name( name ),
  _variation( variation )
{
}

const greedy_vector<ScoredExtentResult>& IdentitySimilarityNode::score( int documentID, int begin, int end, int documentLength ) {
  double score = 0.0;
  double variance = sqrt((double)10); // TODO: set this elsewhere
  
  int docCount = _children[0]->getDocCount();
  int queryLen = _children[0]->getQueryLength();
  int extentLen = end - begin;
  INT64 contextLen = _children[0]->getContextSize();
  
  /*std::cout << "documentID = " << documentID << std::endl;
  std::cout << "start = " << begin << std::endl;
  std::cout << "end = " << end << std::endl;
  std::cout << "documentLength = " << documentLength << std::endl;
	
  std::cout << "extentLen = " << extentLen << std::endl;
  std::cout << "queryLen = " << queryLen << std::endl;
  std::cout << "docCount = " << docCount << std::endl;*/

  // get the identity similarity measure variation
  int variation = getVariation();

  for( unsigned int i=0; i<_children.size(); i++ ) {
    StatisticsBeliefNode *child = _children[i];

    if( !child ) {
      std::cout << "[IdentitySimilarityNode] child is NOT a StatisticsBeliefNode (skipping) --> " << _children[i]->getName() << std::endl;
      continue;
    } 

    int df = child->getDocOccurrences();
    int tf = child->hasMatch( documentID ) ? child->matches( begin, end ) : 0;
    int qf = child->getQF();
    INT64 cf = child->getOccurrences();

    if( ( tf == 0 && variation < 50 ) || qf == 0 )
      continue;
    
    double val = 0.0;

    if( variation == 0 ) { // coordinate level matching
      val = 1;
    }
    else if( variation == 1 ) {
      val = log( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + abs( extentLen - queryLen ) );
    }
    else if( variation == 2 ) {
      val = log( 1.0 + 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + log( 1.0 + abs( extentLen - queryLen ) ) );
    }
    else if( variation == 3 ) {
      val = log( 1.0 + 1.0*docCount / df ) * (tf + qf ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + log( 1.0 + abs( extentLen - queryLen ) ) );
    }
    else if( variation == 4 ) {
      val = log( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + log( 1.0 + abs( extentLen - queryLen ) ) );
    }
    else if( variation == 5 ) {
      val = ( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + log( 1.0 + abs( extentLen - queryLen ) ) );
    }
    else if( variation == 6 ) { // log |D| - log |Q| variant of 4
      val = log( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 - abs( log( 1.0*extentLen ) - log( 1.0*queryLen ) ) );
    }
    else if( variation == 7 ) { // log |D| - log |Q| variant of 5
      val = ( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 - abs( log( 1.0*extentLen ) - log( 1.0*queryLen ) ) );
    }
    else if( variation == 8 ) { // min(|D|, |Q|) / max(|D|, |Q|) variant of 4
      val = log( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + exp( - abs( log( 1.0*extentLen ) - log( 1.0*queryLen ) ) ) );
    }
    else if( variation == 9 ) { // min(|D|, |Q|) / max(|D|, |Q|) variant of 5
      val = ( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + exp( - abs( log( 1.0*extentLen ) - log( 1.0*queryLen ) ) ) );
    }    
    else if( variation == 10 ) {
      val = log( 1.0 + qf ) * log ( 1.0 + tf ) * log( ( docCount + 1.0 ) / ( df + 0.5 ) );
    }
    else if( variation == 50 ) { // Dirichlet smoothing w/ mu = 1.0
      val = qf * log ( ( tf + ( 1.0*cf / contextLen ) ) / ( 1.0 + extentLen ) );
    }
    else if( variation == 51 ) { // Dirichlet smoothing w/ mu = 1.0
      val = qf * log ( ( tf + ( 1.0*cf / contextLen ) ) / ( 1.0 + extentLen ) );
    }
    else { // defaults to variation 1
      val = log( 1.0*docCount / df ) / ( 1.0 + abs( tf - qf ) );
      val *= 1.0 / (1.0 + abs( extentLen - queryLen ) );
    }

    /*std::cout << "child name = " << child->getName() << std::endl;
    std::cout << "df = " << df << std::endl;
    std::cout << "tf = " << tf << std::endl;  	
    std::cout << "qf = " << qf << std::endl;
    std::cout << "contextLen = " << contextLen << std::endl;
    std::cout << "cf = " << cf << std::endl;
    std::cout << "val = " << val << std::endl;
    std::cout << "variation = " << variation << std::endl;*/
    
    score += (1.0 / qf) * val;
    
    // special, quirky case that must be handled
    // can this be done better?
    CachedFrequencyBeliefNode* cacheNode = dynamic_cast<CachedFrequencyBeliefNode*>(child);
    if( cacheNode )
      cacheNode->advance();
  }

  if( variation < 50 ) { // always return the log of "true" similarity value
	  if( score == 0.0 )
		  score = -100000.0; // TODO: change this to SMALL_SCORE
	  else
		  score = log( score );
  }

  if( variation >= 50 && score == 0.0 )
	  score = -100000.0; // TODO: change this to SMALL_SCORE

  if( variation == 51 ) {
    //std::cout << "ORIGINAL SCORE = " << score << std::endl;
    //std::cout << "extentLen = " << extentLen << ", queryLen = " << queryLen << std::endl;
    score += -0.5 * ( extentLen - queryLen ) * ( extentLen - queryLen ) / variance;
    //std::cout << "AFTER SCORE = " << score << std::endl;
  }
  
  _scores.clear();
  _scores.push_back( ScoredExtentResult( score, documentID, begin, end ) );

  return _scores;
}

void IdentitySimilarityNode::annotate( class Annotator& annotator, int documentID, int begin, int end ) {
  annotator.add(this, documentID, begin, end);

  for( unsigned int i=0; i<_children.size(); i++ ) {
    _children[i]->annotate( annotator, documentID, begin, end );
  }
}

bool IdentitySimilarityNode::hasMatch( int documentID ) {
  for( unsigned int i=0; i<_children.size(); i++ ) {
    if( _children[i]->hasMatch( documentID ) )
      return true;
  }

  return false;
}

int IdentitySimilarityNode::nextCandidateDocument() {
  int nextCandidate = MAX_INT32;
  
  for( unsigned int i=0; i<_children.size(); i++ ) {
    nextCandidate = lemur_compat::min<int>( nextCandidate, _children[i]->nextCandidateDocument() );
  }

  return nextCandidate;
}

const std::string& IdentitySimilarityNode::getName() const {
  return _name;
}

