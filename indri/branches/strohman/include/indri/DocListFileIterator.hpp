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
// DocListFileIterator
//
// 9 January 2004 - tds
//

#ifndef INDRI_DOCLISTFILEITERATOR_HPP
#define INDRI_DOCLISTFILEITERATOR_HPP

#include "indri/DocListIterator.hpp"
#include "indri/TermData.hpp"

namespace indri {
  namespace index {
    class DocListFileIterator {
    public:
      struct DocListData {
        DocListIterator* iterator;
        TermData* termData;
      };

      struct iterator_greater {
        bool operator() ( const DocListFileIterator*const& one, const DocListFileIterator*const& two ) const {
          assert( !one->finished() && !two->finished() );
          
          const DocListData* oneData = one->currentEntry();
          const DocListData* twoData = two->currentEntry();

          const char* oneTerm = oneData->termData->term;
          const char* twoTerm = twoData->termData->term;

          int result = strcmp( oneTerm, twoTerm );

          // if terms don't match, we're done
          if( result < 0 )
            return false;
          if( result > 0 )
            return true;

          // terms match, so go by document
          int oneDocument = oneData->iterator->currentEntry()->document;
          int twoDocument = twoData->iterator->currentEntry()->document;

          return oneDocument > twoDocument;
        }
      };

      virtual ~DocListFileIterator() {};
      
      virtual bool finished() const = 0;
      virtual void startIteration() = 0;

      virtual bool nextEntry() = 0;
      virtual DocListData* currentEntry() = 0;
      virtual const DocListData* currentEntry() const = 0;
    };
  }
}

#endif // INDRI_DOCLISTITERATOR_HPP

