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
// DependenceModelQueryFormulator
//
// 27 February 2005 -- dam
//

#include "indri/QueryFormulator.hpp"
#include <math.h>

namespace indri {
  namespace query {
    class DependenceModelQueryFormulator : public QueryFormulator {
    private:
      double _termWt;
      double _orderedWt;
      double _unorderedWt;
      std::string _type;

      std::vector<std::string> _extractTerms( const std::string query );
      
    public:
      DependenceModelQueryFormulator( double termWt = 0.8,
				      double orderedWt = 0.1,
				      double unorderedWt = 0.1,
				      const std::string& type = "fd" );
      
      std::string formulate( const std::string& query );
    };
  }
}
