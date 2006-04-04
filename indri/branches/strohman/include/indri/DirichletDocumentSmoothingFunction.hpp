
//
// DirichletDocumentSmoothingFunction
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_DIRICHLETDOCUMENTSMOOTHINGFUNCTION_HPP
#define INDRI_SIMILARITY_DIRICHLETDOCUMENTSMOOTHINGFUNCTION_HPP

#include "indri/UnigramDocument.hpp"
#include "indri/UnigramCollection.hpp"

namespace indri {
    namespace similarity {
        class DirichletDocumentSmoothingFunction : public DocumentSmoothingFunction {
        private:
            const UnigramCollection& _collection;
            double _mu;
            double _oneOverCollectionLength;

        public:
            DirichletDocumentSmoothingFunction( const UnigramCollection& collection, double mu ) :
                _collection(collection),
                _mu(mu)
            {
                _oneOverCollectionLength = 1.0 / _collection.termCount();
            }

            ~DirichletDocumentSmoothingFunction() {};

            double mu() const {
              return _mu;
            }

            void operator() ( SmoothedUnigramDocument& smoothed, const UnigramDocument& input ) const {
                double oneOverDocumentLength = 1.0 / double(input.length);
                double oneOverDenominator = 1.0 / (_mu + double(input.length));

                for( int i=0; i<input.terms.size(); i++ ) {
                    int termID = input.terms[i];                    
                    int count = input.counts[i];

                    double collectionCount = _collection[termID];
                    double collectionProbability = collectionCount * _oneOverCollectionLength;
                    double backgroundNumerator = _mu * collectionProbability;

                    double numerator = double(count) + backgroundNumerator;
                    double probability = numerator * oneOverDenominator;
                    double background = backgroundNumerator * oneOverDenominator;

                    smoothed.terms.push_back( termID );
                    smoothed.probabilities.push_back( probability );
                    smoothed.background.push_back( background );
                }

                assert( smoothed.terms.size() == smoothed.probabilities.size() );
                assert( smoothed.probabilities.size() == smoothed.background.size() );
                smoothed.length = input.length;
            }
          
            double background( const SmoothedUnigramDocument& document, int missingTerm ) const {
              double numerator = _mu * double(_collection[missingTerm]) * _oneOverCollectionLength;
              double denominator = _mu + document.length;
              return numerator / denominator;
            }
        };
    }
}

#endif // INDRI_SIMILARITY_DIRICHLETDOCUMENTSMOOTHINGFUNCTION_HPP
