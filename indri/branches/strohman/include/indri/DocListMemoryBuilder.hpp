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
// DocListMemoryBuilder.hpp
//
// tds - 17 December 2003
//

#ifndef LEMUR_KEYFILEDOCLISTMEMORYBUILDER_HPP
#define LEMUR_KEYFILEDOCLISTMEMORYBUILDER_HPP

#include "lemur/RVLCompress.hpp"
#include <vector>
#include <assert.h>
#include "indri/greedy_vector"
#include "indri/DocListIterator.hpp"

namespace indri {
  namespace index {
    struct DocListMemoryBuilderSegment {
      DocListMemoryBuilderSegment( char* b, char* d, char* c ) {
        base = b;
        data = d;
        capacity = c;
      }

      char* base;
      char* data;
      char* capacity;
    };

    class DocListMemoryBuilderIterator : public DocListIterator {
      const greedy_vector< DocListMemoryBuilderSegment, 4 >* _lists;
      greedy_vector< DocListMemoryBuilderSegment, 4 >::const_iterator _current;
      indri::index::DocListIterator::DocumentData _data;
      greedy_vector<DocListIterator::TopDocument> _emptyTopDocuments;
      
      const char* _list;
      const char* _listEnd;
      bool _finished;

    public:
      DocListMemoryBuilderIterator();
      DocListMemoryBuilderIterator( class DocListMemoryBuilder& builder );

      void reset( class DocListMemoryBuilder& builder );
      void reset( const greedy_vector< DocListMemoryBuilderSegment, 4 >& lists );

      void startIteration();
      bool finished();
      bool nextEntry( int documentID );
      bool nextEntry();
      DocListIterator::DocumentData* currentEntry();
      greedy_vector<DocListIterator::TopDocument>& topDocuments();
    };

    class DocListMemoryBuilder {
    public:
      typedef DocListMemoryBuilderIterator iterator;
      friend class DocListMemoryBuilderIterator;

    private:
      int _documentFrequency;
      int _termFrequency;

      greedy_vector< DocListMemoryBuilderSegment, 4 > _lists;

      char* _list;
      char* _listBegin;
      char* _listEnd;

      char* _documentPointer;
      char* _locationCountPointer;

      int _lastLocation;
      int _lastDocument;
      int _lastTermFrequency;

      inline size_t _compressedSize( int documentID, int position );
      inline void _safeAddLocation( int documentID, int position );
      void _growAddLocation( int documentID, int position, size_t newDataSize );
      void _grow();
      void _terminateDocument();

    public:
      DocListMemoryBuilder();
      ~DocListMemoryBuilder();
      const DocListMemoryBuilder& operator=( DocListMemoryBuilder& other );
      
      void addLocation( int docID, int location );
      void clear();
      void flush();
      iterator* getIterator();
      bool empty();

      int documentFrequency() const;
      int termFrequency() const;
      size_t memorySize() const;
    };
  }
}

#endif // LEMUR_DOCLISTMEMORYBUILDER_HPP
