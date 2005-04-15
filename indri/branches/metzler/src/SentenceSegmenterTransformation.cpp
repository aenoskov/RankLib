//
// SentenceSegmenterTransformation
//
// 12 October 2004 -- dam
//

#include "indri/Buffer.hpp"
#include "indri/SentenceSegmenterTransformation.hpp"

ParsedDocument* SentenceSegmenterTransformation::transform( ParsedDocument* document ) {
  numSentences = 0;
  buffer.clear();

  int numTerms = document->positions.size();
  size_t sentenceBegin = getStartPos( document );
  bool addTag = false;

  std::string prevTerm = "";
  std::string curTerm = "";
  std::string nextTerm = "";

  //std::string sentence = "";

  int numTags = document->tags.size();

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

    //std::cout << "i: " << i << ", curTerm = " << curTerm << std::endl;

    int curTermLength = curTerm.length();
    char lastChar = '\0';
    while( curTermLength > 0 ) {
      char tmpChar = curTerm[ curTermLength - 1 ];
      if( tmpChar != '\"' && tmpChar != '\'' && tmpChar != '`' && tmpChar != ' ' &&
	  tmpChar != '<'  && tmpChar != '>') {
	lastChar = tmpChar;
	break;
      }
      curTermLength--;
    }

    if( lastChar == '?' || lastChar == '!' || lastChar == ';' ) {
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
      addSentenceTag( document, sentenceBegin, i + 1 );
      
      sentenceBegin = i + 1; // new beginning of sentence position
    }

    prevTerm = curTerm;
    curTerm = nextTerm;
  }

  // add tag to enclose the remaining data (if any)
  if( numTerms - sentenceBegin > 0 )
    addSentenceTag( document, sentenceBegin, numTerms );

  // add metadata about the number of
  // sentences in this document
  char ns[1024];
  sprintf( ns, "%d", numSentences );
  char *numSentencesPoint = buffer.write( strlen(ns) + 1 );
  strcpy( numSentencesPoint, ns );
  
  MetadataPair pair;
  pair.key = "numsentences";
  pair.value = numSentencesPoint;
  pair.valueLength = strlen( ns ) + 1;
  document->metadata.push_back( pair );

  return document;
}

void SentenceSegmenterTransformation::addSentenceTag( ParsedDocument* document, int begin, int end ) {
  TagExtent tag;
  tag.name = "sentence";
  tag.begin = begin;
  tag.end = end;
  tag.number = 0;

  assert( tag.begin <= tag.end );
  assert( tag.begin <= document->terms.size() );
  assert( tag.end   <= document->terms.size() );
  
  if( MIN_SENTENCE_TOKENS < ( end - begin ) && ( end - begin ) < MAX_SENTENCE_TOKENS ) {
    document->tags.push_back( tag );
    numSentences++;
  }
}

size_t SentenceSegmenterTransformation::getStartPos( ParsedDocument* document ) {
  int numTags = document->tags.size();

  for( int i = 0; i < numTags; i++ ) {
    TagExtent tag = document->tags[ i ];
    if( !strcmp( tag.name, "text" ) )
      return tag.begin;
  }

  // by default start at position 0
  return 0;
}

void SentenceSegmenterTransformation::setHandler( ObjectHandler<ParsedDocument>& handler ) {
  _handler = &handler;
}

void SentenceSegmenterTransformation::handle( ParsedDocument* document ) {
  _handler->handle( transform( document ) );
}
