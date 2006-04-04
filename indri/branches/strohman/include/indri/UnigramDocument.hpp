
//
// UnigramDocument
//
// 13 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_UNIGRAMDOCUMENT_HPP
#define INDRI_SIMILARITY_UNIGRAMDOCUMENT_HPP

#include "indri/greedy_vector"

namespace indri {
    namespace similarity {
        struct UnigramDocument {
            indri::utility::greedy_vector< int > terms;
            indri::utility::greedy_vector< int > counts;
            int length;
        };
    }
}

#endif // INDRI_SIMILARITY_UNIGRAMDOCUMENT_HPP

