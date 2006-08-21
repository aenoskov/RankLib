

//
// OverlappingExtentPassage
//
// 21 August 2006 -- tds
//

#ifndef INDRI_OVERLAPPINGEXTENTPASSAGE_HPP
#define INDRI_OVERLAPPINGEXTENTPASSAGE_HPP

#include "indri/BeliefNode.hpp"
#include "indri/FieldIteratorNode.hpp"
#include "indri/greedy_vector"
#include "indri/Extent.hpp"

namespace indri {
  namespace infnet {
    class OverlappingExtentPassage : public BeliefNode {
    private:
      BeliefNode* _child;
      FieldIteratorNode* _field;

      indri::utility::greedy_vector<indri::api::ScoredExtentResult> _scores;
      indri::utility::greedy_vector<bool> _matches;
      indri::utility::greedy_vector<indri::index::Extent> _subextents;

      std::string _name;

      int _windowSize;
      int _increment;

      void _scorePassage( int documentID, int begin, int end, int documentLength, int passageBegin, int passageEnd );

    public:
      OverlappingExtentPassage( const std::string& name, BeliefNode* child, FieldIteratorNode* field, int windowSize, int increment );

      int nextCandidateDocument();
      void indexChanged( indri::index::Index& index );

      double maximumScore();
      double maximumBackgroundScore();

      const indri::utility::greedy_vector<indri::api::ScoredExtentResult>& score( int documentID, int begin, int end, int documentLength );
      void annotate( class Annotator& annotator, int documentID, int begin, int end );

      const indri::utility::greedy_vector<bool>& hasMatch( int documentID, const indri::utility::greedy_vector<indri::index::Extent>& extents );
      bool hasMatch( int documentID );
      const std::string& getName() const;
    };
  }
}

#endif // INDRI_OVERLAPPINGEXTENTPASSAGE_HPP
