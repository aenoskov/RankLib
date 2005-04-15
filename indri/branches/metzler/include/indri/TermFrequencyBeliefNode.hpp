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
// TermFrequencyBeliefNode
//
// 25 August 2004 -- tds
//

#ifndef INDRI_TERMFREQUENCYBELIEFNODE_HPP
#define INDRI_TERMFREQUENCYBELIEFNODE_HPP

#include "indri/Annotator.hpp"
#include "indri/ScoredExtentResult.hpp"
//#include <string>
#include "indri/TermScoreFunction.hpp"
#include "indri/ListBeliefNode.hpp"
#include "indri/DocListIterator.hpp"
#include "indri/StatisticsBeliefNode.hpp"

class TermFrequencyBeliefNode : public StatisticsBeliefNode {
private:
  class InferenceNetwork& _network;
  TermScoreFunction& _function;
  greedy_vector<ScoredExtentResult> _extents;
  greedy_vector<bool> _matches;
  indri::index::DocListIterator* _list;
  double _maximumBackgroundScore;
  double _maximumScore;
  std::string _name;
  int _listID;

  greedy_vector<indri::index::DocListIterator::TopDocument> _emptyTopdocs;

public:
  TermFrequencyBeliefNode( const std::string& name,
    class InferenceNetwork& network,
    int listID,
    TermScoreFunction& scoreFunction );

  ~TermFrequencyBeliefNode();

  const greedy_vector<indri::index::DocListIterator::TopDocument>& topdocs() const;
  int nextCandidateDocument();
  void indexChanged( indri::index::Index& index );
  double maximumBackgroundScore();
  double maximumScore();
  const greedy_vector<ScoredExtentResult>& score( int documentID, int begin, int end, int documentLength );
  void annotate( class Annotator& annotator, int documentID, int begin, int end );
  bool hasMatch( int documentID );
  const greedy_vector<bool>& hasMatch( int documentID, const greedy_vector<Extent>& extents );
  const std::string& getName() const;
  
  double matches( int begin, int end );
  double contextSize( int begin, int end );
};

#endif // INDRI_TERMFREQUENCYBELIEFNODE_HPP


