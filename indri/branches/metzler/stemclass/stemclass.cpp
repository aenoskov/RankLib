#include <stdio.h>
#include "indri/Repository.hpp"
#include "indri/Parameters.hpp"
#include "lemur/Exception.hpp"
#include "indri/KrovetzStemmerTransformation.hpp"
#include "indri/PorterStemmerTransformation.hpp"

int main( int argc, char** argv ) {
  try {
    indri::api::Parameters& param = indri::api::Parameters::instance();
    param.loadCommandLine( argc, argv );

    if( !param.exists( "index" ) ) {
      LEMUR_THROW( LEMUR_RUNTIME_ERROR, "Need an index parameter." );
    }

    // make some stemmers
    indri::parse::KrovetzStemmerTransformation kstemmer( param );
    indri::parse::PorterStemmerTransformation pstemmer;
    
    std::string index = (std::string) param["index"];
    indri::collection::Repository repository;
    repository.openRead( index );
   
    indri::collection::Repository::index_state state = repository.indexes();
    
    printf( "# base porter krovetz\n" );
    
    // for each index in state
    // extract all words
    // stem all of them too
    for( int i=0; i<state->size(); i++ ) {
      indri::index::Index* index = (*state)[i];
      indri::index::VocabularyIterator* iterator = index->vocabularyIterator();
      indri::api::ParsedDocument document;      
      
      iterator->startIteration();
      
      while( !iterator->finished() ) {
        std::string term = iterator->currentEntry()->termData->term;
        char termBuffer[1000];

        strcpy( termBuffer, term.c_str() );
        document.terms.clear();
        document.terms.push_back( termBuffer );
        
        std::string kterm = kstemmer.transform( &document )->terms[0];

        strcpy( termBuffer, term.c_str() );
        document.terms.clear();
        document.terms.push_back( termBuffer );
                
        std::string pterm = pstemmer.transform( &document )->terms[0];

        std::cout << term << "\t" << pterm << "\t" << kterm << std::endl;
        iterator->nextEntry();
      } 
      
      delete iterator;     
    }
    
    repository.close();
  } catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  }

  return 0;
}
