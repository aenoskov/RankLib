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
// SentenceSegmenterTransformation
//
// 12 October 2004 -- dam
//

#include "indri/SentenceSegmenterTransformation.hpp"

ParsedDocument* SentenceSegmenterTransformation::transform( ParsedDocument* document ) {

  int numTerms = document->positions.size();
  size_t sentenceBegin = 0;
  bool addTag = false;

  std::string prevTerm = "";
  std::string curTerm = "";
  std::string nextTerm = "";

  //std::string sentence = "";

  int numTags = document->tags.size();

  if( numTags )
    sentenceBegin = document->tags[ numTags - 1 ].end;

  assert( document->positions.size() == document->terms.size() );

  if( !numTerms || sentenceBegin >= numTerms )
    return document;
  
  TermExtent pos0 = document->positions[ sentenceBegin ];
  curTerm = std::string( document->text, pos0.begin, pos0.end - pos0.begin);
  
  for( size_t i = sentenceBegin; i < numTerms; i++ ) {

    if( i + 1 < numTerms ) {
      TermExtent pos = document->positions[ i+1 ];
      nextTerm = std::string( document->text, pos.begin, pos.end - pos.begin);
    }
    else {
      nextTerm = "";
    }

    //std::cout << curTerm << std::endl;

    int curTermLength = curTerm.length();
    char lastChar = '\0';
    if( curTermLength )
      lastChar = curTerm[ curTermLength - 1 ];

    if( lastChar == '?' || lastChar == '!' ) {
      addTag = true;
    }
    else if( lastChar == '.' ) {
      // next word is lowercase => not sentence boundary
	  if( nextTerm.length() > 0 && nextTerm[0] < 'A' || nextTerm[0] > 'Z' ) {
		addTag = false;
      }
      // current term uppercase and contains more than 1 '.' => not sentence boundary
      else if( curTerm[0] >= 'A' && curTerm[0] <= 'Z' && curTerm.find( ".", 0 ) < curTerm.length()-1 ) {
		addTag = false;
      }
      // current term uppercase and shorter than 5 characters => not sentence boundary
      else if( curTerm[0] >= 'A' && curTerm[0] <= 'Z' && curTerm.length() < 5 ) {
		addTag = false;
      }
      else 
		addTag = true;
    }
    else {
      addTag = false;
    }

    //sentence += curTerm + " ";

    if( addTag ) {
      //std::cout << "SENTENCE = " << sentence << std::endl;
      //sentence = "";
      TagExtent tag;
      tag.name = "sentence";
      tag.begin = sentenceBegin;
      tag.end = i + 1;
      tag.number = 0;

      assert( tag.begin <= tag.end );
      assert( tag.begin <= document->terms.size() );
      assert( tag.end   <= document->terms.size() );
      
      document->tags.push_back( tag );

      // new beginning of sentence position
      sentenceBegin = i + 1;
    }

    prevTerm = curTerm;
    curTerm = nextTerm;
  }

  return document;
}

void SentenceSegmenterTransformation::setHandler( ObjectHandler<ParsedDocument>& handler ) {
  _handler = &handler;
}

void SentenceSegmenterTransformation::handle( ParsedDocument* document ) {
  _handler->handle( transform( document ) );
}
