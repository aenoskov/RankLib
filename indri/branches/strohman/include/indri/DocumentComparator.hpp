
//
// DocumentComparator
//
// 22 February 2006 -- tds
//

#ifndef INDRI_SIMILARITY_DOCUMENTCOMPARATOR_HPP
#define INDRI_SIMILARITY_DOCUMENTCOMPARATOR_HPP

#include "indri/BaseComparisonValueFunction.hpp"

/**
 * Upon reading this code for the first time, it would be natural to ask the questions:
 * "Why are there all these template things all over the place?" and
 * "How come document similarity is computed in this convoluted way?".  I'll try to answer those
 * questions.
 *
 * First, templates are used here because the alternative would be a bunch of virtual function 
 * calls.  Since a lot of these function calls are rather short, the function call overhead would
 * be high.  By using templates, we give the compiler the ability to inline these things.  In the 
 * process, the compiler can do dead code elimination in some cases for simplified models; for instance,
 * if you were to write a smoothing method that didn't actually smooth anything, a lot of the
 * multiplies in DocumentComparator::compare would be optimized away.  If this wasn't a template,
 * the compiler would not be able to perform that elimination.
 *
 * So, why is similarity computed in this convoluted way?  It's because document similarity of two
 * vectors is (in the naive implementation) a rather expensive process.  For instance, the 
 * KL-divergence of two multinomial distributions is a sum over all non-zero probabilities in
 * both distributions.  Once you add smoothing, every probability is non-zero.  For big collections,
 * that could mean many millions of calculations per document.
 *
 * It's possible to get around this by realizing that there is a kind of base value for the similarity
 * between two documents, giving a smoothing method and a similarity model.  In this code, the 'base value'
 * is computed by the BaseComparisonValueFunction, which is unique for each combination of 
 * smoothing type and similarity model.  As a simple example, consider Jelinek-Mercer (linear) smoothing
 * with cosine similarity (dot product).  Let p_i be the maximum likelihood distribution for document 1,
 * while p'_i is the ML distribution for document 2; and c_i is then the collection distribution.  The 
 * smoothed cosine similarity is computed as:
 *
 * \sum_i ( ( 1 - \lambda ) p_i + \lambda c_i ) * ( ( 1 - \lambda ) p'_i + \lambda c_i )
 *
 * Suppose that the p_i and p'_i values are all 0 (this is kind of a strange situation, since we usually
 * assume that these distributions sum to 1, but bear with me).  Then, this computation becomes:
 *
 * \sum_i ( \lambda c_i )^2
 * 
 * We can pull the lambda out of the sum, giving:
 * 
 * \lambda^2 \sum_i c_i^2
 *
 * This is the base value of the comparison; it's how similar two documents are that have maximum
 * likelihood distributions that are identically zero.  The DocumentComparator::compare function assumes
 * we have this kind of score.  In a sense, this score is correct for all terms i where the maximum
 * likelihood estimates from each document are zero.  We just have to fix it up for all the terms
 * t_i where one of the documents in question contains t_i.  You can see that that's exactly what 
 * DocumentComparator::compare does.  Now, the sum is over only the terms that occur in either document,
 * not all the terms that appear in the collection.
 */


namespace indri {
    namespace similarity {
        class AbstractDocumentComparator {
        public:
            virtual bool symmetric() const = 0;
            virtual void smooth( SmoothedUnigramDocument& smoothed, UnigramDocument& document ) const = 0;
            virtual double compare( const SmoothedUnigramDocument& one, const SmoothedUnigramDocument& two ) const = 0;
        };

        template< typename _SimilarityType, typename _SmoothingType >
        class DocumentComparator : public AbstractDocumentComparator {
        private:
            const UnigramCollection& _collection;
            const _SimilarityType* _similarity;
            const _SmoothingType* _smoothing;
            BaseComparisonValueFunction<_SimilarityType, _SmoothingType> _baseFunction;

        public:
            DocumentComparator( const _SimilarityType* similarity, const _SmoothingType* smoothing, const UnigramCollection& collection ) :
                _smoothing( smoothing ),
                _similarity( similarity ),
                _collection( collection ),
                _baseFunction( *_similarity, *_smoothing, collection ) 
            {
            }

            ~DocumentComparator() {
                delete _smoothing;
                delete _similarity;
            }

            bool symmetric() const {
                return _similarity->symmetric();
            }

            void smooth( SmoothedUnigramDocument& smoothed, UnigramDocument& document ) const {
                (*_smoothing)( smoothed, document );
            }

            double compare( const SmoothedUnigramDocument& one, const SmoothedUnigramDocument& two ) const {
                int i=0;
                int j=0;

                assert( one.probabilities.size() == one.background.size() );
                assert( two.probabilities.size() == two.background.size() );

                double result = _baseFunction.baseResult( one, two );
                
                while( i < one.terms.size() && j < two.terms.size() ) {
                    int oneTerm = one.terms[i];
                    int twoTerm = two.terms[j];

                    if( oneTerm == twoTerm ) {
                        // terms match
                        result += _similarity->bothMatch( one.probabilities[i], two.probabilities[j],
                                                          one.background[i], two.background[j] );

                        i++; j++;
                    } else if ( oneTerm < twoTerm ) {
                        // a term appears in document one that does not appear in document two
                        result += _similarity->firstMatches( one.probabilities[i], one.background[i], 
                                                             _smoothing->background( two, oneTerm ) );
                        i++;
                    } else {
                        // a term appears in document two that does not appear in document one
                        result += _similarity->secondMatches( two.probabilities[j], two.background[j], 
                                                              _smoothing->background( one, twoTerm ) );
                        j++;
                    }
                }

                while( i < one.terms.size() ) {
                    int oneTerm = one.terms[i];

                    // look at all remaining terms in document one
                    result += _similarity->firstMatches( one.probabilities[i], one.background[i], 
                                                         _smoothing->background( two, oneTerm ) );
                    i++;
                }

                while( j < two.terms.size() ) {
                    int twoTerm = two.terms[j];

                    // look at all remaining terms in document two
                    result += _similarity->secondMatches( two.probabilities[j], two.background[j], 
                                                          _smoothing->background( one, twoTerm ) );
                    j++;
                }

                return _baseFunction.scaleResult( result );
            }
        };
    }
}

#endif // INDRI_SIMILARITY_DOCUMENTCOMPARATOR_HPP

