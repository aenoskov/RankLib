
//
// similarity
//
// June 2005 -- tds
//
// Compute similarity scores between documents in
// an Indri index and output a binary file.
// 
// This application computes similarity as KL-divergence or 
// with the Fisher diffusion kernel.
//
//

#include <math.h>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>
#include <time.h>

#include "lemur/IndexTypes.hpp"
#include "indri/Repository.hpp"
#include "indri/greedy_vector"
#include "indri/count_iterator"
#include "lemur/Exception.hpp"
#include "indri/File.hpp"
#include "indri/SequentialReadBuffer.hpp"
#include "indri/SequentialWriteBuffer.hpp"

struct statistics_t {
  ~statistics_t() {
  }

  UINT64 total;
  UINT64 documents;
  indri::utility::greedy_vector<UINT64> counts;
};

struct pair_second_greater {
  bool operator () ( const std::pair< int, double >& one, const std::pair< int, double >& two ) const {
    return one.second > two.second || ( one.second == two.second && one.first > two.first );
  }
};

class ScoreAggregator {
public:
  virtual void operator () ( int x, int y, double score ) = 0;
};

class TopKAggregator : public ScoreAggregator {
private:
  std::vector< std::vector< std::pair< int, double > > > _scores;
  int _k;
  
public:
  TopKAggregator( int xCount, int yCount, int k ) {
    _k = k;
    _scores.resize( xCount );
  }
  
  void _moveSmallestToFront( int x ) {
    std::vector< std::pair< int, double > >& data = _scores[x];
    std::pair< int, double > small = std::make_pair( 0, DBL_MAX );
  
    for( int i=0; i<data.size(); i++ ) {
      if( data[i].second < small.second ) {
        small.second = data[i].second;
        small.first = i;
      }
    }

    // swap the smallest into the first position
    int smallDocument = data[ small.first ].first;
    double smallScore = data[ small.first ].second;
    
    data[small.first] = data[0];
    data[0].first = smallDocument;
    data[0].second = smallScore;
  }
  
  void _store( int x, int y, double score ) {
    if( _scores[x].size() < _k ) {
      _scores[x].push_back( std::make_pair( y, score ) );
      _moveSmallestToFront( x );
    } else {
      if( _scores[x][0].second < score ) {
        // if this one is bigger than the smallest current score,
        // replace the smallest score with this one
        _scores[x][0] = std::make_pair<int, double>( y, score );
        _moveSmallestToFront( x );
      }
    }
  }

  void operator () ( int x, int y, double score ) {
    _store( x, y, score );
  }
  
  const std::vector< std::vector< std::pair< int, double > > >& getScores( ) const {
    return _scores;
  }
};

typedef indri::utility::greedy_vector< std::pair< int, double > > VCounts;
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
// extract_field_extents
//

static void extract_field_extents( indri::utility::greedy_vector< indri::index::Extent >& extents, const indri::index::TermList& termList, int field ) {
  // determine the extents that we want to count
  if( field < 0 ) {
    // all fields are requested
    indri::index::Extent extent( 0, termList.terms().size() );
    extents.push_back( extent );
  } else {
    // only one kind of field is requested (we assume non-overlapping fields here)
    indri::utility::greedy_vector< indri::index::FieldExtent >::const_iterator iter;    

    for( iter = termList.fields().begin(); iter < termList.fields().end(); iter++ ) {
      if( (*iter).id == field ) {
        indri::index::Extent extent( (*iter).begin, (*iter).end );
        extents.push_back( extent );
      }
    }
  }
}

//
// convert_document_vector
//

