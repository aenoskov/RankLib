//
// StatisticsBeliefNode
//
// 23 October 2004 -- dam
//
// designed to encapsulate and centralize
// the statistics associated with belief nodes

#ifndef INDRI_STATISTICSBELIEFNODE_HPP
#define INDRI_STATISTICSBELIEFNODE_HPP

#include "indri/BeliefNode.hpp"
namespace indri
{
  namespace infnet
  {

    class StatisticsBeliefNode : public BeliefNode {
    protected:
      // number of matches in entire collection
      double _occurrences;
      
      // number of documents this node matches
      INT64 _docOccurrences;
      
      // number of extents this node matches
      //INT64 _extentOccurrences;
      
      // size of the context this node's scored under
      double _totalContextSize;
      
      // total number of times the query text associated
      // with this node occurs in the query
      int _qf;
      
      // total number of documents in the collection
      int _docCount;
      
      // total number of StatisticsNodes within the network
      // (before optimization / unnecessary node removal)
      int _queryLength;
      
    public:
      // returns the number of times this node matches in the
      // current document within the given given extent range  
      virtual double matches( int begin, int end ) = 0;
      
      // returns the size of the current document's
      // scorable context
      virtual double contextSize( int begin, int end ) = 0;
      
      void setStatistics( double occurrences, double contextSize, INT64 docOccurrences,
			  int qf, int docCount, int queryLength ) {
  	_occurrences = occurrences;
  	_totalContextSize = contextSize;
  	_docOccurrences = docOccurrences;
  	_qf = qf;
  	_docCount = docCount;
  	_queryLength = queryLength;
      }
      
      double getOccurrences() { return _occurrences; }
      INT64 getDocOccurrences() { return _docOccurrences; }
      //INT64 getExtentOccurrences() { return _extentOccurrences; }
      double getContextSize() { return _totalContextSize; }
      int getQF() { return _qf; }
      int getDocCount() { return _docCount; }
      int getQueryLength() { return _queryLength; }
    };
  }
}
    
#endif // INDRI_STATISTICSBELIEFNODE_HPP
    
    
