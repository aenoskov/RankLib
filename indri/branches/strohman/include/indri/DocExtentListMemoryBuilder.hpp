
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
      void reset( const greedy_vector< std::pair<char*,char*>, 4 >& lists, bool numeric );
      void reset( const class DocExtentListMemoryBuilder& builder );

      DocExtentListMemoryBuilderIterator( const class DocExtentListMemoryBuilder& builder ); 
      
      void startIteration();
      bool finished();
      bool nextEntry( int documentID );
      bool nextEntry();
      indri::index::DocExtentListIterator::DocumentExtentData* currentEntry();
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
