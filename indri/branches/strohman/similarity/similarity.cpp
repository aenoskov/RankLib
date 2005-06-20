
//
// similarity
//
// Compute similarity scores between documents or passages in
// an Indri index and output a binary file.
// 
// This application computes similarity as KL-divergence or 
// with the Fisher diffusion kernel.
//

#include <math.h>
#include <set>
#include <vector>
#include <algorithm>
#include <time.h>

#include "lemur/IndexTypes.hpp"
#include "indri/Repository.hpp"
#include "indri/greedy_vector"
#include "indri/count_iterator"
#include "lemur/Exception.hpp"

struct statistics_t {
  ~statistics_t() {
  }

  UINT64 total;
  UINT64 documents;
  indri::utility::greedy_vector<UINT64> counts;
};

typedef indri::utility::greedy_vector< std::pair< int, int > > VCounts;
typedef double (*similarity_function)( VCounts& one, VCounts& two, statistics_t* stats );

//
// Fisher kernel: exp( t^{-1} arccos^{2} ( sqrt( theta_i ) <dot> sqrt( theta_j ) )
//
// KL-divergence (x entropy): - \sum_i p_i log p_i / q_i
//

struct statistics_t* collect_statistics( indri::index::Index* index ) {
  struct statistics_t* stats = new statistics_t;
  indri::index::VocabularyIterator* iter = index->vocabularyIterator();
  
  iter->startIteration();
  
  stats->total = index->termCount();
  stats->documents = index->documentCount();
  stats->counts.reserve( index->uniqueTermCount() );
  stats->counts.push_back( 0 );

  while( !iter->finished() ) {
    indri::index::DiskTermData* entry = iter->currentEntry();
    indri::index::TermData* termData = entry->termData;

    stats->counts.push_back( termData->corpus.documentCount );
    iter->nextEntry();
  }

  delete iter;
  
  return stats;
}

//
// convert_document_vector
//

static void convert_document_vector( VCounts& counts, const indri::index::TermList& termList ) {
  indri::utility::greedy_vector<lemur::api::TERMID_T> termIDs = termList.terms();
  indri::utility::count_iterator<lemur::api::TERMID_T> iter( termIDs.begin(), termIDs.end() );

  // length stored in termID 0
  counts.push_back( std::make_pair( 0, termIDs.size() ) );
  
  for( ; iter != termIDs.end(); ++iter ) {
    //assert( (*iter).object >= 0 );
    
    if( (*iter).object > 0 ) {
      counts.push_back( std::make_pair( (*iter).object, 
                                        (*iter).count ) );
    }
  }
}

//
// fisher_diffusion
//

static double fisher_diffusion( VCounts& one, VCounts& two, statistics_t* statistics ) {
  int i=1;
  int j=1;
  
  double lambda = 0.25;
  double oneLength = 1.0 / double(one[0].second);
  double twoLength = 1.0 / double(two[0].second);
  double dotProduct = lambda;
  double collectionLength = statistics->total;
  double oneOverT = 1./10.;
  
  while( i < one.size() && j < two.size() ) {
    int oneTerm = one[i].first;
    int twoTerm = two[j].first;
    
    if( oneTerm > statistics->counts.size() || twoTerm > statistics->counts.size() ) {
      i++; j++;
      continue;
    }
    
    assert( oneTerm > 0 && twoTerm > 0 );
    
    if( oneTerm == twoTerm ) {
      // terms match
      double oneFraction = double(one[i].second) * oneLength;
      double twoFraction = double(two[j].second) * twoLength;
      double background = statistics->counts[ oneTerm ] * collectionLength;
      
      dotProduct += sqrt( ((1. - lambda) * oneFraction + lambda * background) * 
                          ((1. - lambda) * twoFraction + lambda * background) ) -
                    lambda * background;
      
      i++;
      j++;
    } else if( oneTerm < twoTerm ) {
      double oneFraction = one[i].second * oneLength;
      double background = statistics->counts[ oneTerm ] * collectionLength;

      dotProduct += sqrt( ((1. - lambda) * oneFraction + lambda * background) *
                          (lambda * background) ) -
                    lambda * background;
      
      i++;
    } else /* twoTerm < oneTerm */ {
      double twoFraction = two[j].second * twoLength;
      double background = statistics->counts[ twoTerm ] * collectionLength;

      dotProduct += sqrt( (lambda * background) *
                          ((1 - lambda) * twoFraction + lambda * background) ) -
                    lambda * background;
      
      j++;
    }
  }

  for( ; i < one.size(); i++ ) { 
    int oneTerm = one[i].first;
    assert( oneTerm >= 0 );

    if( oneTerm > statistics->counts.size() ) {
      i++;
      continue;
    }

    double oneFraction = one[i].second * oneLength;
    double background = statistics->counts[ oneTerm ] * collectionLength;
    
    dotProduct += sqrt( ((1. - lambda) * oneFraction + lambda * background) *
                        (lambda * background) ) -
                  lambda * background;
  }
  
  for( ; j < two.size(); j++ ) {
    int twoTerm = two[j].first;
    assert( twoTerm >= 0 );
    
    if( twoTerm > statistics->counts.size() ) {
      j++;
      continue;
    }
    
    double twoFraction = two[j].second * twoLength;
    double background = statistics->counts[ twoTerm ] * collectionLength;
    
    dotProduct += sqrt( ((1. - lambda) * twoFraction + lambda * background) *
                        (lambda * background) ) -
                  lambda * background;
  }
  
  double ac = acos( dotProduct );
  return exp(ac*ac*oneOverT);
}

