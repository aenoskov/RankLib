
//
// DocExtentListMemoryBuilder
//
// 24 November 2004 -- tds
//

#ifndef INDRI_DOCEXTENTLISTMEMORYBUILDER_HPP
#define INDRI_DOCEXTENTLISTMEMORYBUILDER_HPP

#include "indri/indri-platform.h"
#include "greedy_vector"
#include "indri/DocExtentListIterator.hpp"
#include <utility>
#include "lemur/RVLCompress.hpp"

namespace indri {
  namespace index {
    class DocExtentListMemoryBuilderIterator : public DocExtentListIterator {
      const greedy_vector< std::pair<char*, char*>, 4 >* _lists;
      greedy_vector< std::pair<char*, char*>, 4 >::const_iterator _current;
      indri::index::DocExtentListIterator::DocumentExtentData _data;
      
      const char* _list;
      const char* _listEnd;
      bool _numeric;

    public:
      void reset( const greedy_vector< std::pair<char*,char*>, 4 >& lists, bool numeric ) {
        _lists = &lists;
        _numeric = numeric;
        
        _current = _lists->begin();
        
        if( _current != _lists->end() ) {
          _list = _current->first;
          _listEnd = _current->second;
        } else {
          _list = 0;
          _listEnd = 0;
        }
        
        _data.document = 0;
        _data.numbers.clear();
        _data.extents.clear();

        nextEntry();
      }
      
      DocExtentListMemoryBuilderIterator() {
      }

      DocExtentListMemoryBuilderIterator( const greedy_vector< std::pair<char*,char*>, 4 >& lists, bool numeric ) {
        reset( lists, numeric );
      }
      
      bool finished() {
        return _current == _lists->end() && _list == _listEnd;
      }

      bool nextEntry( int documentID ) {
        do {
          if( _data.document >= documentID )
            return true;
        }
        while( nextEntry() );
        
        return false;
      }
      
      bool nextEntry() {
        if( _list < _listEnd ) {
          int deltaDocument;
          int extents;
          
          _list = RVLCompress::decompress_int( _list, deltaDocument );
          _data.document += deltaDocument;
          _data.extents.clear();
          _data.numbers.clear();

          _list = RVLCompress::decompress_int( _list, extents );

          int deltaPosition;
          Extent extent;
          extent.begin = 0;
          extent.end = 0;

          for( int i=0; i<extents; i++ ) {
            _list = RVLCompress::decompress_int( _list, deltaPosition );
            extent.begin = extent.end + deltaPosition;
            _list = RVLCompress::decompress_int( _list, deltaPosition );
            extent.end = extent.begin + deltaPosition;

            _data.extents.push_back( extent );

            if( _numeric ) {
              INT64 number;
              _list = RVLCompress::decompress_signed_longlong( _list, number );
              _data.numbers.push_back( number );
            }
          }
        } else {    
          assert( _list == _listEnd );

          // no data left, go to the next segment
          if( _current != _lists->end() )
            _current++;
          
          if( _current != _lists->end() ) {
            _list = _current->first;
            _listEnd = _current->second;
            return nextEntry();
          }

          // no more list segments
          return false;
        }
      }
      
      indri::index::DocExtentListIterator::DocumentExtentData* currentEntry() {
        return &_data;
      }
    };

    class DocExtentListMemoryBuilder {
    public:
      typedef DocExtentListMemoryBuilderIterator iterator;

      int _documentFrequency;
      int _extentFrequency;

      char* _list;
      char* _listBegin;
      char* _listEnd;

      int _lastLocation;
      int _lastDocument;
      int _lastExtentFrequency;

      char* _documentPointer;
      char* _locationCountPointer;

      bool _numeric;

      greedy_vector< std::pair<char*,char*>, 4 > _lists;

      inline size_t _compressedSize( int documentID, int begin, int end, INT64 number );
      inline void _safeAddLocation( int documentID, int begin, int end, INT64 number );
      void _growAddLocation( int documentID, int begin, int end, INT64 number, size_t newDataSize );
      void _grow();
      void _terminateDocument();

    public:
      DocExtentListMemoryBuilder( bool numeric );
      ~DocExtentListMemoryBuilder();

      void addLocation( int documentID, int begin, int end, INT64 number );
      void addLocation( int documentID, int begin, int end );

      void clear();
      void close();
      bool empty();

      int documentFrequency() const;
      int extentFrequency() const;
      size_t memorySize() const;
    };
  }
}

#endif // INDRI_DOCEXTENTLISTMEMORYBUILDER_HPP
