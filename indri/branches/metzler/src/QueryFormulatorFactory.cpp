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
// QueryFormulatorFactory
//
// 27 February 2006 -- dam
//

#include "indri/Parameters.hpp"
#include "indri/QueryFormulator.hpp"
#include "indri/QueryFormulatorFactory.hpp"

#include "indri/QueryLikelihoodQueryFormulator.hpp"
#include "indri/DependenceModelQueryFormulator.hpp"
//#include "indri/NamedPageQueryFormulator.hpp"
//#include "indri/RelevanceModelQueryFormulator.hpp"

static void queryformulatorfactory_parse( indri::api::Parameters& converted, const std::string& spec );

indri::query::QueryFormulator* indri::query::QueryFormulatorFactory::get( const std::string& stringSpec ) {
  indri::api::Parameters spec;
  queryformulatorfactory_parse( spec, stringSpec );
  std::string method = spec.get( "method", "ql" );

  if( method == "ql" ) {
    return new indri::query::QueryLikelihoodQueryFormulator();
  } else if( method == "dm" ) {
    double termWt = spec.get( "lambdaT", 0.8 );
    double orderedWt = spec.get( "lambdaO", 0.1 );
    double unorderedWt = spec.get( "lambdaU", 0.1 );
    std::string variant = spec.get( "variant", "fd" );

    return new indri::query::DependenceModelQueryFormulator( termWt, orderedWt, unorderedWt, variant );
  } else if( method == "rm" ) {
    
  } else if( method == "np" ) {
    
  }

  return new indri::query::QueryLikelihoodQueryFormulator();
}

static void queryformulatorfactory_parse( indri::api::Parameters& converted, const std::string& spec ) {
  int nextComma = 0;
  int nextColon = 0;
  int location = 0;

  for( location = 0; location < spec.length(); ) {
    nextComma = spec.find( ',', location );
    nextColon = spec.find( ':', location );

    std::string key = spec.substr( location, nextColon-location );
    std::string value = spec.substr( nextColon+1, nextComma-nextColon-1 );

    converted.set( key, value );

    if( nextComma > 0 )
      location = nextComma+1;
    else
      location = spec.size();
  }
}
