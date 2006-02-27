
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

#include "indri/QueryAnnotation.hpp"
#include "indri/QueryEnvironment.hpp"
#include "indri/ScoredExtentResult.hpp"
#include "indri/TermScoreFunction.hpp"
#include "indri/TermScoreFunctionFactory.hpp"

namespace indri {
  namespace query {
    class QueryFeatureWeighter {
    private:
      indri::api::QueryEnvironment& _env;
      indri::api::QueryAnnotation* _results;

      const std::map< std::string, std::vector<indri::api::ScoredExtentResult> >& _annotations;

      double _computeWeights( std::vector<indri::api::ScoredExtentResult>& matches, indri::api::QueryEnvironment::ExpressionStatistics* stats, std::string& smoothing );
      void _treeWalker( const indri::api::QueryAnnotationNode *node );

    public:
      QueryFeatureWeighter( indri::api::QueryEnvironment& env,
			    indri::api::QueryAnnotation* results );
      ~QueryFeatureWeighter();

      void generate();
    };
  }
}
