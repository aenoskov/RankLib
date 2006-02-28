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
// QueryFormulator
//
// 27 February 2005 -- dam
//

#include "indri/Parameters.hpp"

#ifndef INDRI_QUERYFORMULATOR_HPP
#define INDRI_QUERYFORMULATOR_HPP
namespace indri {
  namespace query {
    class QueryFormulator {
    public:
      virtual std::string formulate( const std::string& query ) = 0;
    };
  }
}

#endif // INDRI_QUERYFORMULATOR_HPP
