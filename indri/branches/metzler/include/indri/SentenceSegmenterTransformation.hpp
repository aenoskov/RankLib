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
namespace indri
{
  namespace parse
  {

    class SentenceSegmenterTransformation : public Transformation {
    private:
      indri::utility::Buffer buffer;
      ObjectHandler<indri::api::ParsedDocument>* _handler;
      
    protected:
      void addSentenceTag( indri::api::ParsedDocument* document, int begin, int end );
      size_t getStartPos( indri::api::ParsedDocument* document );
      int numSentences;
      
    public:
      indri::api::ParsedDocument* transform( indri::api::ParsedDocument* document );
      
      void setHandler( ObjectHandler<indri::api::ParsedDocument>& handler );
      void handle( indri::api::ParsedDocument* document );
    };
  }
}
    
#endif // INDRI_SENTENCESEGMENTERTRANSFORMATION_HPP
    
