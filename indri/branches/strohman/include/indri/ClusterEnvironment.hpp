//
// ClusterEnvironment
//
// 17 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_CLUSTERENVIRONMENT_HPP
#define INDRI_SIMILARITY_CLUSTERENVIRONMENT_HPP

#include <algorithm>
#include <vector>
#include <functional>

#include "indri/UnigramDocument.hpp"
#include "indri/SmoothedUnigramDocument.hpp"
#include "indri/greedy_vector"
#include "indri/Index.hpp"
#include "indri/Repository.hpp"
#include "indri/UnigramCollection.hpp"
#include <string>

#include "indri/DocumentSmoothingFunction.hpp"
#include "indri/DocumentSimilarityFunction.hpp"
#include "indri/DocumentComparator.hpp"

namespace indri {
    namespace similarity {
        // what documents are in a cluster?
        // what is the center of the cluster, and/or how would I find out?
        // want a normalized version of the document vector
        // how do the documents in this cluster relate to one another?

        class Matrix {
        private:
            double* _v;
            int _rows;
            int _columns;

        public:
            Matrix( int rows, int columns ) {
                _v = new double[ rows * columns ];
                _rows = rows;
                _columns = columns;
            }

            inline double get( int i, int j ) {
                assert( i < _rows && j < _columns );
                assert( i >= 0 && j >= 0 );

                return _v[i*_columns+j];
            } 

            inline void set( int i, int j, double value ) {
                assert( i < _rows && j < _columns );
                assert( i >= 0 && j >= 0 );

                _v[i*_columns+j] = value;
            }

            inline int rows() {
                return _rows;
            }

            inline int columns() {
                return _columns;
            }

            inline double rowSum( int i ) {
              double total = 0;
              for( int k=0; k<_columns; k++ )
                total += get( i, k );
              return total;
            }
  
            inline double columnSum( int j ) {
              double total = 0;
              for( int k=0; k<_rows; k++ )
                total += get( k, j );
              return total;
            }
        };

        class ClusterEnvironment {
        private:
            /** Number of documents to compare in one block.  We want this to be small enough 
                that 2*_stride documents can fit in the L2 cache with room to spare, but it should
                be big enough that we're getting some benefit from the cache locality. */
            const int _stride;

            /** Same as the stride, but now we want the number of documents we can reasonably
                hold in RAM at once. */
            const int _ramStride;

            indri::collection::Repository _repository;
            UnigramCollection _collection;

            AbstractDocumentComparator* _comparator;

            std::map< std::string, std::string > _unpackRuleString( const std::string& rule ) const;
            std::vector<SmoothedUnigramDocument*> _fetchDocuments( const std::vector<int>& documents );

            void _maxMatrix( std::vector<int>& result, 
                             Matrix* matrix, 
                             const std::vector<int>& comparisons,
                             int startOffset );

            void _compareBlock( Matrix* output,
                                const std::vector<SmoothedUnigramDocument*>& sourceDocuments,
                                const std::vector<SmoothedUnigramDocument*>& destDocuments,
                                const std::pair<int, int>& sourceRange,
                                const std::pair<int, int>& destRange,
                                bool upperTriangle );

        public:
            ClusterEnvironment() :
                _stride(100),
                _ramStride(100000)
            {
            }

            // what do you want to do?

            // use cases: 
            //      get the most similar document to a centroid
            //      get the k-most similar documents to something
            //      compute a full similarity matrix
            //      overlapping clusters; how is that done?

            //      agglomerative clustering...?  I think that'd be quite slow           
            //      approximate clustering methods
            //      approximation for centroid in cluster, or ability to pick a centroid from a cluster, etc.

            //      want to do K-means as well

            bool openRead( const std::string& indexName );
            void close();
            void setComparisonMethod( const std::string& smoothingRule );

            // base methods
            std::vector<int> mostSimilarDocuments( const std::vector<int>& candidates, const std::vector<int>& comparisons );
            indri::similarity::Matrix* compareDocuments( const std::vector<int>& source, const std::vector<int>& destination );
            int medoid( const std::vector<int>& documents );
        };
    }
}

#endif // INDRI_SIMILARITY_CLUSTERENVIRONMENT_HPP

