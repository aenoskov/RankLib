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
// RMExpander
// 
// 18 Aug 2004 -- dam
//

#include "indri/RMExpander.hpp"
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <set>

indri::query::RMExpander::RMExpander( indri::api::QueryEnvironment * env , indri::api::Parameters& param ) : indri::query::QueryExpander( env, param ) { }

std::string indri::query::RMExpander::expand( std::string originalQuery , std::vector<indri::api::ScoredExtentResult>& results ) {
  int fbDocs = _param.get( "fbDocs" , 10 );
  int fbTerms = _param.get( "fbTerms" , 10 );
  double fbOrigWt = _param.get( "fbOrigWeight", 0.5 );
  double mu = _param.get( "fbMu", 0 );

  std::vector<indri::api::DocumentVector*> docVectors = getDocumentVectors( results, fbDocs );
  std::vector<std::string> * rm_vocab = getVocabulary( docVectors );
  size_t vocabSize = rm_vocab->size();
  UINT64 colLen = _env->termCount();

  std::map<std::string, double> query_model;
  std::map<std::string, int> document_counts;
  std::vector< std::pair<std::string, double> > probs;

  // gather document vectors / statistics for top fbDocs ranked documents
  for( int doc = 0; doc < fbDocs; doc++ ) {
    if( doc >= results.size() ) break;

    indri::api::DocumentVector * docVec = docVectors[ doc ];
    std::vector<int> term_positions = docVec->positions();
    std::vector<std::string> term_list = docVec->stems();
    std::set<std::string> termInDocument;
    double score = exp( results[ doc ].score );

    size_t docLen = term_positions.size();

    // add in "seen" proportion of term probability
    std::vector<int>::iterator pos_iter;
    for( pos_iter = term_positions.begin() ; pos_iter != term_positions.end() ; ++pos_iter ) {
      query_model[ term_list[ *pos_iter ] ] += ( 1.0 / fbDocs ) * score / ( docLen + mu );
      termInDocument.insert( term_list[ *pos_iter ] );
    }

    std::set<std::string>::iterator setiter;
    for( setiter = termInDocument.begin(); setiter != termInDocument.end(); setiter++ ) {
      document_counts[ *setiter ]++;
    }

    // add in "unseen" proportion of term probability
    if( mu != 0.0 ) {
      std::vector<std::string>::iterator vocab_iter;
      for( vocab_iter = rm_vocab->begin() ; vocab_iter != rm_vocab->end() ; ++vocab_iter )
        query_model[ *vocab_iter ] += ( 1.0 / fbDocs ) * score * ( mu * double( getCF( *vocab_iter ) )/double(colLen) ) / ( docLen + mu );
    }
    
    delete docVec;
  }

  std::vector<std::string>::iterator vocab_iter;
  int minDocuments = _param.get( "fbMinDocs", 1 );
  bool randomize = _param.get( "fbRandomize", 0 );
  int countRemoved = 0;

  // count the number of terms that we'll need to remove
  for( vocab_iter = rm_vocab->begin() ; vocab_iter != rm_vocab->end() ; ++vocab_iter ) {
    if( document_counts[ *vocab_iter ] < minDocuments ) {
      countRemoved++;
    }
  }

  // if we want to randomly remove words, pick which ones to remove
  std::vector<bool> random_choices;
  if( randomize ) {
    random_choices.resize( document_counts.size(), false );
    int removed = 0;

    // randomly pick which terms to remove
    while( removed < countRemoved ) {
      int index = rand() % random_choices.size();

      if( random_choices[index] == false ) {
        random_choices[index] = true;
        removed++;
      }
    }
  }

  // sort terms based on P( w | \theta_Q )
  int i=0;
  for( vocab_iter = rm_vocab->begin() ; vocab_iter != rm_vocab->end() ; ++vocab_iter, ++i ) {
    double model = query_model[ *vocab_iter ];
    if( randomize && random_choices[i] ) {
      model = 0;
    } else if( document_counts[ *vocab_iter ] < minDocuments ) {
      model = 0;
    }

    probs.push_back( std::pair<std::string, double>( *vocab_iter , model ) );    
  }

  std::sort( probs.begin() , probs.end() , QueryExpanderSort() );
  delete rm_vocab;

  bool useWeights = (bool) _param.get( "fbUseWeights", true );

  if( !useWeights ) {
    for( int i=0; i<probs.size(); i++ ) {
      probs[i].second = 1;
    }
  }

  return buildQuery( originalQuery, fbOrigWt, probs, fbTerms );
}

