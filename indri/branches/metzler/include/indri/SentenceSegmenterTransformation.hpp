//
// SentenceSegmenterTransformation
// 
// 12 October 2004 -- dam
//

//
// A very simple heuristic sentence segmenter that
// adds 'sentence' extents around detected sentences.
//
// WARNING: currently only works in conjunction with TextParser
// (NOT any TaggedTextParser-derived parsers)
//

#ifndef INDRI_SENTENCESEGMENTERTRANSFORMATION_HPP
#define INDRI_SENTENCESEGMENTERTRANSFORMATION_HPP

#include "indri/Buffer.hpp"
#include "indri/Transformation.hpp"

#define MIN_SENTENCE_TOKENS  ( 3 )
#define MAX_SENTENCE_TOKENS  ( 100 )

class SentenceSegmenterTransformation : public Transformation {
private:
  Buffer buffer;
  ObjectHandler<ParsedDocument>* _handler;

protected:
  void addSentenceTag( ParsedDocument* document, int begin, int end );
  size_t getStartPos( ParsedDocument* document );
  int numSentences;

public:
  ParsedDocument* transform( ParsedDocument* document );

  void setHandler( ObjectHandler<ParsedDocument>& handler );
  void handle( ParsedDocument* document );
};

#endif // INDRI_SENTENCESEGMENTERTRANSFORMATION_HPP
