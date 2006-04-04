
//
// BaseComparisonValueFunction
//
// 22 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_BASECOMPARISONVALUEFUNCTION_HPP
#define INDRI_SIMILARITY_BASECOMPARISONVALUEFUNCTION_HPP

#include "indri/KLDivergenceSimilarityFunction.hpp"
#include "indri/DiffusionDocumentSimilarityFunction.hpp"
#include "indri/JelinekMercerDocumentSmoothingFunction.hpp"
#include "indri/DirichletDocumentSmoothingFunction.hpp"
#include "indri/NullDocumentSmoothingFunction.hpp"
#include "indri/UnigramCollection.hpp"

namespace indri {
  namespace similarity {
    template<typename _T, typename _V>
    class BaseComparisonValueFunction {
    public:
      BaseComparisonValueFunction( _T& t, _V& v, UnigramCollection& collection ) {
      }

      double baseResult ( const SmoothedUnigramDocument& one,
                          const SmoothedUnigramDocument& two ) const {
        return 0;
      }

      double scaleResult( double result ) const {
        return result;
      }
    };

    //
    // KL-Divergence with Jelinek Mercer smoothing
    //

    template<>
    class BaseComparisonValueFunction<KLDivergenceSimilarityFunction,
                                      JelinekMercerDocumentSmoothingFunction> {
      double _lambda;
      const UnigramCollection& _collection;

    public:
      BaseComparisonValueFunction( const KLDivergenceSimilarityFunction& kl,
                                   const JelinekMercerDocumentSmoothingFunction& jm,
                                   const UnigramCollection& collection ) :
        _collection(collection),
        _lambda( jm.lambda() )
      { 
      }

      double baseResult ( const SmoothedUnigramDocument& one,
                          const SmoothedUnigramDocument& two ) const {
        return _lambda * ( log( _lambda ) + _collection.entropy() );
      }

      double scaleResult( double result ) const {
        return result;
      }
    };

    //
    // KL-Divergence with Dirichlet smoothing
    //

    template<>
    class BaseComparisonValueFunction<KLDivergenceSimilarityFunction,
                                      DirichletDocumentSmoothingFunction> {
      double _mu;
      const UnigramCollection& _collection;
    
    public:
      BaseComparisonValueFunction( const KLDivergenceSimilarityFunction& kl,
                                   const DirichletDocumentSmoothingFunction& dir,
                                   const UnigramCollection& collection ) :
        _collection(collection),
        _mu( dir.mu() )
      {
      }

      double baseResult ( const SmoothedUnigramDocument& one,
                          const SmoothedUnigramDocument& two ) const {
        double outerTerm = _mu / ( _mu + one.length );
        double logMu = log( _mu );
        double logMuPlusTwo = log( _mu + two.length );
        
        return outerTerm * ( logMu - logMuPlusTwo + _collection.entropy() );
      }

      double scaleResult( double result ) const {
        return result;
      }
    };

    //
    // KL-Divergence with no smoothing
    //

    template<>
    class BaseComparisonValueFunction<KLDivergenceSimilarityFunction,
                                      NullDocumentSmoothingFunction> {
      double _mu;
    
    public:
      BaseComparisonValueFunction( const KLDivergenceSimilarityFunction& kl,
                                   const NullDocumentSmoothingFunction& nl,
                                   const UnigramCollection& collection )
      {
      }

      double baseResult ( const SmoothedUnigramDocument& one,
                          const SmoothedUnigramDocument& two ) const {
        return 0;
      }

      double scaleResult( double result ) const {
        return result;
      }
    };

    //
    // Diffusion kernel with Jelinek-Mercer smoothing
    //

    template<>
    class BaseComparisonValueFunction<DiffusionDocumentSimilarityFunction,
                                      JelinekMercerDocumentSmoothingFunction> {
    private:
      double _lambda;
      double _oneOverT;

    public:
      BaseComparisonValueFunction( const DiffusionDocumentSimilarityFunction& dsf, 
                                   const JelinekMercerDocumentSmoothingFunction& jm,
                                   const UnigramCollection& collection ) :
        _lambda( jm.lambda() ),
        _oneOverT( 1.0 / dsf.t() )
      {
      }

      double baseResult ( const SmoothedUnigramDocument& one,
                          const SmoothedUnigramDocument& two ) const {
        return _lambda;
      }
      
      double scaleResult( double result ) const {
        return _oneOverT * exp( result );
      }
    };

    //
    // Diffusion kernel with Dirichlet smoothing
    //

    template<>
    class BaseComparisonValueFunction<DiffusionDocumentSimilarityFunction,
                                      DirichletDocumentSmoothingFunction> {
    private:
      double _mu;
      double _oneOverT;

    public:
      BaseComparisonValueFunction( const DiffusionDocumentSimilarityFunction& dsf, 
                                   const DirichletDocumentSmoothingFunction& dir,
                                   const UnigramCollection& collection ) :
        _mu( dir.mu() ),
        _oneOverT( 1.0 / dsf.t() )
      {
      }

      double baseResult ( const SmoothedUnigramDocument& one,
                          const SmoothedUnigramDocument& two ) const {
        return _mu / ( _mu + double(one.length) );
      }
      
      double scaleResult( double result ) const {
        return _oneOverT * exp( result );
      }
    };

    //
    // Diffusion kernel with no smoothing
    //

    template<>
    class BaseComparisonValueFunction<DiffusionDocumentSimilarityFunction,
                                      NullDocumentSmoothingFunction> {
      double _oneOverT;
    
    public:
      BaseComparisonValueFunction( const DiffusionDocumentSimilarityFunction& dsf,
                                   const NullDocumentSmoothingFunction& nl,
                                   const UnigramCollection& collection ) :
        _oneOverT( 1.0 / dsf.t() )
      {
      }

      double baseResult ( const SmoothedUnigramDocument& one,
                          const SmoothedUnigramDocument& two ) const {
        return 0;
      }

      double scaleResult( double result ) const {
        return _oneOverT * exp( result );
      }
    };
  }
}

#endif // INDRI_SIMILARITY_BASECOMPARISONVALUEFUNCTION_HPP


