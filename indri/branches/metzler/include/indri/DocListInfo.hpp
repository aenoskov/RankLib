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
// DocListInfo
//
// 9 January 2004 - tds
//

#ifndef INDRI_DOCLISTINFO_HPP
#define INDRI_DOCLISTINFO_HPP

#include "indri/DocPositionInfoList.hpp"
#include <indri/greedy_vector>

///
/// This is the DocInfo structure that Indri
/// indexes return from the docInfoList() and
/// docPositionInfoList() calls.  This structure is
/// also used internally by IndriIndex when merging
/// inverted lists.
///

namespace indri {
  namespace index {
    class DocListInfo : public DocInfo {
    private:
      int _documentID;
      int _termID;
      greedy_vector<int,32> _positions;

    public:
      // DocInfo interface
      int docID() const {
        return _documentID;
      }

      int termCount() const {
        return int(_positions.size());
      }

      virtual const int* positions() const {
        return &_positions.front();
      }

      // Internal index use
      int termID() const {
        return _termID;
      }

      void setDocID( int documentID ) {
        _documentID = documentID;
      }

      void setTermID( int termID ) {
        _termID = termID;
      }

      void addPosition( int position ) {
        _positions.push_back( position );
      }

      void addPositions( int* positions, int count ) {
        _positions.append( positions, positions + count );
      }

      void clear() {
        _positions.clear();
      }
    };
  }
}

#endif // KEYFILE_DOCLISTINFO_HPP

