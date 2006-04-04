
//
// DocumentSmoothingFunction
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_DOCUMENTSMOOTHINGFUNCTION_HPP
#define INDRI_SIMILARITY_DOCUMENTSMOOTHINGFUNCTION_HPP

#include "indri/UnigramDocument.hpp"
#include "indri/SmoothedUnigramDocument.hpp"

namespace indri {
    namespace similarity {
        class DocumentSmoothingFunction {
        public:
            virtual ~DocumentSmoothingFunction() {};
            virtual void operator() ( SmoothedUnigramDocument& smoothed, const UnigramDocument& input ) const = 0;
            virtual double background( const SmoothedUnigramDocument& smoothed, int termID ) const = 0;
        };
    }
}

#endif // INDRI_SIMILARITY_DOCUMENTSMOOTHINGFUNCTION_HPP

