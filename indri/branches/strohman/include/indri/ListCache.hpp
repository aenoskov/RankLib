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
// ListCache
//
// 24 September 2004 -- tds
//
// Stores previously used precomputed lists.
//

#ifndef INDRI_LISTCACHE_HPP
#define INDRI_LISTCACHE_HPP

#include <vector>
#include "indri/delete_range.hpp"
#include "indri/SimpleCopier.hpp"
#include "indri/DocumentCount.hpp"
#include "indri/QuerySpec.hpp"
#include "indri/TreePrinterWalker.hpp"
#include "indri/Parameters.hpp"
#include "indri/ScopedLock.hpp"
#include "indri/Mutex.hpp"
#include "indri/ref_ptr.hpp"

namespace indri
{
  namespace lang
  {
    class ListCache {
    public:
      struct CachedList {
        // hash codes
        UINT64 rawHash;
        UINT64 contextHash;

        // query structure
        indri::lang::SimpleCopier raw;
        indri::lang::SimpleCopier context;

        // postings
        indri::utility::greedy_vector<indri::index::DocumentContextCount> entries;

        // statistics about the entries
        double occurrences;
        int contextSize;
        int maximumContextSize;
        double maximumContextFraction;
      };

    private:
      std::vector< indri::atomic::ref_ptr<struct CachedList> > _lists;
      indri::thread::Mutex _mutex;
  
    public:
      void insert( CachedList* list ) {
        indri::thread::ScopedLock sl( _mutex );

        while( _lists.size() > 100 ) {
          _lists.erase( _lists.begin() );
        }

        indri::atomic::ref_ptr<CachedList> p = list;
        _lists.push_back( p );
      }

      indri::atomic::ref_ptr<CachedList> find( indri::lang::Node* raw, indri::lang::Node* context ) {
        indri::thread::ScopedLock sl( _mutex );

        ListCache::CachedList* list = 0;
        size_t i = 0;

        UINT64 rawHash = raw->hashCode();
        UINT64 contextHash = context ? context->hashCode() : 0;

        for( i=0; i<_lists.size(); i++ ) {
          if( rawHash != _lists[i]->rawHash ||
              contextHash != _lists[i]->contextHash )
            continue;
          
          indri::lang::Node* cachedRaw = _lists[i]->raw.root();
          indri::lang::Node* cachedContext = _lists[i]->context.root();

          if( *cachedRaw == *raw ) {
            if( ( !cachedContext && !context ) ||
                ( cachedContext && context && (*context == *cachedContext)) ) {
              return _lists[i];
            } 
          }
        }

        return indri::atomic::ref_ptr<CachedList>();
      }
    };
  }
}

#endif // INDRI_LISTCACHE_HPP


