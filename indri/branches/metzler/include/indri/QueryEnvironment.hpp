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
// QueryEnvironment
//
// 9 March 2004 -- tds
//

#ifndef INDRI_QUERYENVIRONMENT_HPP
#define INDRI_QUERYENVIRONMENT_HPP

#include "indri/ScoredExtentResult.hpp"
#include "indri/QueryServer.hpp"
#include "indri/NetworkStream.hpp"
#include "indri/NetworkMessageStream.hpp"
#include "indri/Parameters.hpp"
#include "indri/PriorFactory.hpp"
#include "indri/ParsedDocument.hpp"
#include "indri/Repository.hpp"
#include "indri/QueryAnnotation.hpp"

class QueryEnvironment {
private:
  std::vector<QueryServer*> _servers;

  std::vector<Repository*> _repositories;
  std::vector<NetworkStream*> _streams;
  std::vector<NetworkMessageStream*> _messageStreams;

  Parameters _parameters;
  PriorFactory _priorFactory;

  void _mergeQueryResults( InferenceNetwork::MAllResults& results, std::vector<QueryServerResponse*>& responses );
  void _copyStatistics( std::vector<indri::lang::RawScorerNode*>& scorerNodes, InferenceNetwork::MAllResults& statisticsResults );

  std::vector<QueryServerResponse*> _runServerQuery( std::vector<indri::lang::Node*>& roots, int resultsRequested );
  void _sumServerQuery( InferenceNetwork::MAllResults& results, std::vector<indri::lang::Node*>& roots, int resultsRequested );
  void _mergeServerQuery( InferenceNetwork::MAllResults& results, std::vector<indri::lang::Node*>& roots, int resultsRequested );
  void _annotateQuery( InferenceNetwork::MAllResults& results, const std::vector<int>& documentIDs, std::string& annotatorName, indri::lang::Node* queryRoot );

  std::vector<ScoredExtentResult> _runQuery( InferenceNetwork::MAllResults& results,
                                             const std::string& q,
                                             int resultsRequested,
                                             const std::vector<int>* documentIDs,
                                             QueryAnnotation** annotation );
  void _scoredQuery( InferenceNetwork::MAllResults& results, indri::lang::Node* queryRoot, std::string& accumulatorName, int resultsRequested, const std::vector<int>* documentSet );

  QueryEnvironment( QueryEnvironment& other ) {}

public:
  QueryEnvironment();
  ~QueryEnvironment();

  void setMemory( UINT64 memory );
  void setScoringRules( const std::vector<std::string>& rules );
  void setStopwords( const std::vector<std::string>& stopwords );

  void addServer( const std::string& hostname );
  void addIndex( const std::string& pathname );
  void close();

  std::vector<ScoredExtentResult> runQuery( const std::string& query, int resultsRequested );
  std::vector<ScoredExtentResult> runQuery( const std::string& query, const std::vector<int>& documentSet, int resultsRequested );

  QueryAnnotation* runAnnotatedQuery( const std::string& query, int resultsRequested );  
  QueryAnnotation* runAnnotatedQuery( const std::string& query, const std::vector<int>& documentSet, int resultsRequested );

  std::vector<ParsedDocument*> documents( const std::vector<int>& documentIDs );
  std::vector<ParsedDocument*> documents( const std::vector<ScoredExtentResult>& results );

  std::vector<std::string> documentMetadata( const std::vector<int>& documentIDs, const std::string& attributeName );
  std::vector<std::string> documentMetadata( const std::vector<ScoredExtentResult>& documentIDs, const std::string& attributeName );

  INT64 termCount();
  INT64 termCount( const std::string& term );
  INT64 stemCount( const std::string& term );
  INT64 termFieldCount( const std::string& term, const std::string& field );
  INT64 stemFieldCount( const std::string& term, const std::string& field );
  std::vector<std::string> fieldList();
  INT64 documentCount();
  INT64 documentCount( const std::string& term );

  std::vector<DocumentVector*> documentVectors( const std::vector<int>& documentIDs );
};

#endif // INDRI_QUERYENVIRONMENT_HPP

