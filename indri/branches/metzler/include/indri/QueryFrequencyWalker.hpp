//
// QueryFrequencyWalker
//
// 23 October 2004 -- dam
//

#include <vector>

#ifndef INDRI_QUERYFREQUENCYWALKER_HPP
#define INDRI_QUERYFREQUENCYWALKER_HPP
namespace indri
{
  namespace lang
  {

    class QueryFrequencyWalker : public indri::lang::Walker {
    private:
      // keep track of nodes visited
      std::vector<indri::lang::StatisticsNode*> _nodes;
      
      // keep track of query text counts
      std::map<std::string, int> _counter;
      
      // query length
      int _queryLength;
      
      void _processNode( indri::lang::StatisticsNode* statsNode ) {
	std::string qtext = statsNode->queryText();
	std::map<std::string, int>::iterator iter = _counter.find( qtext );
	if( iter == _counter.end() )
	  _counter[ qtext ] = 1;
	else
	  _counter[ qtext ] = iter->second + 1;
	
	_queryLength++;
  	_nodes.push_back( statsNode );
      }
      
    public:
      QueryFrequencyWalker() : _queryLength( 0 ) {}
      ~QueryFrequencyWalker() {}
      
      void setStatistics() {
  	for(int i = 0; i < _nodes.size(); i++ ) {
	  //std::cout << "text = " << _nodes[i]->queryText() << ", qf = " << _counter[ _nodes[i]->queryText() ] << ", queryLength = " << _queryLength << std::endl;
	  _nodes[i]->setQF( _counter[ _nodes[i]->queryText() ] );
	  _nodes[i]->setQueryLength( _queryLength );
  	}
      }
      
      void after( indri::lang::RawScorerNode* statsNode ) {
  	_processNode( statsNode );
      }
      
      void after( indri::lang::TermFrequencyScorerNode* statsNode ) {
  	_processNode( statsNode );
      }
      
      void after( indri::lang::CachedFrequencyScorerNode* statsNode ) {
  	_processNode( statsNode );
      }
      
    };
  }
}

#endif // INDRI_QUERYFREQUENCYWALKER_HPP

