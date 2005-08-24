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
// WebFeatures
//
// 24 July 2005 -- dam
//

#ifndef INDRI_WEBFEATURES_HPP
#define INDRI_WEBFEATURES_HPP

#include <string>
#include <vector>
#include <map>

#include "indri/UnparsedDocument.hpp"
#include "indri/Parameters.hpp"
#include "indri/FileTreeIterator.hpp"
#include "indri/TaggedDocumentIterator.hpp"
#include "indri/TaggedTextParser.hpp"
#include "indri/Path.hpp"

namespace indri
{
  namespace parse
  {
    
    class WebFeatures {
    private:
      const std::string _corpusPath;
      const std::string _linkPath;

      std::ifstream _linkFile;
      std::string _curLinkDocno;
      int _curLinkCount;
      
      static int _computeURLDepth( const std::string& url );

      void _nextLinkDoc();
      int _getInlinkCount( const std::string& docno );

    public:
      WebFeatures( const std::string& corpusPath, const std::string& linkPath ) : _corpusPath( corpusPath ), _linkPath( linkPath ) {};
      void computeFeatures();
    };
  }
}

#endif
