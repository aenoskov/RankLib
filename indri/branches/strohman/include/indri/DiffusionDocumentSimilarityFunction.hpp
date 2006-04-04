
//
// DiffusionDocumentSimilarityFunction
//
// 9 March 2006 -- tds
//

#ifndef INDRI_DIFFUSIONSIMILARITYFUNCTION_HPP
#define INDRI_DIFFUSIONSIMILARITYFUNCTION_HPP

namespace indri {
  namespace similarity {
    class DiffusionDocumentSimilarityFunction : public DocumentSimilarityFunction {
    private:
      double _t;

    public:
      DiffusionDocumentSimilarityFunction( double t ) : _t(t) {}
      DiffusionDocumentSimilarityFunction() {}

      double t() const {
        return _t;
      }

      double bothMatch( const double probabilityOne, const double probabilityTwo,
                    const double backgroundOne, const double backgroundTwo ) const {
        return sqrt( probabilityOne * probabilityTwo ) -
               sqrt( backgroundOne * backgroundTwo );
      }

      double firstMatches( const double probabilityOne, const double backgroundOne,
                                         const double backgroundTwo ) const {
        return sqrt( probabilityOne * backgroundTwo ) - 
               sqrt( backgroundOne * backgroundTwo );
      }

      double secondMatches( const double probabilityTwo, const double backgroundTwo,
                                    const double backgroundOne ) const {
        return sqrt( backgroundOne * probabilityTwo ) - 
               sqrt( backgroundOne * backgroundTwo );
      }

      bool symmetric() const {
        return true;
      }
    };
  }
}

#endif // INDRI_DIFFUSIONSIMILARITYFUNCTION_HPP
