
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
// QueryFeatureWeighter
//
// 22 November 2005 -- dam
//

#include "indri/QueryFeatureWeighter.hpp"
#include <math.h>

indri::query::QueryFeatureWeighter::QueryFeatureWeighter( 
                  indri::api::QueryEnvironment& env,
		  indri::api::QueryAnnotation* results )
  :
  _env( env ),
  _results( results ),
  _annotations( results->getAnnotations() )
{
}

indri::query::QueryFeatureWeighter::~QueryFeatureWeighter()
{
}

void indri::query::QueryFeatureWeighter::generate() {
  const indri::api::QueryAnnotationNode *queryTree = _results->getQueryTree();
  _treeWalker( queryTree );
}

void indri::query::QueryFeatureWeighter::_treeWalker( const indri::api::QueryAnnotationNode *node ) {
  // if we found a term/proximity node, then compute the weight for it
  if( node->type == "RawScorerNode" ) {
    //indri::api::QueryEnvironment::ExpressionStatistics* stats =  _env.expressionStatistics( node->queryText );
    std::vector<indri::api::ScoredExtentResult> matches = (_annotations.find( node->name ))->second;
    std::string smoothing = "";
    double weight = _computeWeights( matches, 0, smoothing );
    //delete stats;

    std::cout << node->queryText << "\t" << weight << std::endl;
    return; // don't process children of these nodes
  }

  // walk the query tree
  std::vector<indri::api::QueryAnnotationNode*> children = node->children;
  for( int i = 0; i < children.size(); i++ )
    _treeWalker( children[i] );
}

double indri::query::QueryFeatureWeighter::_computeWeights( std::vector<indri::api::ScoredExtentResult>& matches, indri::api::QueryEnvironment::ExpressionStatistics* stats, std::string& smoothing ) {
  double collectionSize = _env.termCount();
  //indri::query::TermScoreFunction* function = 0;  
  int docCount = _env.documentCount();
  
  // compute tfs
  std::map<int, int> tf; // tf map (docid -> tf)
  for( int i = 0; i < matches.size(); i++ )
    tf[ matches[i].document ]++;

  // get term score function
  //function = indri::query::TermScoreFunctionFactory::get( smoothing, stats->occurrences, collectionSize, stats->documentOccurrences, docCount );

  // compute term weight
  double weight = 0.0;
  std::vector<indri::api::ScoredExtentResult> rankedList = _results->getResults();
  for( int i = 0; i < rankedList.size(); i++ ) {
    int docid = rankedList[i].document;
    int documentLength = rankedList[i].end - rankedList[i].begin;
    double termScore = log( double( tf[docid] ) / double( documentLength ) ); //function->scoreOccurrence( tf[ docid ], documentLength );
    weight += exp( rankedList[i].score + termScore );
  }

  return weight;
}
