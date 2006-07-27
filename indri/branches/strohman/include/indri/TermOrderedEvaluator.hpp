
//
// TermOrderedEvaluator
//
// 25 July 2006 -- tds
//


#include "indri/Index.hpp"
#include "indri/Walker.hpp"
#include "indri/QuerySpec.hpp"
#include "indri/Repository.hpp"
#include "indri/ScoredExtentResult.hpp"
#include "indri/TermScoreFunction.hpp"
#include "indri/InferenceNetwork.hpp"
#include <vector>
#include <map>

namespace indri {
  namespace infnet {
    class TermOrderedEvaluator : public indri::lang::Walker {
    private:
      indri::collection::Repository& _repository;
    
    public:
      struct Extent {
        Extent( int begin, int end ) {
          this->begin = begin;
          this->end = end;
        }
 
        bool operator< ( const Extent& other ) const {
          return this->begin < other.begin;
        }

        int begin;
        int end;
      };

      struct InvertedList {
        UINT64 totalCount() {
          UINT64 count = 0;
          for( int i=0; i<counts.size(); i++ ) {
            count += counts[i];
          }
          return count;
        }

        std::vector<int> documents;
        std::vector<int> counts;
        std::vector<Extent> positions;
      };
      
      struct ScoredList {
        ScoredList() : function(0) {}
        ~ScoredList() {
          delete function;
        }
      
        std::vector<indri::api::ScoredExtentResult> results;
        indri::query::TermScoreFunction* function;
      };
     
    private:
      std::map<indri::lang::Node*, InvertedList*> _listMap;
      std::map<indri::lang::Node*, ScoredList*> _scoreMap;
      indri::infnet::InferenceNetwork::MAllResults _results;
      
      bool _allSameDocument( std::vector<TermOrderedEvaluator::InvertedList*>& lists, std::vector<int>& indexes );
      void _advanceToDocument( const std::vector<TermOrderedEvaluator::InvertedList*>& lists, std::vector<int>& dind, std::vector<int>& cind, int document );
      int _listMaxDocument( std::vector<TermOrderedEvaluator::InvertedList*>& lists, std::vector<int>& indexes );
      
      indri::query::TermScoreFunction* _buildTermScoreFunction( const std::string& smoothing, double occurrences, double contextSize ) const;
      
    public:
      TermOrderedEvaluator( indri::collection::Repository& repository );
      ~TermOrderedEvaluator();
      
      void defaultAfter( indri::lang::Node* node );
      
      void after( indri::lang::IndexTerm* term );
      void after( indri::lang::Field* field );   
      void after( indri::lang::RawScorerNode* scorer );
      void after( indri::lang::ScoreAccumulatorNode* scorer );
      void after( indri::lang::ContextSimpleCounterNode* simple );
      void after( indri::lang::ContextCounterNode* contextCounter );
      void after( indri::lang::CombineNode* combine );
      void after( indri::lang::WeightNode* weight );
      void after( indri::lang::WSumNode* weight );
      void after( indri::lang::ODNode* odNode );
      void after( indri::lang::UWNode* uwNode );
      void after( indri::lang::ExtentInside* exInside );
      
      indri::infnet::InferenceNetwork::MAllResults getResults();
       
    // ContextCounterNode
    // ContextSimpleCounterNode
    // ExtentInside
    // WeightedExtentOr
    // ExtentOr
    // ExtentAnd
    // UWNode
    // ODNode
    // FilReqNode
    // FilRejNode
    // TermFrequencyScorerNode
    // ScoreAccumulatorNode
    // ListAccumulator
    // WeightNode
    // WAndNode
    // RawScorerNode
    // WSumNode
   
    };
  }
}