/*==========================================================================
  Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.

  Use of the Lemur Toolkit for Language Modeling and Information Retrieval
  is subject to the terms of the software license set forth in the LICENSE
  file included with this software, and also available at
  http://www.cs.cmu.edu/~lemur/license.html 
  as well as the conditions below.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  3. The names "Indri", "Center for Intelligent Information Retrieval", 
  "CIIR", and "University of Massachusetts" must not be used to
  endorse or promote products derived from this software without
  prior written permission. To obtain permission, contact
  indri-info@ciir.cs.umass.edu.

  4. Products derived from this software may not be called "Indri" nor 
  may "Indri" appear in their names without prior written permission of 
  the University of Massachusetts. To obtain permission, contact 
  indri-info@ciir.cs.umass.edu.

  THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF MASSACHUSETTS AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
  ==========================================================================
*/


//
// ContextSimpleCountCollectorCopier
//
// 5 March 2004 -- tds
//
// This copier uses a IndriIndex to extract context
// counts for certain simple subgraphs.  It can compute
// counts for the following types of expressions:
//
//   dog
//   <dog cat>
//   dog.title
//   <dog cat>.title
//   dog.(title)
//   <dog cat>.(title)
//   
// Notably, it is unable to compute counts when more than
// one field is involved.
//

#ifndef INDRI_CONTEXTSIMPLECOUNTCOLLECTORCOPIER_HPP
#define INDRI_CONTEXTSIMPLECOUNTCOLLECTORCOPIER_HPP

#include "indri/QuerySpec.hpp"
#include "indri/Copier.hpp"
#include "indri/delete_range.hpp"

class ContextSimpleCountCollectorCopier : public indri::lang::Copier {
private:
  std::vector<indri::lang::Node*> _newNodes;

  class SubtreeWalker : public indri::lang::Walker {
  private:
    bool _computable;
    int _extentOrs;
    bool _hasContext;

    std::vector<indri::lang::IndexTerm*> _terms;
    indri::lang::Field* _field;

  public:
    SubtreeWalker() :
      _computable(true),
      _extentOrs(0),
      _field(0)
    {
    }

    bool isComputable() {
      return _computable && _terms.size();
    }

    std::vector<indri::lang::IndexTerm*>& getTerms() {
      return _terms;
    }

    indri::lang::Field* getField() {
      return _field;
    }

    bool hasContext() const {
      return _hasContext;
    }

    void defaultBefore( indri::lang::Node* node ) {
      // this means that we're seeing some node type that
      // we aren't otherwise trapping--that means this subtree
      // is surely not precomputable
      _computable = false;
    }

    void before( indri::lang::ContextCounterNode* contextNode ) {
      // if the context node has a context, then it must have a field in the context
      // if we find more than one field, we say this isn't computable.  Therefore, if
      // this subtree is computable and it has a context, the single field must be in the context.
      _hasContext = contextNode->getContext() ? true : false;
    }
    
    void before( indri::lang::ExtentOr* extentOrNode ) {
      _extentOrs++;
    }

    void after( indri::lang::ExtentOr* extentOrNode ) {
      _extentOrs--;
    }

    void before( indri::lang::ExtentAnd* extentAndNode ) {
      // we definitely can't deal with any "true" extentAnds
      // however, if this is just an and wrapper around a single
      // field, we won't let it fool us
      if( extentAndNode->getChildren().size() > 1 )
        _computable = false;
    }

    void before( indri::lang::Field* fieldNode ) {
      if( _extentOrs || _field ) {
        // fields can't be or-ed together; only terms can (_extentOr)
        // If we already saw a field, then this one proves that the tree isn't computable (_field)
        _computable = false;
      }

      _field = fieldNode;
    }

    void before( indri::lang::IndexTerm* termNode ) {
      _terms.push_back(termNode);
    }

    void before( indri::lang::ExtentInside* insideNode ) {
      // ignore this; the other checks should catch any bad trees
      // without having to worry about checking here
    }
  };

public:
  ContextSimpleCountCollectorCopier()
  {
  }

  ~ContextSimpleCountCollectorCopier() {
    delete_vector_contents<indri::lang::Node*>( _newNodes );
  }

  indri::lang::Node* defaultAfter( indri::lang::Node* oldNode, indri::lang::Node* newNode ) {
    _newNodes.push_back( newNode );
    return newNode;
  }

  indri::lang::Node* after( indri::lang::ContextCounterNode* contextNode, indri::lang::ContextCounterNode* newNode ) {
    // first, walk the subtree to find out if it's computable
    SubtreeWalker subtree;
    contextNode->walk(subtree);
    indri::lang::Node* result = newNode;
  
    if( subtree.isComputable() ) {
      // terms
      std::vector<std::string> terms;
      for( int i=0; i<subtree.getTerms().size(); i++ ) {
        terms.push_back( subtree.getTerms()[i]->getText() );
      }

      std::string field;
      std::string context;

      if( subtree.hasContext() ) {
        context = subtree.getField()->getFieldName();
      } else if( subtree.getField() ) {
        field = subtree.getField()->getFieldName();
      }

      result = new indri::lang::ContextSimpleCounterNode( terms, field, context );
      result->setNodeName( contextNode->nodeName() );
      delete newNode;
    }

    // if it wasn't computable, keep the subtree around so the
    // inference network code can run it and figure out the counts
    _newNodes.push_back( result );
    return result;
  }
};

#endif // INDRI_CONTEXTSIMPLECOUNTCOLLECTORCOPIER_HPP

