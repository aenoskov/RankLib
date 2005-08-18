/*==========================================================================
 * Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
 */

//
// FrequencyAccumulatorCopier
//
// 29 July 2005 -- tds
//

#ifndef INDRI_FREQUENCYACCUMULATORCOPIER_HPP
#define INDRI_FREQUENCYACCUMULATORCOPIER_HPP

#include <iostream>

namespace indri
{
  namespace lang 
  {
    class FrequencyAccumulatorCopier : public indri::lang::Copier {
    private:
      std::vector<indri::lang::Node*> _nodes;

    public:
      ~FrequencyAccumulatorCopier() {
        indri::utility::delete_vector_contents( _nodes );
      }

      indri::lang::Node* defaultAfter( indri::lang::Node* old, indri::lang::Node* newNode ) {
        _nodes.push_back( newNode );
        return newNode;
      }

      indri::lang::Node* after( indri::lang::ScoreAccumulatorNode* oldAccumulatorNode, indri::lang::ScoreAccumulatorNode* newAccumulatorNode ) {
        // if we have a WeightNode child, and it has children that are only TermFrequencyNodes, we should swap in the TermFrequencyAccumulatorNode
        indri::lang::WeightNode* childWeight = dynamic_cast<indri::lang::WeightNode*>( newAccumulatorNode->getChild() );
        indri::lang::CombineNode* childCombine = dynamic_cast<indri::lang::CombineNode*>( newAccumulatorNode->getChild() );
        indri::lang::TermFrequencyScorerNode* childTermFrequency = dynamic_cast<indri::lang::TermFrequencyScorerNode*>( newAccumulatorNode->getChild() );
        
        if( !childWeight && !childCombine && !childTermFrequency ) {
          // child isn't a WeightNode, so we can't replace it
          std::cout << "not weight, combine, or tf: " << newAccumulatorNode->getChild()->typeName() << std::endl;
          _nodes.push_back( newAccumulatorNode );
          return newAccumulatorNode;
        }

        std::vector< std::pair<double, indri::lang::ScoredExtentNode*> > children;

        if( childWeight ) {
          children = childWeight->getChildren();
        } else if( childCombine ) {
          // child is a combine node
          const std::vector< indri::lang::ScoredExtentNode* >& kids = childCombine->getChildren();

          for( int i=0; i<kids.size(); i++ ) {
            children.push_back( std::make_pair( 1.0 / double(kids.size()), kids[i] ) );
          }
        } else if( childTermFrequency ) {
          children.push_back( std::make_pair( 1.0, childTermFrequency ) );
        }

        for( int i=0; i<children.size(); i++ ) {
          indri::lang::TermFrequencyScorerNode* tfs = dynamic_cast<indri::lang::TermFrequencyScorerNode*>( children[i].second );

          if( !tfs ) {
            // found a node that's not a term frequency node, so we can't continue;
            std::cout << "found non-tfs" << std::endl;
            _nodes.push_back( newAccumulatorNode );
            return newAccumulatorNode;
          }
        }

        // everything checks out, so make a TermFrequencyAccumulatorNode instead
        indri::lang::TermFrequencyAccumulatorNode* accumulator = new indri::lang::TermFrequencyAccumulatorNode( children );
        accumulator->setNodeName( newAccumulatorNode->nodeName() );
        delete newAccumulatorNode;
        _nodes.push_back( accumulator );

        return accumulator;
      }
    };
  }
}

#endif // INDRI_FREQUENCYACCUMULATORCOPIER_HPP

