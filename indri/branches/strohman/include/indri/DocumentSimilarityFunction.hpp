
//
// DocumentSimilarityFunction
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_DOCUMENTSIMILARITYFUNCTION_HPP
#define INDRI_SIMILARITY_DOCUMENTSIMILARITYFUNCTION_HPP

#include "indri/UnigramDocument.hpp"

namespace indri {
    namespace similarity {
        class BaseSimilarityFunction {
        public:
            virtual ~BaseSimilarityFunction() {};
            virtual double operator() ( const SmoothedUnigramDocument& one, const SmoothedUnigramDocument& two ) const = 0;
        };

        class DocumentSimilarityFunction {
        public:
            DocumentSimilarityFunction() {};
            virtual ~DocumentSimilarityFunction() {};
            virtual bool symmetric() const = 0;

            virtual double bothMatch( double pOne, double pTwo, double bOne, double bTwo ) const = 0;
            virtual double firstMatches( double pOne, double bOne, double bTwo ) const = 0;
            virtual double secondMatches( double pTwo, double bTwo, double bOne ) const = 0;
            virtual void normalize( SmoothedUnigramDocument& input ) {}
        };
    }
}

#endif // INDRI_SIMILARITY_SIMILARITYFUNCTION_HPP
