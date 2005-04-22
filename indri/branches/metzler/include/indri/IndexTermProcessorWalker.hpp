//
// IndexTermProcessorWalker
//
// 25 October 2004 -- dam
//
// Stems and stops all IndexTerm nodes.
// This allows us to find RawScorerNodes
// with the same queryText.

#include "indri/Repository.hpp"

#ifndef INDRI_INDEXTERMPROCESSORWALKER_HPP
#define INDRI_INDEXTERMPROCESSORWALKER_HPP
namespace indri
{
  namespace lang
  {

    class IndexTermProcessorWalker : public indri::lang::Walker {
    private:
      indri::collection::Repository& _repository;
      
    public:
      IndexTermProcessorWalker( indri::collection::Repository& repository ) : _repository(repository) {}
      ~IndexTermProcessorWalker() {}
      
      void after( indri::lang::IndexTerm* termNode ) {
	bool stopword = false;
	std::string originalText = termNode->getText();
	std::string processedText = originalText;
	int termID = 0;
	
	// stem and stop the word
	if( termNode->getStemmed() == false ) {
	  processedText = _repository.processTerm( originalText );
	  stopword = processedText.length() == 0;
	}
	
	if( !stopword ) {
	  //termID = _repository.index()->term( processedText.c_str() );
	  processedText = _repository.processTerm( processedText );
	  
	  if( processedText.length() == 0 )
	    processedText = originalText;
	  else
	    termNode->setStemmed( true );
	}
	else 
	  processedText = originalText;
	
	termNode->setText( processedText );          	
      }
    };
  }
}
    
#endif // INDRI_INDEXTERMPROCESSORWALKER_HPP

