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
// QueryFrequencyWalker
//
// 23 October 2004 -- dam
//

#include <vector>

#ifndef INDRI_QUERYFREQUENCYWALKER_HPP
#define INDRI_QUERYFREQUENCYWALKER_HPP

class QueryFrequencyWalker : public indri::lang::Walker {
private:
  // keep track of nodes visited
  std::vector<indri::lang::StatisticsNode*> _nodes;

  // keep track of query text counts
  std::map<std::string, int> _counter;

  // query length
  int _queryLength;

  void _processNode( indri::lang::StatisticsNode* statsNode ) {
    std::string qtext = statsNode->queryText();
    std::map<std::string, int>::iterator iter = _counter.find( qtext );
    if( iter == _counter.end() )
      _counter[ qtext ] = 1;
    else
      _counter[ qtext ] = iter->second + 1;

	_queryLength++;
  	_nodes.push_back( statsNode );
  }

public:
  QueryFrequencyWalker() : _queryLength( 0 ) {}
  ~QueryFrequencyWalker() {}

  void setStatistics() {
  	for(int i = 0; i < _nodes.size(); i++ ) {
  	   //std::cout << "text = " << _nodes[i]->queryText() << ", qf = " << _counter[ _nodes[i]->queryText() ] << ", queryLength = " << _queryLength << std::endl;
  	   _nodes[i]->setQF( _counter[ _nodes[i]->queryText() ] );
  	   _nodes[i]->setQueryLength( _queryLength );
  	}
  }

  void after( indri::lang::RawScorerNode* statsNode ) {
  	_processNode( statsNode );
  }

  void after( indri::lang::TermFrequencyScorerNode* statsNode ) {
  	_processNode( statsNode );
  }
  
  void after( indri::lang::CachedFrequencyScorerNode* statsNode ) {
  	_processNode( statsNode );
  }
  
};

#endif // INDRI_QUERYFREQUENCYWALKER_HPP

