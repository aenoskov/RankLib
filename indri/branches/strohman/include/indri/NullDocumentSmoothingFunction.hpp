
//
// NullDocumentSmoothingFunction
//
// 9 March 2006 -- tds
//

#ifndef INDRI_NULLDOCUMENTSMOOTHINGFUNCTION_HPP
#define INDRI_NULLDOCUMENTSMOOTHINGFUNCTION_HPP

namespace indri {
  namespace similarity {
    class NullDocumentSmoothingFunction : public DocumentSmoothingFunction {
    public:
      NullDocumentSmoothingFunction() {}
      ~NullDocumentSmoothingFunction() {};   
    
      void operator() ( SmoothedUnigramDocument& smoothed, const UnigramDocument& input ) const {
        assert( input.terms.size() == input.counts.size() );
        double oneOverDocumentLength = 1.0 / double(input.length);

        for( int i=0; i<input.terms.size(); i++ ) {
          int termID = input.terms[i];                    
          int count = input.counts[i];

          smoothed.terms.push_back( termID );
          smoothed.probabilities.push_back( double(count) * oneOverDocumentLength );
          smoothed.background.push_back( 0 );
        }

        assert( smoothed.terms.size() == smoothed.probabilities.size() );
        assert( smoothed.probabilities.size() == smoothed.background.size() );
        smoothed.length = input.length;
      }

      double background( const SmoothedUnigramDocument& document, int missingTerm ) const {
        return 0;
      }
    };
  }
}

#endif // INDRI_NULLDOCUMENTSMOOTHINGFUNCTION_HPP

