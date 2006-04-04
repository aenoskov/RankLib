// 
// ClusterEnvironment
// 
// 17 February 2006 -- tds
// 

#include "indri/ClusterEnvironment.hpp"
#include <algorithm>
#include "indri/DocumentComparator.hpp"
#include "indri/KLDivergenceSimilarityFunction.hpp"
#include "indri/TermList.hpp"
#include "indri/Index.hpp"
#include "indri/count_iterator"
#include "indri/greedy_vector"

//
// cenv_termlist_length
//

static int cenv_termlist_length( const indri::index::TermList* termList ) {
    int length = 0;

    for( int i=0; i<termList->terms().size(); i++ ) {
        if( termList->terms()[i] != 0 )
           length++;
    }

    return length;
}

//
// openRead
//

bool indri::similarity::ClusterEnvironment::openRead( const std::string & indexName ) {
    _repository.openRead( indexName );

    indri::index::Index * index = ( *_repository.indexes() )[0];
    _collection.load( index );

    setComparisonMethod( "compare=kl:smoothing=dirichlet:mu=2500" );
}

//
// close
//

void indri::similarity::ClusterEnvironment::close() {
    _repository.close();
}

//
// _unpackRuleString
//

std::map< std::string, std::string > indri::similarity::ClusterEnvironment::_unpackRuleString( const std::string& rule ) const {
    // first, split by colons:
    int spot = 0;
    int index = 0;
    std::map< std::string, std::string > pairs;
  
    do {
        spot = rule.find( ':', index );
        std::string pair = rule.substr( index, spot-index );

        int equalsSpot = pair.find( '=' );
        std::string key = pair.substr( 0, equalsSpot );
        std::string value = pair.substr( equalsSpot+1 );
 
        pairs[key] = value; 
        index = spot+1;
    }
    while( spot > 0 );

    return pairs;
}

//
// map_get
//

std::string map_get( const std::map<std::string,std::string>& m, const std::string& key, const std::string& dvalue ) {
  std::map<std::string, std::string>::const_iterator iter;
  iter = m.find( key );

  if( iter == m.end() )
    return dvalue;
  else
    return iter->second;
}

//
// get_dirichlet
//

static indri::similarity::DirichletDocumentSmoothingFunction* get_dirichlet( const std::map<std::string, std::string>& parameters,
                                                                             const indri::similarity::UnigramCollection& collection ) {
  std::string muString = map_get( parameters, "mu", "1000" );
  double mu = atof( muString.c_str() );
  return new indri::similarity::DirichletDocumentSmoothingFunction( collection, mu );
}

//
// get_null
//

static indri::similarity::NullDocumentSmoothingFunction* get_null( const std::map< std::string, std::string>& parameters ) {
  return new indri::similarity::NullDocumentSmoothingFunction();
}

//
// get_jm
//

static indri::similarity::JelinekMercerDocumentSmoothingFunction* get_jm( const std::map< std::string, std::string>& parameters,
                                                                          const indri::similarity::UnigramCollection& collection ) {
  std::string lambdaString = map_get( parameters, "lambda", "0.4" );
  double lambda = atof( lambdaString.c_str() );
  
  return new indri::similarity::JelinekMercerDocumentSmoothingFunction( collection, lambda );
}

//
// get_diffusion
//

static indri::similarity::DiffusionDocumentSimilarityFunction* get_diffusion( const std::map< std::string, std::string>& parameters ) {
  std::string tString = map_get( parameters, "t", "1" );
  double t = atof( tString.c_str() );
  return new indri::similarity::DiffusionDocumentSimilarityFunction( t );
}

//
// get_kl
//

static indri::similarity::KLDivergenceSimilarityFunction* get_kl( const std::map< std::string, std::string>& parameters ) {
  return new indri::similarity::KLDivergenceSimilarityFunction();
}

//
// setComparisonMethod
//

