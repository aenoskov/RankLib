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
// 21 October 2004 -- dam
//
// Computes an "identity measure" similarity
// as described in:
//
// Methods for Identifying Plagiarized Text
// Hoad and Zobel 
//

#ifndef INDRI_IDENTITYSIMILARITYNODE_HPP
#define INDRI_IDENTITYSIMILARITYNODE_HPP

#include <math.h>
#include "indri/BeliefNode.hpp"
#include "indri/StatisticsBeliefNode.hpp"
#include <vector>
#include "indri/greedy_vector"

class IdentitySimilarityNode : public BeliefNode {
private:
  std::vector<StatisticsBeliefNode*> _children;
  greedy_vector<ScoredExtentResult> _scores;
  std::string _name;

  int _variation;

  int my_abs( int );
  double my_abs( double );

public:
  IdentitySimilarityNode( const std::string& name, int variation = 1 );
  IdentitySimilarityNode( const std::string& name, const std::vector<StatisticsBeliefNode*>& children, int variation = 1 );

  const greedy_vector<ScoredExtentResult>& score( int documentID, int begin, int end, int documentLength );
  void annotate( class Annotator& annotator, int documentID, int begin, int end );
  
  // these nodes don't support max-score skipping
  double maximumScore() { return INDRI_HUGE_SCORE; }
  double maximumBackgroundScore() { return INDRI_TINY_SCORE; }
  
  bool hasMatch( int documentID );
  int nextCandidateDocument();
  const std::string& getName() const;

  void setVariation( int variation ) { _variation = variation; }
  int getVariation() { return _variation; }
};

#endif // INDRI_IDENTITYSIMILARITYNODE_HPP

