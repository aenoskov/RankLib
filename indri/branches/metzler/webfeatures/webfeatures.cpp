
//
// webfeatures
//
// 24 July 2005 -- dam
//
// application to extract a number of features from a web corpus
//

#include <time.h>
#include "indri/Parameters.hpp"

#include "lemur/Exception.hpp"
#include "indri/WebFeatures.hpp"

int main( int argc, char** argv ) {
  try {
    indri::api::Parameters& parameters = indri::api::Parameters::instance();
    parameters.loadCommandLine( argc, argv );

    if( parameters.get( "version", 0 ) ) {
      std::cout << INDRI_DISTRIBUTION << std::endl;
    }

    std::string corpusPath = parameters[ "corpus" ];
    std::string linkPath = parameters[ "links" ];

    indri::parse::WebFeatures wf( corpusPath, linkPath );
    wf.computeFeatures();
  }
  catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  }
}