void indri::similarity::ClusterEnvironment::setComparisonMethod( const std::string& smoothingRule ) {
    std::map< std::string, std::string > pairs = _unpackRuleString( smoothingRule );
    std::string method = map_get( pairs, "method", "kl" );
    std::string smoothing = map_get( pairs, "smoothing", "jm" );

    if( method == "kl" ) {
      KLDivergenceSimilarityFunction* kl = new KLDivergenceSimilarityFunction();

      if( smoothing == "jm" ) {
        JelinekMercerDocumentSmoothingFunction* jm = get_jm( pairs, _collection );

        _comparator = new DocumentComparator<KLDivergenceSimilarityFunction,
                                             JelinekMercerDocumentSmoothingFunction>( kl, jm, _collection );
      } else if( smoothing == "null" || smoothing == "none" ) {
        NullDocumentSmoothingFunction* nullSmoothing = get_null( pairs );

        _comparator = new DocumentComparator<KLDivergenceSimilarityFunction,
                                             NullDocumentSmoothingFunction>( kl, nullSmoothing, _collection );
      } else {
        DirichletDocumentSmoothingFunction* dir = get_dirichlet( pairs, _collection );

        _comparator = new DocumentComparator<KLDivergenceSimilarityFunction,
                                             DirichletDocumentSmoothingFunction>( kl, dir, _collection );
      }
    } else if( method == "diffusion" ) {
      DiffusionDocumentSimilarityFunction* diffusion = get_diffusion( pairs );

      if( smoothing == "jm" ) {
        JelinekMercerDocumentSmoothingFunction* jm = get_jm( pairs, _collection );

        _comparator = new DocumentComparator<DiffusionDocumentSimilarityFunction,
                                             JelinekMercerDocumentSmoothingFunction>( diffusion, jm, _collection );
      } else if( smoothing == "null" || smoothing == "none" ) {
        NullDocumentSmoothingFunction* nullSmoothing = get_null( pairs );

        _comparator = new DocumentComparator<DiffusionDocumentSimilarityFunction,
                                             NullDocumentSmoothingFunction>( diffusion, nullSmoothing, _collection );
      } else {
        DirichletDocumentSmoothingFunction* dir = get_dirichlet( pairs, _collection );

        _comparator = new DocumentComparator<DiffusionDocumentSimilarityFunction,
                                             DirichletDocumentSmoothingFunction>( diffusion, dir, _collection );
      }
    }
}

//
// _maxMatrix
//

void indri::similarity::ClusterEnvironment::_maxMatrix( std::vector<int>& result, Matrix* matrix, const std::vector<int>& comparisons, int startOffset ) {
  assert( startOffset + matrix->rows() <= result.size() );

  for ( int j = 0; j < matrix->rows(); j++ ) {
      int m = 0;
      double maxValue = -DBL_MAX;

      for ( int k = 0; k < matrix->columns(); k++ ) {
          double value = matrix->get( j, k );

          if ( value > maxValue ) {
              m = k;
              maxValue = value;
          }
      }

      result[startOffset + j] = comparisons[m];
  }
}

//
// mostSimilarDocuments
//
// Uses compareDocuments to find the most similar document for each document in 
// the comparison set.  Primarily useful for k-means type clustering.
//

std::vector<int> indri::similarity::ClusterEnvironment::mostSimilarDocuments( const std::vector< int >& candidates,
                                                                              const std::vector< int >& comparisons )
{
    std::vector<int> result;
    result.resize( candidates.size(), 0 );

    for ( int i = 0; i < candidates.size(); i += _stride ) {
        std::vector<int> candidateChunk;
        int endRange = std::min<int>( i + _stride, candidates.size() );

        candidateChunk.assign( candidates.begin() + i, candidates.begin() + endRange );
        Matrix* matrix = compareDocuments( candidateChunk, comparisons );

        _maxMatrix( result, matrix, comparisons, i );
        delete matrix;
    }

    return result;
}

//
// _compareBlock
//
// Compares a small block of documents and stores the result in the output matrix
//

void indri::similarity::ClusterEnvironment::_compareBlock( Matrix* output,
    const std::vector<SmoothedUnigramDocument*>& sourceDocuments,
    const std::vector<SmoothedUnigramDocument*>& destDocuments,
    const std::pair<int,int>& sourceRange,
    const std::pair<int,int>& destRange,
    bool upperTriangle ) {
  // if we're using the upperTriangle option, this region should be
  // square
  assert( !upperTriangle || ((sourceRange.second - sourceRange.first) == (destRange.second - destRange.first)) );
  int i, j;

  for( i = sourceRange.first; i < sourceRange.second; i++ ) {
    if( upperTriangle )
      j = i;
    else
      j = destRange.first;

    for ( j = destRange.first; j < destRange.second; j++ ) {
      SmoothedUnigramDocument* source = sourceDocuments[i];
      SmoothedUnigramDocument* dest = destDocuments[j];

      double value = _comparator->compare( *source, *dest );
      output->set( i, j, value );
    }
  }
}

