//
// KLDivergenceSimilarityFunction
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_KLDIVERGENCEDOCUMENTSIMILARITYFUNCTION_HPP
#define INDRI_SIMILARITY_KLDIVERGENCEDOCUMENTSIMILARITYFUNCTION_HPP

#include "indri/UnigramDocument.hpp"
#include "indri/SmoothedUnigramDocument.hpp"
#include "indri/DocumentSimilarityFunction.hpp"
#include <math.h>

namespace indri {
    namespace similarity {
        class KLDivergenceSimilarityFunction : public DocumentSimilarityFunction {
        public:
            ~KLDivergenceSimilarityFunction() {};

            double bothMatch( const double probabilityOne, const double probabilityTwo,
                               const double backgroundOne, const double backgroundTwo ) const {
                return probabilityOne * log( probabilityTwo ) -
                       backgroundOne * log( backgroundTwo );
            }

            double firstMatches( const double probabilityOne, const double backgroundOne,
                                         const double backgroundTwo ) const {
                return (probabilityOne - backgroundOne) * log( backgroundTwo );
            }

            double secondMatches( const double probabilityTwo, const double backgroundTwo,
                                          const double backgroundOne ) const {
                return backgroundOne * (log( probabilityTwo ) - log( backgroundTwo ));
            }

            bool symmetric() const {
                return false;
            }
        };
    }
}

#endif // INDRI_SIMILARITY_KLDIVERGENCEDOCUMENTSIMILARITYFUNCTION_HPP
