//
// CosineSimilarityFunction
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_COSINESIMILARITYFUNCTION_HPP
#define INDRI_SIMILARITY_COSINESIMILARITYFUNCTION_HPP

#include "indri/UnigramDocument.hpp"

namespace indri {
    namespace similarity {
        class CosineSimilarityFunction {
        private:
            const UnigramCollection& _collection;

        public:
            CosineSimilarityFunction( const UnigramCollection& unigram ) :
              _collection(unigram)
            {
            }

            static double bothMatch( const double probabilityOne, const double probabilityTwo,
                               const double backgroundOne, const double backgroundTwo ) {
                return probabilityOne * probabilityTwo -
                       backgroundOne * backgroundTwo;
            }

            static double oneMatches( const double probabilityOne, const double backgroundOne,
                                const double backgroundTwo ) {
                return (probabilityOne - backgroundOne) * probabilityTwo;
            }

            static double firstMatches( const double probabilityOne, const double backgroundOne, const double backgroundTwo ) {
                return oneMatches( probabilityOne, backgroundOne, backgroundTwo );
            }
            
            static double secondMatches( const double probabilityOne, const double backgroundOne, const double backgroundTwo ) {
                return oneMatches( probabilityOne, backgroundOne, backgroundTwo );
            }

            bool symmetric() {
                return true;
            }
        };
    }
}

#endif // INDRI_SIMILARITY_COSINESIMILARITYFUNCTION_HPP
