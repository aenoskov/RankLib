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
// queryweight
//
// 22 November 2005 -- dam
//

#include "indri/Parameters.hpp"
#include "indri/QueryFeatureWeighter.hpp"

static bool copy_parameters_to_string_vector( std::vector<std::string>& vec, indri::api::Parameters p, const std::string& parameterName ) {
  if( !p.exists(parameterName) )
    return false;

  indri::api::Parameters slice = p[parameterName];
  
  for( int i=0; i<slice.size(); i++ ) {
    vec.push_back( slice[i] );
  }

  return true;
}

static void open_indexes( indri::api::QueryEnvironment& environment, indri::api::Parameters& param ) {
  if( param.exists( "index" ) ) {
    indri::api::Parameters indexes = param["index"];

    for( unsigned int i=0; i < indexes.size(); i++ ) {
      environment.addIndex( std::string(indexes[i]) );
    }
  }

  if( param.exists( "server" ) ) {
    indri::api::Parameters servers = param["server"];

    for( unsigned int i=0; i < servers.size(); i++ ) {
      environment.addServer( std::string(servers[i]) );
    }
  }

  std::vector<std::string> smoothingRules;
  if( copy_parameters_to_string_vector( smoothingRules, param, "rule" ) )
    environment.setScoringRules( smoothingRules );
}

int main( int argc, char** argv ) {
  try {
    indri::api::Parameters& param = indri::api::Parameters::instance();
    param.loadCommandLine( argc, argv );

    indri::api::QueryEnvironment env;
    open_indexes( env, param );
              
    indri::api::Parameters queries = param[ "query" ];
    int numDocs = (int)param[ "documents" ];
    int queryOffset = param.get( "queryOffset", 0 );
    
    indri::api::QueryAnnotation *results;
    std::string queryText;
    int queryNumber;
    
    for( int i = 0; i < queries.size(); i++ ) {
      if( queries[ i ].exists( "number" ) ) {
	queryText = (std::string)queries[ i ][ "text" ];
	queryNumber = (int)queries[ i ][ "number" ];
      }
      else {
	queryText = (std::string)queries[ i ];
	queryNumber = queryOffset + i;
      }

      std::cout << "# qid: " << queryNumber << " query: " << queryText << std::endl;
      results = env.runAnnotatedQuery( queryText, numDocs );
      indri::query::QueryFeatureWeighter queryWeighter( env, results );
      queryWeighter.generate();
      
    }
  } catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  } catch( ... ) {
    std::cout << "Caught an unhandled exception" << std::endl;
  }

  return 0;
}

