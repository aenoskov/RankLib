
//
// similarity
//
// 22 February 2006 -- tds
//

#include "lemur/Exception.hpp"
#include "indri/ClusterEnvironment.hpp"
#include "indri/Parameters.hpp"

#include "indri/TopKMatrixReader.hpp"
#include "indri/TopKMatrixWriter.hpp"
#include "indri/TopKMatrixCombiner.hpp"

#include "indri/MatrixWriter.hpp"
#include "indri/MatrixReader.hpp"

//
// document_list
//

static std::vector<int> document_list( indri::api::Parameters parameters ) {
  std::vector<int> result;

  if( parameters.exists( "start" ) && parameters.exists( "end" ) ) {
    int start = (int) parameters["start"];
    int end = (int) parameters["end"];

    for ( int i=start; i<end; i++ ) {
      result.push_back(i);
    }
  } else {
    indri::api::Parameters docs = parameters["doc"];

    for( int i=0; i<docs.size(); i++ ) {
      result.push_back( (int)docs[i] );
    }
  }

  return result;
}


//
// text_topk_matrix
//

static void text_topk_matrix( const std::string& filename ) {
  indri::similarity::TopKMatrixReader reader;
  reader.open( filename );

  while( !reader.finished() ) {
    reader.readRow();

    const indri::utility::greedy_vector< std::pair<int, double> >& row = reader.currentRow();
    int document = reader.currentDocument();

    for( int i=0; i<row.size(); i++ ) {
      std::cout << document << " " << row[i].first << " " << row[i].second << std::endl;
    }
  }
}

//
// text_matrix
//

static void text_matrix( const std::string& filename ) {
  indri::similarity::MatrixReader reader;
  reader.open( filename );
  indri::similarity::Matrix* m = reader.read();
  reader.close();

  for( int i=0; i<m->rows(); i++ ) {
    for( int j=0; j<m->columns(); j++ ) {
      std::cout << m->get(i,j) << std::endl;
    }
  }

  delete m;
}

//
// topk_combine
//

static void topk_combine( indri::api::Parameters& p ) {
  std::vector<indri::similarity::TopKMatrixReader*> readers;
  indri::api::Parameters inputs = p["input"];
  std::string outputName = (std::string) p["output"];
  int k = p.get( "k", 10 );

  for( int i=0; i<inputs.size(); i++ ) {
    std::string inputName = inputs[i];
    indri::similarity::TopKMatrixReader* reader = new indri::similarity::TopKMatrixReader();

    reader->open( inputName );
    readers.push_back( reader );
  }

  indri::similarity::TopKMatrixCombiner combiner( readers, k );
  combiner.open( outputName );
  combiner.combine();
  combiner.close();
}

//
// topk_matrix
//

static void topk_matrix( indri::api::Parameters& parameters, indri::similarity::ClusterEnvironment& env ) {
  std::vector<indri::similarity::TopKMatrixReader*> readers;
  int k = parameters.get( "k", 10 );

  std::vector<int> src = document_list( parameters["source"] );
  std::vector<int> dest = document_list( parameters["dest"] );
  indri::similarity::Matrix* matrix = env.compareDocuments( src, dest );

  indri::similarity::TopKMatrixWriter mw(k);
  mw.open( parameters["output"] );
  mw.write( matrix, src, dest );
  mw.close();
  
  delete matrix;
}

//
// assign_documents_to_centers
//

static void assign_documents_to_centers( indri::api::Parameters p, indri::similarity::ClusterEnvironment* env ) {
  std::vector<int> docs = document_list( p["source"] );
  std::vector<int> centers = document_list( p["centers"] );
  std::vector<int> assignments = env->mostSimilarDocuments( docs, centers );
  assert( assignments.size() == docs.size() );

  for( int i=0; i<assignments.size(); i++ ) {
    std::cout << docs[i] << " " << assignments[i] << std::endl;
  }
}

//
// main
//

//
// Stuff main should do:
//    compute the similarity matrix of source to dest
//    output that dest matrix as text or binary
//    read a binary matrix, convert to text
//    
//    compute a top-k matrix (source to dest)
//    print matrix as text or binary (text assignment is a special case)
//    combine binary top-k matrices
//
//    compute new centroid for a cluster
//

void require( indri::api::Parameters param, const std::string& p ) {
  if( !param.exists(p) )
    LEMUR_THROW( LEMUR_RUNTIME_ERROR, "similarity requires parameter '" + p + "'." );
}

int main( int argc, char** argv ) {
  try {
    indri::api::Parameters& parameters = indri::api::Parameters::instance();
    parameters.loadCommandLine( argc, argv );
    indri::similarity::ClusterEnvironment* env = new indri::similarity::ClusterEnvironment();
    require( parameters, "index" );
    require( parameters, "operation" );

    std::string indexName = parameters["index"];
    std::string operation = parameters["operation"];

    if( parameters.exists( "comparison" ) ) {
      env->setComparisonMethod( parameters["comparison"] );
    }

    env->openRead( indexName );

    if( operation == "medoid" ) {
      // find the center document
      std::vector<int> docs = document_list( parameters );
      std::cout << env->medoid( docs ) << std::endl;
    } else if( operation == "assign" ) {
      // assign documents to centers
      assign_documents_to_centers( parameters, env );
    } else if( operation == "matrix" ) {
      require( parameters, "source" );
      require( parameters, "dest" );
      require( parameters, "output" );

      std::vector<int> src = document_list( parameters["source"] );
      std::vector<int> dest = document_list( parameters["dest"] );
      indri::similarity::Matrix* matrix = env->compareDocuments( src, dest );

      indri::similarity::MatrixWriter mw;
      mw.open( parameters["output"] );
      mw.write( matrix );
      mw.close();

      delete matrix;
    } else if( operation == "topkmatrix" ) {
      topk_matrix( parameters, *env );
    } else if( operation == "combine" ) {
      topk_combine( parameters );
    } else if( operation == "textmatrix" ) {
      std::string input = parameters["input"];
      text_matrix( input );
    } else if( operation == "texttopk" ) {
      std::string input = parameters["input"];
      text_topk_matrix( input );
    }

    env->close();
    delete env;
  } catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  }

  return 0;
}
