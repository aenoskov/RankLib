/*==========================================================================
  Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.

  Use of the Lemur Toolkit for Language Modeling and Information Retrieval
  is subject to the terms of the software license set forth in the LICENSE
  file included with this software, and also available at
  http://www.cs.cmu.edu/~lemur/license.html 
  as well as the conditions below.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  3. The names "Indri", "Center for Intelligent Information Retrieval", 
  "CIIR", and "University of Massachusetts" must not be used to
  endorse or promote products derived from this software without
  prior written permission. To obtain permission, contact
  indri-info@ciir.cs.umass.edu.

  4. Products derived from this software may not be called "Indri" nor 
  may "Indri" appear in their names without prior written permission of 
  the University of Massachusetts. To obtain permission, contact 
  indri-info@ciir.cs.umass.edu.

  THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF MASSACHUSETTS AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
  ==========================================================================
*/


//
// DocListIterator
//
// 9 January 2004 - tds
//

#ifndef INDRI_DOCLISTITERATOR_HPP
#define INDRI_DOCLISTITERATOR_HPP

#include "greedy_vector"
#include "indri/TermData.hpp"

namespace indri {
  namespace index {
    class DocListIterator {
    public:
      struct DocumentData {
        int document;
        greedy_vector<int> positions;
      };

      struct TopDocument {
        struct less {
          bool operator() ( const TopDocument& one, const TopDocument& two ) const {
            double oneFrac = double(one.count) / double(one.length);
            double twoFrac = double(two.count) / double(two.length);
            return oneFrac < twoFrac;
          }
        };

        struct docid_less {
          bool operator() ( const TopDocument& one, const TopDocument& two ) const {
            return one.document < two.document;
          }
        };

        TopDocument( int _document, int _count, int _length ) :
          document(_document),
          count(_count),
          length(_length)
        {
        }

        int document;
        int count;
        int length;
      };
      
      virtual ~DocListIterator() {};

      // get the iterator ready to return data; call this before calling currentEntry or nextEntry
      virtual void startIteration() = 0;

      // get the termData structure associated with this term
      virtual TermData* termData() = 0;

      // get a list of top documents for this iterator (must call startIteration() first)
      virtual const greedy_vector<TopDocument>& topDocuments() = 0;

      // return the current document entry if we're not finished, null otherwise.
      virtual DocumentData* currentEntry() = 0;
    
      // move to the next document in the list; return false if there are no more valid documents
      virtual bool nextEntry() = 0;

      // find the first document that contains this term that has an id >= documentID.
      // returns false if no such document exists.
      virtual bool nextEntry( int documentID ) = 0;

      // returns true if the iterator has no more entries
      virtual bool finished() = 0;
    };
  }
}

#endif // INDRI_DOCLISTITERATOR_HPP