static void convert_document_vector( VCounts& counts, const indri::index::TermList& termList, int field ) {
  indri::utility::greedy_vector<lemur::api::TERMID_T> termIDs;
  const indri::utility::greedy_vector<lemur::api::TERMID_T>& termListTerms = termList.terms();
  indri::utility::greedy_vector< indri::index::Extent > extents;
  counts.clear();

  extract_field_extents( extents, termList, field );

  // iterate through all extents
  for( int i=0; i < extents.size(); i++ ) {
    for( int j = extents[i].begin; j < extents[i].end; j++ ) {
      if( termListTerms[j] != 0 )
        termIDs.push_back( termListTerms[j] );
    }
  }

  indri::utility::count_iterator<lemur::api::TERMID_T> iter( termIDs.begin(), termIDs.end() );
  double length = termListTerms.size();

  // run through the termIDs, get frequency for each  
  for( ; iter != termIDs.end(); ++iter ) {
    if( (*iter).object > 0 ) {
      counts.push_back( std::make_pair( (*iter).object, 
                                        double((*iter).count) / length ) );
    }
  }
}

//
// dot_product_sqrt
//

static double dot_product_sqrt( VCounts& one, VCounts& two, statistics_t* statistics ) {
  int i=0;
  int j=0;
  double dotProduct = 0;
  
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
      dotProduct += sqrt( one[i].second * two[j].second );
      i++; j++;
    } else if ( oneTerm < twoTerm ) {
      i++;
    } else {
      j++;
    }
  }

  return dotProduct;
}

//
// fisher_diffusion
//

