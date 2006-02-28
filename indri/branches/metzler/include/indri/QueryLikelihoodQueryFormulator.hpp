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
// QueryLikelihoodQueryFormulator
//
// 27 February 2005 -- dam
//

namespace indri {
  namespace query {
    class QueryLikelihoodQueryFormulator : public QueryFormulator {
    public:
      std::string formulate( const std::string& query ) {
	std::stringstream ret;	
	ret << "#combine( " << query << " )";
	return ret.str();
      }
    };
  }
}
