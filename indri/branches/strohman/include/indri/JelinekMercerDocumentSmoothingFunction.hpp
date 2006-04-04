
//
// JelinekMercerDocumentSmoothingFunction
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_JELINEKMERCERDOCUMENTSMOOTHINGFUNCTION_HPP
#define INDRI_SIMILARITY_JELINEKMERCERDOCUMENTSMOOTHINGFUNCTION_HPP

#include "indri/UnigramDocument.hpp"
#include "indri/SmoothedUnigramDocument.hpp"
#include "indri/UnigramCollection.hpp"

namespace indri {
    namespace similarity {
        class JelinekMercerDocumentSmoothingFunction : public DocumentSmoothingFunction {
        private:
            const UnigramCollection& _collection;
            double _lambda;
            double _lambdaOverCollectionLength;
            double _oneOverCollectionLength;

        public:
            JelinekMercerDocumentSmoothingFunction( const UnigramCollection& collection, double lambda ) :
                _collection(collection),
                _lambda(lambda)
            {
                _oneOverCollectionLength = 1.0 / double(_collection.termCount());
                _lambdaOverCollectionLength = _lambda / double(_collection.termCount());
            }

            ~JelinekMercerDocumentSmoothingFunction() {};

            double lambda() const {
                return _lambda;
            }

            void operator() ( SmoothedUnigramDocument& smoothed, const UnigramDocument& input ) const {
                assert( input.terms.size() == input.counts.size() );

                double oneOverDocumentLength = 1.0 / double(input.length);
                double oneMinusLambda = 1.0 - _lambda;
                double oneOverCollectionLength = 1.0 / double(_collection.termCount());
                double lambdaOverCollectionLength = _lambda * oneOverCollectionLength;

                for( int i=0; i<input.terms.size(); i++ ) {
                    int termID = input.terms[i];                    
                    int count = input.counts[i];

                    double collectionProbability = _collection[termID] * oneOverCollectionLength;
                    double background = collectionProbability * lambdaOverCollectionLength;

                    double foreground = (1 - _lambda) * double(count) * oneOverDocumentLength;
                    
                    smoothed.terms.push_back( termID );
                    smoothed.probabilities.push_back( foreground + background );
                    smoothed.background.push_back( background );
                }

                assert( smoothed.terms.size() == smoothed.probabilities.size() );
                assert( smoothed.probabilities.size() == smoothed.background.size() );
                smoothed.length = input.length;
            }

            double background( const SmoothedUnigramDocument& document, int missingTerm ) const {
                return _lambdaOverCollectionLength * double(_collection[missingTerm]);
            }
        };
    }
}

#endif // INDRI_SIMILARITY_JELINEKMERCERDOCUMENTSMOOTHINGFUNCTION_HPP
