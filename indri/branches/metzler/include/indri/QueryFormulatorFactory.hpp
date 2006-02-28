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

#ifndef INDRI_QUERYFORMULATORFACTORY_HPP
#define INDRI_QUERYFORMULATORFACTORY_HPP

namespace indri
{
  namespace query
  {
    class QueryFormulatorFactory {
    public:
      static QueryFormulator* get( const std::string& spec );
    };
  }
}

#endif // INDRI_QUERYFORMULATORFACTORY_HPP

