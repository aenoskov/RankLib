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
// TermData.hpp
//
// 4 February 2004 -- tds
//

#ifndef INDRI_TERMDATA_HPP
#define INDRI_TERMDATA_HPP

#include "indri/TermFieldStatistics.hpp"
#include <indri/greedy_vector>
#include "lemur/lemur-compat.hpp"
#include "indri/RVLCompressStream.hpp"
#include "indri/RVLDecompressStream.hpp"

#ifdef WIN32
// remove warning about zero-sized arrays
#pragma warning ( disable: 4200 )
#endif 

namespace indri {
  namespace index {
    struct TermData {
    private:
      // these are private, bogus functions so that this object can never be copied
      // we don't want to be able to copy it, because any real copy operator needs to
      // take into account the data in the fields[] array, and we don't know how long it is.
      TermData( const TermData& other ) {}
      const TermData& operator= ( const TermData& other ) { return *this; }

    public:
      TermData() :
          maxDocumentFrequency(0),
          maxDocumentFraction(0),
          minDocumentLength(MAX_INT32)
      {
        term = 0;
      }
      
      struct term_less {
      public:
        bool operator () ( const TermData* one, const TermData* two ) const {
          return strcmp( one->term, two->term ) < 0;
        }
      };

      TermFieldStatistics corpus;

      float maxDocumentFraction;         // argmax_documents of (termCount/docLength)
      unsigned int maxDocumentFrequency; // maximum number of times this term appears in any given document
      unsigned int minDocumentLength;    // minimum length of any document that contains this term

      const char* term;                  // name of this term

      TermFieldStatistics fields[0];
    };
  }
}

inline indri::index::TermData* termdata_construct( void* buffer, int fieldCount ) {
  // call the constructor in place
  new(buffer) indri::index::TermData();

  // call field data constructors in place
  for( int i=0; i<fieldCount; i++ ) {
    new((char*)buffer +
        sizeof(indri::index::TermData) +
        sizeof(indri::index::TermFieldStatistics)*i) indri::index::TermFieldStatistics();
  }

  return (indri::index::TermData*) buffer;
}

inline indri::index::TermData* termdata_create( int fieldCount ) {
  // allocate enough room for the term data, plus enough room for fields
  void* buffer = malloc( sizeof(indri::index::TermData) + sizeof(indri::index::TermFieldStatistics)*fieldCount );
  return termdata_construct( buffer, fieldCount );
}

inline void termdata_destruct( indri::index::TermData* termData, int fieldCount ) {
  if( termData ) {
    termData->~TermData();

    for( int i=0; i<fieldCount; i++ ) {
      termData->fields[i].~TermFieldStatistics();
    }
  }
}

inline void termdata_delete( indri::index::TermData* termData, int fieldCount ) {
  if( termData ) {
    termdata_destruct( termData, fieldCount );
    free(termData);
  }
}

inline void termdata_clear( indri::index::TermData* termData, int fieldCount ) {
  termData->corpus.documentCount = 0;
  termData->corpus.totalCount = 0;
  termData->corpus.lastCount = 0;
  termData->corpus.lastDocument = 0;

  for( int i=0; i<fieldCount; i++ ) {
    indri::index::TermFieldStatistics& field = termData->fields[i];

    field.documentCount = 0;
    field.totalCount = 0;
    field.lastCount = 0;
    field.lastDocument = 0;
  }

  termData->maxDocumentFraction = 0;
  termData->maxDocumentFrequency = 0;
  termData->minDocumentLength = MAX_INT32;
}

inline void termdata_merge( indri::index::TermData* termData, indri::index::TermData* merger, int fieldCount ) {
  termData->corpus.documentCount += merger->corpus.documentCount;
  termData->corpus.totalCount += merger->corpus.totalCount;

  for( int i=0; i<fieldCount; i++ ) {
    indri::index::TermFieldStatistics& field = termData->fields[i];
    indri::index::TermFieldStatistics& mergeField = merger->fields[i];

    field.documentCount += mergeField.documentCount;
    field.totalCount += mergeField.totalCount;
  }

  termData->maxDocumentFraction = lemur_compat::max( termData->maxDocumentFraction, merger->maxDocumentFraction );
  termData->maxDocumentFrequency = lemur_compat::max( termData->maxDocumentFrequency, merger->maxDocumentFrequency );
  termData->minDocumentLength = lemur_compat::min( termData->minDocumentLength, termData->minDocumentLength );
}

inline int termdata_size( int fieldCount ) {
  return sizeof(indri::index::TermData) + fieldCount * sizeof(indri::index::TermFieldStatistics);
}

inline void termdata_compress( RVLCompressStream& stream, indri::index::TermData* termData, int fieldCount ) {
  // corpus statistics
  stream << termData->corpus.totalCount
         << termData->corpus.documentCount;

  // max-score statistics
  stream << termData->maxDocumentFrequency
         << termData->minDocumentLength
         << termData->maxDocumentFraction;

  // field statistics
  for( int i=0; i<fieldCount; i++ ) {
    stream << termData->fields[i].totalCount
          << termData->fields[i].documentCount;
  }
}

inline void termdata_decompress( RVLDecompressStream& stream, indri::index::TermData* termData, int fieldCount ) {
  // corpus statistics
  stream >> termData->corpus.totalCount
         >> termData->corpus.documentCount;

  // max-score statistics
  stream >> termData->maxDocumentFrequency
         >> termData->minDocumentLength
         >> termData->maxDocumentFraction;

  // field statistics
  for( int i=0; i<fieldCount; i++ ) {
    stream >> termData->fields[i].totalCount
           >> termData->fields[i].documentCount;
  }
}

#endif // INDRI_TERMDATA_HPP
