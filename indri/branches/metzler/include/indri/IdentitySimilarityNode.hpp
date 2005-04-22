//
// IdentitySimilarityNode
//
// 21 October 2004 -- dam
//
// Computes an "identity measure" similarity
// as described in:
//
// Methods for Identifying Plagiarized Text
// Hoad and Zobel 
//

#ifndef INDRI_IDENTITYSIMILARITYNODE_HPP
#define INDRI_IDENTITYSIMILARITYNODE_HPP

#include <math.h>
#include "indri/BeliefNode.hpp"
#include "indri/StatisticsBeliefNode.hpp"
#include <vector>
#include "indri/greedy_vector"
namespace indri
{
  namespace infnet
  {
    
    class IdentitySimilarityNode : public BeliefNode {
    private:
      std::vector<StatisticsBeliefNode*> _children;
      indri::utility::greedy_vector<indri::api::ScoredExtentResult> _scores;
      std::string _name;
      
      int _variation;
      
      int my_abs( int );
      double my_abs( double );
      
      indri::utility::greedy_vector<bool> _matches;
      
    public:
      IdentitySimilarityNode( const std::string& name, int variation = 1 );
      IdentitySimilarityNode( const std::string& name, const std::vector<StatisticsBeliefNode*>& children, int variation = 1 );
      
      const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& score( int documentID, int begin, int end, int documentLength );
      void annotate( class Annotator& annotator, int documentID, int begin, int end );
      
      // these nodes don't support max-score skipping
      double maximumScore() { return INDRI_HUGE_SCORE; }
      double maximumBackgroundScore() { return INDRI_TINY_SCORE; }
      
      bool hasMatch( int documentID );
      const indri::utility::greedy_vector<bool>& hasMatch( int documentID, const indri::utility::greedy_vector<indri::index::Extent>& extents );
      
      int nextCandidateDocument();
      const std::string& getName() const;
      
      void indexChanged( indri::index::Index& index ) { /* do nothing for now -- fix this! */ }
      
      void setVariation( int variation ) { _variation = variation; }
      int getVariation() { return _variation; }
    };
  }
}

#endif // INDRI_IDENTITYSIMILARITYNODE_HPP

