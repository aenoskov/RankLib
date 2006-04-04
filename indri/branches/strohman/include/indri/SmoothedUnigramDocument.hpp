
//
// SmoothedUnigramDocument
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_SMOOTHEDUNIGRAMDOCUMENT_HPP
#define INDRI_SIMILARITY_SMOOTHEDUNIGRAMDOCUMENT_HPP

#include "indri/greedy_vector"

namespace indri {
    namespace similarity {
        struct SmoothedUnigramDocument {
            indri::utility::greedy_vector< int >    terms;
            indri::utility::greedy_vector< double > probabilities;
            indri::utility::greedy_vector< double > background;
            int                                     length;
        };
    }
}

#endif // INDRI_SIMILARITY_SMOOTHEDUNIGRAMDOCUMENT_HPP

