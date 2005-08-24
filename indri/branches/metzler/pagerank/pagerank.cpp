
//
// pagerank
//
// 16 July 2005 -- dam
//
// application to compute PageRank using output from harvestlinks application
//

#include <time.h>
#include "indri/Parameters.hpp"

#include "lemur/Exception.hpp"
#include "indri/PageRank.hpp"

int main( int argc, char** argv ) {
  try {
    indri::api::Parameters& parameters = indri::api::Parameters::instance();
    parameters.loadCommandLine( argc, argv );

    if( parameters.get( "version", 0 ) ) {
      std::cout << INDRI_DISTRIBUTION << std::endl;
    }

    std::string corpusPath = parameters[ "corpus" ];
    std::string linkPath = parameters[ "links" ];
    std::string outputFile = parameters[ "output" ];

    int docsPerIter = parameters.get( "docs", 1000 );
    int maxIters = parameters.get( "iters", 10 );
    double c = parameters.get( "c", 0.85 );

    indri::parse::PageRank pr( corpusPath, linkPath );
    pr.computePageRank( outputFile, maxIters, docsPerIter, c );

    if( parameters.get( "printPageRanks", 0 ) )
      indri::parse::PageRank::printSavedPageRanks( outputFile );
  }
  catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  }
}
