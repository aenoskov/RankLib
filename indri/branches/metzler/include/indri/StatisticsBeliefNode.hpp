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
// StatisticsBeliefNode
//
// 23 October 2004 -- dam
//
// designed to encapsulate and centralize
// the statistics associated with belief nodes

#ifndef INDRI_STATISTICSBELIEFNODE_HPP
#define INDRI_STATISTICSBELIEFNODE_HPP

#include "indri/BeliefNode.hpp"

class StatisticsBeliefNode : public BeliefNode {
protected:
  // number of matches in entire collection
  INT64 _occurrences;

  // number of documents this node matches
  INT64 _docOccurrences;
  
  // number of extents this node matches
  INT64 _extentOccurrences;
  
  // size of the context this node's scored under
  INT64 _totalContextSize;

  // total number of times the query text associated
  // with this node occurs in the query
  int _qf;

  // total number of documents in the collection
  int _docCount;

  // total number of StatisticsNodes within the network
  // (before optimization / unnecessary node removal)
  int _queryLength;
  
public:
  // returns the number of times this node matches in the
  // current document within the given given extent range  
  virtual int matches( int begin, int end ) = 0;
  
  // returns the size of the current document's
  // scorable context
  virtual int contextSize( int begin, int end ) = 0;
  
  void setStatistics( INT64 occurrences, INT64 contextSize, INT64 docOccurrences,
                      int qf, int docCount, int queryLength ) {
  	_occurrences = occurrences;
  	_totalContextSize = contextSize;
  	_docOccurrences = docOccurrences;
  	_qf = qf;
  	_docCount = docCount;
  	_queryLength = queryLength;
  }
  
  INT64 getOccurrences() { return _occurrences; }
  INT64 getDocOccurrences() { return _docOccurrences; }
  INT64 getExtentOccurrences() { return _extentOccurrences; }
  INT64 getContextSize() { return _totalContextSize; }
  int getQF() { return _qf; }
  int getDocCount() { return _docCount; }
  int getQueryLength() { return _queryLength; }
};

#endif // INDRI_STATISTICSBELIEFNODE_HPP