static double fisher_diffusion( VCounts& one, VCounts& two, statistics_t* statistics ) {
  int i=1;
  int j=1;
  
  double lambda = 0.25;
  double oneMinusLambda = 1.0 - lambda;
  double oneLength = 1.0 / double(one[0].second);
  double twoLength = 1.0 / double(two[0].second);
  double dotProduct = lambda;
  double collectionLength = statistics->total;
  double oneOverCollectionLength = 1.0 / collectionLength;
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
      double background = statistics->counts[ oneTerm ] * oneOverCollectionLength;
      
      dotProduct += sqrt( (oneMinusLambda * oneFraction + lambda * background) * 
                          (oneMinusLambda * twoFraction + lambda * background) ) -
                    lambda * background;
      
      i++;
      j++;
    } else if( oneTerm < twoTerm ) {
      double oneFraction = one[i].second * oneLength;
      double background = statistics->counts[ oneTerm ] * oneOverCollectionLength;

      dotProduct += sqrt( (oneMinusLambda * oneFraction + lambda * background) *
                          (lambda * background) ) -
                    lambda * background;
      
      i++;
    } else /* twoTerm < oneTerm */ {
      double twoFraction = two[j].second * twoLength;
      double background = statistics->counts[ twoTerm ] * oneOverCollectionLength;

      dotProduct += sqrt( (lambda * background) *
                          (oneMinusLambda * twoFraction + lambda * background) ) -
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
    
    dotProduct += sqrt( (oneMinusLambda * oneFraction + lambda * background) *
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
    
    dotProduct += sqrt( (oneMinusLambda * twoFraction + lambda * background) *
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

//
// read_documents
//

static std::vector< document_t* > read_documents( indri::index::Index* index, int field = -1 ) {
  // docs
  std::vector< document_t* > documents;
  document_t* document; 
  UINT64 length = 0; 
  
  for( int i=1; i<index->documentCount(); i++ ) {
    document = new document_t;
    documents.push_back( document );
    document->id = i;
    
    const indri::index::TermList* termList = index->termList( document->id );
    convert_document_vector( document->counts, *termList, field );
    length += document->counts.size();
    
    delete termList;
  }
  
  return documents;
}

static std::vector< document_t* > read_documents( indri::index::Index* index, const std::string& documentPath, int field = -1 ) {
  if( documentPath.size() == 0 ) {
    return read_documents( index );
  }

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
  
  while( !in.eof() && in.good() ) {
    in.getline( line, sizeof line );
    int spaces = count_leading_spaces( line );
    
    while( spaces < docStack.size() )
      docStack.pop();
    
    document = new document_t;
    document->id = atoi( line + spaces );
    
    if( document->id == 0 ) {
      delete document;
      break;
    }
    
    if( docStack.size() )
      docStack.top()->children.push_back( document );
    else
      documents.push_back( document );
    
    docStack.push( document );
    
    const indri::index::TermList* termList = index->termList( document->id );
    convert_document_vector( document->counts, *termList, field );
    
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
                       int overlap,
                       int startDocID,
                       int endDocID,
                       int field = -1 ) {
  std::ofstream output;
  output.open( outputPath.c_str(), std::ofstream::out );
  int documentID = startDocID;
  
  // centroids
  indri::index::TermListFileIterator* iterator = index->termListFileIterator();
  VCounts document;
  
  // statistics
  statistics_t* stats = collect_statistics( index );
  
  iterator->startIteration();
  iterator->nextEntry( startDocID );
  
  if( endDocID < 0 )
    endDocID = index->documentCount();
  
  while( !iterator->finished() && documentID <= endDocID ) {
    convert_document_vector( document, *(iterator->currentEntry()), field );
    
    document_t* best = 0;
    double bestSimilarity = 0;
    std::priority_queue< std::pair< double, int > > scores;
    
    for( int i=0; i < centroids.size(); i++ ) {
      double sim = func( document, centroids[i]->counts, stats );
      scores.push( std::make_pair( sim, centroids[i]->id ) );
    }
    
    output << documentID << " ";

    for( int i=0; i<overlap; i++ ) {
      if( scores.size() ) {
        output << scores.top().second << " ";
        scores.pop();
      }
    }

    output << std::endl;
  
    documentID++;
    iterator->nextEntry();
  }
  
  delete iterator;
  output.close();
}

//
// compute_similarity_matrix
//

static void compute_similarity_matrix( indri::index::Index* index,
                                       const std::vector< document_t* >& documentsX,
                                       const std::vector< document_t* >& documentsY,
                                       similarity_function func,
                                       ScoreAggregator& aggregator,
                                       bool upperTriangle )
{
  // statistics
  statistics_t* stats = collect_statistics( index );
  
  // for cache purposes, it's best to do this in strides
  const int stride = 500;
  UINT64 count = 0;
  UINT64 lastCount = count;
  
  for( int i=0; i < documentsX.size(); i += stride ) {
    int leftStart = i;
    int leftEnd = lemur_compat::min<int>( documentsX.size(), i + stride );
    
    for( int j = 0; j < documentsY.size(); j += stride ) {
      int rightStart = j;
      int rightEnd = lemur_compat::min<int>( documentsY.size(), j + stride );
      
      for( int k = leftStart; k < leftEnd; k++ ) {
        for( int l = rightStart; l < rightEnd; l++ ) {
          if( k <= l && upperTriangle )
            continue;

          double similarity = func( documentsX[k]->counts, documentsY[l]->counts, stats );
          aggregator( k, l, similarity );
          count++;

          if( upperTriangle ) {
            aggregator( l, k, similarity );
          }
        }
      }
      
      if( lastCount != count )
        std::cout << count << std::endl;
      lastCount = count;
    }
  }

  delete stats;
}

//
// topk_similarity
//

static void topk_similarity( const std::string& outputPath,
                             indri::index::Index* index,
                             const std::vector< document_t* >& documentsX,
                             const std::vector< document_t* >& documentsY,
                             similarity_function func,
                             int k, 
                             bool upperTriangle )
{
  TopKAggregator aggregator( documentsX.size(), documentsY.size(), k );
  compute_similarity_matrix( index, documentsX, documentsY, func, aggregator, upperTriangle );

  // output file
  indri::file::File out;
  out.create( outputPath );

  indri::file::SequentialWriteBuffer* outb = new indri::file::SequentialWriteBuffer( out, 1024*1024 );

  // write full dword
  outb->write( "TOPK", 4 );
  
  // write k
  outb->write( &k, sizeof(UINT32) );
  
  const std::vector< std::vector< std::pair< int, double > > >& scores = aggregator.getScores();
  
  // write data
  for( int i=0; i < documentsX.size(); i++ ) {
    int docID = documentsX[i]->id;
    outb->write( &docID, sizeof(lemur::api::DOCID_T) );
    std::vector< std::pair< int, double > > docScores = scores[i];
    
    std::sort( docScores.begin(), docScores.end(), pair_second_greater() );

    for( int j=0; j<k; j++ ) {
      int otherID;
      double similarity;
    
      if( j < docScores.size() ) {
        otherID = documentsY[ docScores[j].first ]->id;
        similarity = docScores[j].second;
      } else {
        otherID = 0;
        similarity = 0;
      }
      
      outb->write( &otherID, sizeof(lemur::api::DOCID_T) );
      outb->write( &similarity, sizeof(double) );
    }
  }

  // write topk data
  
  outb->flush();
  delete outb;
  out.close();
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
  std::vector<int> rootSet = random_set( lemur_compat::min<int>( roots, input.size() ), input.size() );

  // push the roots into the output set
  for( int i=0; i<rootSet.size(); i++ ) {
    output.push_back( input[ rootSet[i] ] );
  }
  
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
  }
  
  // now we need to recurse into the children sets, choosing root sets as we go
  for( int i=0; i<output.size(); i++ ) {
    if( output[i]->children.size() <= roots )
      continue; // no need to recurse when there isn't anything left to cluster
    
    std::vector< document_t* > cluster = output[i]->children;
    output[i]->children.clear();
    
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
  hierarchy( output, documents, stats, dot_product_sqrt, 100 );
  
  // print hierarchy
  print_documents( output, 0 );
}

//
// combine_topk
//

struct TopKFile {
  int _k;
  UINT64 _length;
  std::string _fileName;
  indri::file::File _inFile;
  indri::file::SequentialReadBuffer* _inBuffer;
  
  int document;
  std::vector< std::pair< int, double > > list;
  
  void open( const std::string& filename ) {
    _fileName = filename;
    
    _inFile.openRead( filename );
    _inBuffer = new indri::file::SequentialReadBuffer( _inFile, 1024*128 );
    _length = _inFile.size();

    readHeader();
    read();
  }
  
  ~TopKFile() {
    close();
  }
  
  void close() {
    delete _inBuffer;
    _inBuffer = 0;
    _inFile.close();
  }
  
  void readHeader() {
    char text[4];
    _inBuffer->read( &text, 4 );
    _inBuffer->read( &_k, sizeof(UINT32) );
  }
  
  void read() {
    _inBuffer->read( &document, sizeof(UINT32) );
    list.clear();
    
    for( int i=0; i<_k; i++ ) {
      int otherDocID;
      double similarity;
      
      _inBuffer->read( &otherDocID, sizeof(UINT32) );
      _inBuffer->read( &similarity, sizeof(double) );
      
      list.push_back( std::make_pair( otherDocID, similarity ) );
    }
  }
  
  bool finished() {
    return _length <= _inBuffer->position();
  }
};

void combine_topk( const std::string& outFilename, const std::vector<std::string>& inFilenames, int k ) { 
  // open output file
  indri::file::File out;
  out.create( outFilename );
  indri::file::SequentialWriteBuffer* outb = new indri::file::SequentialWriteBuffer( out, 1024*1024 );
  
  // open input files
  std::vector<TopKFile*> files;
  
  for( int i=0; i<inFilenames.size(); i++ ) {
    TopKFile* f = new TopKFile;
    f->open( inFilenames[i] );
    files.push_back( f );
  }
  
  // write header
  outb->write( "TOPK", 4 );
  outb->write( &k, sizeof(UINT32) );

  while( files.size() ) {
    std::vector<TopKFile*> current;
  
    // find all files that are at the smallest document
    int smallDocument = MAX_INT32;
    for( int i=0; i<files.size(); i++ ) {
      if( files[i]->document < smallDocument ) {
        current.clear();
        smallDocument = files[i]->document;
        current.push_back( files[i] );
      } else if( files[i]->document == smallDocument ) {
        current.push_back( files[i] );
      }
    }
    
    // merge all useful data into one top-k list
    std::vector< std::pair< int, double > > top;
    
    for( int i=0; i<current.size(); i++ ) {
      std::copy( current[i]->list.begin(), current[i]->list.end(), std::back_inserter(top) );
    }

    std::sort( top.begin(), top.end(), pair_second_greater() );
    
    // remove duplicates
    int lastDoc = 0;
    for( int i=0; i<top.size(); i++ ) {
      if( top[i].first == lastDoc ) {
        top.erase( top.begin() + i );
        i--;
      } else {
        lastDoc = top[i].first;
      }
    }
 
    // write that data out to disk
    outb->write( &smallDocument, sizeof(UINT32) );
    
    for( int i=0; i<k; i++ ) {
      int doc = 0;
      double similarity = 0;
      
      if( i < top.size() ) {
        doc = top[i].first;
        similarity = top[i].second;
      }
      
      outb->write( &doc, sizeof(UINT32) );
      outb->write( &similarity, sizeof(double) );
    }
    
    // move to the next document
    for( int i=0; i<current.size(); i++ ) {
      if( current[i]->finished() ) {
        // file is finished, erase it from the files array
        for( int j=0; j<files.size(); j++ ) {
          if( files[j] == current[i] ) {
            files[j]->close();
            delete files[j];
            files.erase( files.begin() + j );
            break;
          }
        }
      } else {
        // read more data
        current[i]->read();
      }
    }
  }
   
  outb->flush();
  delete outb;
  out.close();
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
  
  int startDocID = 1;
  int endDocID = -1;
  int k = 10;
  int overlap = 1;
  std::string repositoryPath;
  std::string documentsXPath;
  std::string documentsYPath;
  std::string outputPath;
  std::vector<std::string> inputFiles;
  
  bool hierarchyMode = false;
  bool partitionMode = false;
  bool fullSimilarityMode = false;
  bool combineMode = false;
  
  while( (ch = getopt( argc, argv, "hpfcd:i:s:e:r:o:k:v:x:y:" )) != -1 ) {
    switch(ch) {
      case 'r': // repository path
        repositoryPath = optarg; break;
        
      case 'd': // documents path
      case 'x': // documents X path
        documentsXPath = optarg; break;

      case 'y':
        documentsYPath = optarg; break;
        
      case 'e': // end docID
        endDocID = atoi(optarg); break;
        
      case 'k':
        k = atoi(optarg); break;
        
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
        
      case 'c':
        combineMode = true; break;

      case 'v':
        overlap = atoi(optarg); break;
        
      case 'i':
        std::string filename = optarg;
        inputFiles.push_back( filename );
        break;
    }
  }
  
  if( !( hierarchyMode || partitionMode || fullSimilarityMode || combineMode ) )
    std::cout << "Must pick at least one mode (hierarchy, partition, full)" << std::endl;
  
  indri::collection::Repository repository;
  std::vector< document_t* > documentsX;
  std::vector< document_t* > documentsY;
  bool upperTriangle = false;
  
  try {
    // open repository
    if( !combineMode )
      repository.openRead( repositoryPath );
  
    // fetch index
    indri::index::Index* index = 0; 
    
    if( !combineMode ) {
      indri::collection::Repository::index_state state = repository.indexes();
      index = (*state)[0];
    }
      
    // read documents
    if( !combineMode ) {
      documentsX = read_documents( index, documentsXPath );

      if( documentsYPath.size() ) {
        documentsY = read_documents( index, documentsYPath );
      } else {
        upperTriangle = true;
        documentsY = documentsX;
      }
    }
    
    if( hierarchyMode ) {
      hierarchy( index, 
                 documentsX );
    } else if( partitionMode ) {
      partition( outputPath,
                 index,
                 documentsX,
                 dot_product_sqrt,
                 overlap,
                 startDocID,
                 endDocID );
    } else if( combineMode ) {
      combine_topk( outputPath, inputFiles, k );
    } else {
      topk_similarity( outputPath,
                       index,
                       documentsX,
                       documentsY,
                       dot_product_sqrt,
                       k,
                       upperTriangle );
    }
  
    if( !combineMode )
      repository.close();
  } catch( lemur::api::Exception& e ) {
    LEMUR_ABORT(e);
  }

  return 0;
}