//
// _fetchDocuments
//
// Fetches documents from documentIDs and smooths them.
//

std::vector<indri::similarity::SmoothedUnigramDocument*> indri::similarity::ClusterEnvironment::_fetchDocuments( const std::vector<int>& documentIDs ) {
    std::vector<indri::similarity::SmoothedUnigramDocument*> documents;
    indri::index::Index* index = (*_repository.indexes())[0];

    for( int i=0; i<documentIDs.size(); i++ ) {
        SmoothedUnigramDocument* smoothed = new SmoothedUnigramDocument();

        // step 1: fetch the document
        const indri::index::TermList* termList = index->termList( documentIDs[i] );

        // step 2: convert the document to unigram representation
        UnigramDocument unsmoothed;
        
        unsmoothed.length = cenv_termlist_length( termList );

        // turn the document vector into a bag of words representation
        indri::utility::greedy_vector<int> terms = termList->terms();
        indri::utility::count_iterator<int> iter( terms.begin(), terms.end() );
        unsmoothed.length = 0;

        // run through the termIDs, get frequency for each  
        for( ; iter != terms.end(); ++iter ) {
            if( (*iter).object > 0 ) {
                unsmoothed.terms.push_back( (*iter).object );
                unsmoothed.counts.push_back( (*iter).count );
                unsmoothed.length++;
            }
        }

        if( unsmoothed.length == 0 ) {
            unsmoothed.length = 1;
        }

        // step 3: smooth the document
        _comparator->smooth( *smoothed, unsmoothed );

        assert( smoothed->terms.size() == smoothed->probabilities.size() );
        assert( smoothed->terms.size() == smoothed->background.size() );

        // step 4: add smoothed document to result vector
        documents.push_back( smoothed );
        delete termList;
    }

    return documents;
}

//
// compareDocuments
//
// Returns a source.size() * destination.size() matrix, where each matrix entry
// is the result of comparing source[i] to destination[j], using the current
// smoothing and similarity methods.
//

indri::similarity::Matrix* indri::similarity::ClusterEnvironment::compareDocuments( const std::vector<int>& source,
                                                                                    const std::vector<int>& destination ) {
    bool symmetric = ( &source == &destination ) && _comparator->symmetric();
    Matrix* result = new Matrix( source.size(), destination.size() );

    std::vector <SmoothedUnigramDocument*> sourceDocuments;
    std::vector <SmoothedUnigramDocument*> destDocuments;

    sourceDocuments = _fetchDocuments( source );
    destDocuments = _fetchDocuments( destination );

    for ( int i = 0; i < source.size(); i += _stride ) {
        for ( int j = 0; j < destination.size(); j += _stride ) {
            std::pair<int, int>  sourceRange = std::make_pair( i, std::min<int>( i + _stride, source.size() ) );
            std::pair<int, int>  destRange   = std::make_pair( j, std::min<int>( j + _stride, destination.size() ) );

            if ( symmetric && sourceRange.first > destRange.first )
                continue;

            bool upperTriangle = ( symmetric && sourceRange == destRange );

            _compareBlock( result, sourceDocuments, destDocuments, sourceRange, destRange, upperTriangle );
        }
    }

    if ( symmetric ) {
        for ( int i = 0; i < source.size(); i++ ) {
            for ( int j = 0; j < i; j++ ) {
                result->set( i, j, result->get( j, i ) );
            }
        }
    }

    indri::utility::delete_vector_contents( sourceDocuments );
    indri::utility::delete_vector_contents( destDocuments );

    return result;
}

//
// center
//

int indri::similarity::ClusterEnvironment::medoid( const std::vector<int>& documents ) {
  indri::similarity::Matrix* matrix = compareDocuments( documents, documents );
  int bestRow = 0;
  double bestSum = -DBL_MAX;

  for( int i=0; i<matrix->rows(); i++ ) {
    double s = matrix->rowSum( i );
    
    if( s > bestSum )
      bestSum = s, bestRow = i;
  }


}