//
// kl_divergence (unimpl)
//

static double kl_divergence( VCounts& one, VCounts& two, statistics_t* statistics, double lambda ) {
  return 0;
}

//
// count_leading_spaces
//

int count_leading_spaces( char* c ) {
  int spaces = 0;
  
  while( c[spaces] == ' ' )
    spaces++;
  
  return spaces;
}

//
// read_document_ids
//
// 1049
//   5948
//     04938
//   50948
//     50984
//     4938
//   989

struct document_t {
  int id;
  VCounts counts;
  std::vector< document_t* > children;
};

static std::vector< document_t* > read_documents( indri::index::Index* index, const std::string& documentPath ) {
  // statistics
  statistics_t* stats = collect_statistics( index );

  // input text
  std::ifstream in;
  char line[100];

  // docs
  std::vector< document_t* > documents;
  std::stack< document_t* > docStack;
  document_t* document;  
  
  in.open( documentPath.c_str(), std::iostream::in );

  while( !in.eof() ) {
    in.getline( line, sizeof line );
    int spaces = count_leading_spaces( line );
    
    while( spaces < docStack.size() )
      docStack.pop();
    
    document = new document_t;
    
    if( docStack.size() )
      docStack.top()->children.push_back( document );
    else
      documents.push_back( document );
    
    docStack.push( document );
    
    document->id = atoi( line + spaces );
    
    const indri::index::TermList* termList = index->termList( document->id );
    convert_document_vector( document->counts, *termList );
    
    delete termList;
  }
                
  delete stats;

  in.close();
  return documents;
}

//
// partition
//

static void partition( const std::string& outputPath,
                       indri::index::Index* index,
                       std::vector< document_t* >& centroids,
                       similarity_function func,
                       int startDocID,
                       int endDocID ) {
  std::ofstream output;
  output.open( outputPath.c_str() );
  
  // part
  output << 'P';
  output << 'A';
  output << 'R';
  output << 'T';
  
  // documents
  output << startDocID;
  output << endDocID;
  
  // centroids
  indri::index::TermListFileIterator* iterator = index->termListFileIterator();
  VCounts document;
  
  // statistics
  statistics_t* stats = collect_statistics( index );
  
  iterator->startIteration();
  
  while( !iterator->finished() ) {
    convert_document_vector( document, *(iterator->currentEntry()) );
    
    document_t* best = 0;
    double bestSimilarity = 0;
    std::vector< document_t* >& centroidLevel = centroids; 
    
    do {
      document_t* bestLevel = 0;
      double bestLevelSimilarity = 0;

      for( int i=0; i < centroidLevel.size(); i++ ) {
        double sim = func( document, centroidLevel[i]->counts, stats );

        if( sim > bestLevelSimilarity ) {
          bestLevel = centroidLevel[i];
          bestLevelSimilarity = sim;
          
          if( sim > bestSimilarity ) {
            bestSimilarity = sim;
            best = bestLevel;
          }
        }
      }
      
      centroidLevel = bestLevel->children;
    }
    while( centroidLevel.size() );
    
    output << best->id;
  }
  
  delete iterator;
  output.close();
}

//
// full_similarity
//

static void full_similarity( const std::string& outputPath, indri::index::Index* index, std::vector< document_t* >& documents, similarity_function func ) {

  // output file
  std::ofstream output;
  output.open( outputPath.c_str() );
  
  // statistics
  statistics_t* stats = collect_statistics( index );
  
  // write full dword
  output << 'F';
  output << 'U';
  output << 'L';
  output << 'L';
  
  // write docs
  output << documents.size();
  
  // write docIDs
  for( int i=0; i < documents.size(); i++ ) {
    output << documents[i];
  }
  
  // for cache purposes, it's probably better to do this in strides
  for( int i=0; i < documents.size(); i++ ) {
    for( int j=i; j < documents.size(); j++ ) {
      double similarity = func( documents[i]->counts, documents[j]->counts, stats );
      output << similarity;
    }
  }
  
  output.close();
}

//
// random_set
//

static std::vector<int> random_set( int size, int maximum ) {
  std::vector<int> v;
  std::set<int> s;
  
  while( v.size() < size ) {
    int r = rand() % maximum;
    
    if( s.find(r) == s.end() ) {
      v.push_back(r);
      s.insert(r);
    } 
  }
  
  std::sort( v.begin(), v.end() );
  
  return v;
}

