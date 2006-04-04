
//
// TermCountHolder
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_UNIGRAMCOLLECTION_HPP
#define INDRI_SIMILARITY_UNIGRAMCOLLECTION_HPP

#include "indri/HashTable.hpp"
#include "indri/greedy_vector"
#include "indri/VocabularyIterator.hpp"
#include <math.h>
#include "indri/Index.hpp"
#include "indri/DiskTermData.hpp"

namespace indri {
    namespace similarity {
        /** This class holds truncated term counts for all terms in a collection.
         *  The term counts list can be truncated if desired in order to keep memory
         *  use down for large collections; for instance, perhaps all terms with fewer
         *  than 5 occurrences would be listed as having a single occurrence.
         */

        class UnigramCollection {
        private:
            indri::utility::greedy_vector<UINT64>       _dense;
            indri::utility::HashTable<int, UINT64>      _sparse;           
            indri::utility::HashTable<UINT64, UINT64>   _frequencyHistogram;
            UINT64                                      _uniqueTermCount;
            UINT64                                      _termCount;
            int                                         _documentCount;
            double                                      _entropy;
            double                                      _dotProduct;

            void _incrementHistogram( UINT64 termCount ) {
                UINT64* histogramValue = _frequencyHistogram.find( termCount );

                if( ! histogramValue ) {
                    *_frequencyHistogram.insert( termCount ) = 1;
                } else {
                    ++(*histogramValue);
                }
            }

            void _computeEntropy() {
                // computes the entropy of the collection
                // assumes that the collection histogram has already been created
                indri::utility::HashTable<UINT64, UINT64>::iterator iterator;
                _entropy = 0;
                double oneOverTermCount = 1.0 / double(_termCount);

                for( iterator = _frequencyHistogram.begin(); iterator != _frequencyHistogram.end(); iterator++ ) {
                    // number of unique terms sharing the same collection count
                    double numberOfUniqueTerms = (double) *(iterator->second);
                    // the collection count that all these terms share
                    double collectionTermCount = (double) *(iterator->first);

                    _entropy += numberOfUniqueTerms * (collectionTermCount * oneOverTermCount) * log( (collectionTermCount * oneOverTermCount) );
                }
            }

            void _computeDistributionDotProduct() {
                // computes the dot product of the term distribution with itself
                indri::utility::HashTable<UINT64, UINT64>::iterator iterator;
                double oneOverTermCount = 1.0 / double(_termCount);
                _dotProduct = 0;

                for( iterator = _frequencyHistogram.begin(); iterator != _frequencyHistogram.end(); iterator++ ) {
                    // number of unique terms sharing the same collection count
                    double numberOfUniqueTerms = (double) *(iterator->second);
                    // the collection count that all these terms share
                    double collectionTermCount = (double) *(iterator->first);

                    _dotProduct += numberOfUniqueTerms * (collectionTermCount * oneOverTermCount) * (collectionTermCount * oneOverTermCount);
                }
            }

            void _loadInfrequentTermsSparse( indri::index::Index* index, int truncateCount ) {
                _sparse.clear();
                indri::index::VocabularyIterator* infrequent = index->infrequentVocabularyIterator();

                _sparseLoad( infrequent, truncateCount );
                delete infrequent;
                _documentCount = index->documentCount();
            }

            void _denseLoad( indri::index::VocabularyIterator* iterator ) {
                for( iterator->startIteration(); !iterator->finished(); iterator->nextEntry() ) {
                    indri::index::DiskTermData* data = iterator->currentEntry();
                    UINT64 total = data->termData->corpus.totalCount;

                    _dense.push_back( total );
                    _incrementHistogram( total );

                    ++_uniqueTermCount;
                    _termCount += total;
                }
            }

            void _sparseLoad( indri::index::VocabularyIterator* iterator, int truncateCount ) {
                for( iterator->startIteration(); !iterator->finished(); iterator->nextEntry() ) {
                    indri::index::DiskTermData* data = iterator->currentEntry();
                    UINT64 total = data->termData->corpus.totalCount;

                    if( truncateCount < total ) {
                        *_sparse.insert( data->termID ) = total;
                    } 

                    _incrementHistogram( total );
                    ++_uniqueTermCount;
                    _termCount += total;
                }
            }

            void _loadInfrequentTermsDense( indri::index::Index* index ) {
                indri::index::VocabularyIterator *infrequent = index->infrequentVocabularyIterator();

                _denseLoad( infrequent );
                delete infrequent;
                _documentCount = index->documentCount();
            }
            
            void _loadFrequentTerms( indri::index::Index* index ) {
                _dense.clear();

                indri::index::VocabularyIterator* frequent = index->frequentVocabularyIterator();
                _dense.push_back( 0 );

                _denseLoad( frequent );
                delete frequent;
            }

        public:
            UnigramCollection() :
                _termCount(0),
                _documentCount(0),
                _entropy(0),
                _dotProduct(0)
            {
            }
                
            /** Returns the corpus count for a particular term */
            UINT64 operator[] ( unsigned int termID ) const {
                if( termID < _dense.size() )
                    return _dense[termID];

                if( termID > _termCount )
                    return 0;

                const UINT64* value = _sparse.find( termID );

                if( value == 0 )
                    return 1;

                return *value;
            }            

            /** Returns the total number of terms in the collection */
            UINT64 termCount() const {
                return _termCount;
            }

            /** Returns the total number of unique terms in the collection */
            UINT64 uniqueTermCount() const {
                return _uniqueTermCount;
            }

            /** Returns the total document count in the collection */
            int documentCount() const {
                return _documentCount;
            }

            /** Returns the entropy of the collection term distribution. */
            double entropy() const {
                return _entropy;
            }
    
            /** Returns the dot product of the distribution with itself. */
            double dotProduct() const {
                return _dotProduct;
            }

            /** Loads all the term data in for a particular index */
            void load( indri::index::Index* index, int truncateCount = 1 ) {
                _termCount = 0;

                _loadFrequentTerms( index );
                if( truncateCount > 1 ) {
                  _loadInfrequentTermsSparse( index, truncateCount );
                } else {
                  _loadInfrequentTermsDense( index );
                }
                _computeEntropy();
            }
        };
    }
}

#endif // INDRI_SIMILARITY_UNIGRAMCOLLECTION_HPP