//
// print_spaces
//

static void print_spaces( int spaces ) {
  for( int i=0; i<spaces; i++ )
    std::cout << ' ';
}

//
// print_documents
//

static void print_documents( std::vector< document_t* >& documents, int indent ) {
  for( int i=0; i<documents.size(); i++ ) {
    print_spaces( indent );
    std::cout << documents[i]->id << std::endl;
    
    if( documents[i]->children.size() )
      print_documents( documents[i]->children, indent+1 );
  }
}

void hierarchy( std::vector< document_t* >& output, std::vector< document_t* >& input, statistics_t* stats, similarity_function func, int roots ) {
  // random set
  std::vector<int> rootSet = random_set( roots, input.size() );
  std::cout << "picked " << rootSet.size() << " roots" << std::endl;

  // push the roots into the output set
  for( int i=0; i<rootSet.size(); i++ ) {
    output.push_back( input[ rootSet[i] ] );
  }
  
  std::cout << "starting to cluster" << std::endl;
  // cluster the rest of inputs into the appropriate output sets
  for( int i=0; i<input.size(); i++ ) {
    double similarity = 0;
    document_t* document = input[i];
    document_t* best = 0;
    double bestSimilarity = 0;
    
    for( int j=0; j<output.size(); j++ ) {
      document_t* centroid = output[j];
      
      if( centroid == document ) {
        best = centroid;
        break;
      }

      similarity = func( document->counts, centroid->counts, stats );
      
      if( similarity > bestSimilarity ) {
        bestSimilarity = similarity;
        best = centroid;
      }
    }

    if( best != document ) {
      best->children.push_back( document );
    }

    std::cout << "#";

    if( (i % 50) == 0 )
      std::cout << std::endl;
  }
  std::cout << "finished clustering children of roots" << std::endl;
  
  // now we need to recurse into the children sets, choosing root sets as we go
  for( int i=0; i<output.size(); i++ ) {
    if( output[i]->children.size() <= roots )
      continue; // no need to recurse when there isn't anything left to cluster
    
    std::vector< document_t* > cluster = output[i]->children;
    output[i]->children.clear();
    
    std::cout << "recursing into child" << std::endl;
    hierarchy( output[i]->children, cluster, stats, func, roots );
  }
}

//
// hierarchy
//

void hierarchy( indri::index::Index* index, std::vector< document_t* >& documents ) {
  // statistics
  statistics_t* stats = collect_statistics( index );
  
  // build hierarchy
  std::vector< document_t* > output;
  hierarchy( output, documents, stats, fisher_diffusion, 10 );
  
  // print hierarchy
  print_documents( output, 0 );
}

//
// main
//

int main( int argc, char** argv ) {
  // three modes:
  //   build a centroid hierarchy
  //   compare all to a set of centroids
  //   do full similarity matching
  srand( time(0) );
  
  char ch;
  
  int startDocID = 0;
  int endDocID = -1;
  std::string repositoryPath;
  std::string documentsPath;
  std::string outputPath;
  
  bool hierarchyMode = false;
  bool partitionMode = false;
  bool fullSimilarityMode = false;
  
  while( (ch = getopt( argc, argv, "hpfd:s:e:r:o:" )) != -1 ) {
    switch(ch) {
      case 'r': // repository path
        repositoryPath = optarg; break;
        
      case 'd': // documents path
        documentsPath = optarg; break;
        
      case 'e': // end docID
        endDocID = atoi(optarg); break;
        
      case 's': // start docID
        startDocID = atoi(optarg); break;
        
      case 'h': // hierarchyMode
        hierarchyMode = true; break;
        
      case 'p': // partitionMode
        partitionMode = true; break;
        
      case 'f': // full similarty
        fullSimilarityMode = true; break;
        
      case 'o':
        outputPath = optarg; break;
    }
  }
  
  if( !( hierarchyMode || partitionMode || fullSimilarityMode ) )
    std::cout << "Must pick at least one mode (hierarchy, partition, full)" << std::endl;
  
  indri::collection::Repository repository;
  std::vector< document_t* > documents;
  
  try {
    // open repository
    repository.openRead( repositoryPath );
  
    // fetch index 
    indri::collection::Repository::index_state state = repository.indexes();
    indri::index::Index* index = (*state)[0];

    // read documents
    documents = read_documents( index, documentsPath );
    
    if( hierarchyMode ) {
      hierarchy( index, 
                 documents );
    } else if( partitionMode ) {
      partition( outputPath,
                 index,
                 documents,
                 fisher_diffusion,
                 startDocID,
                 endDocID );
    } else {
      full_similarity( outputPath,
                       index,
                       documents,
                       fisher_diffusion );
    }
  
    repository.close();
  } catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  }

  return 0;
}
