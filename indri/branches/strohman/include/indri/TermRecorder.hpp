
//
// TermRecorder
//
// 14 January 2005 -- tds
//

#include "indri/Buffer.hpp"
#include <vector>
#include <utility>

#ifndef INDRI_TERMRECORDER_HPP
#define INDRI_TERMRECORDER_HPP

namespace indri {
  namespace index {
    class TermRecorder {
    private:
      struct less {
        bool operator () ( const std::pair<const char*, int>& one, const std::pair<const char*, int>& two ) {
          return strcmp( one.first, two.first ) < 0;
        }
      };

      Buffer _buffer;
      std::vector< std::pair<const char*,int> > _pairs;

    public:
      void add( int sequence, const char* term ) {
        int termLength = strlen( term );
        char* termSpot = _buffer.write( termLength+1 );
        strcpy( termSpot, term );

        _pairs.push_back( std::make_pair( term, sequence ) );
      }

      void sort() {
        std::sort( _pairs.begin(), _pairs.end(), less() );
      }

      void buildMap( std::vector<int>& map, TermRecorder& other, std::vector< std::pair< const char*, int > >* missing = 0 ) {
        map.resize( _pairs.size(), -1 );
        int i = 0;
        int j = 0;
        std::vector< std::pair<const char*, int > >& otherPairs = other._pairs;

        // this joins all matching pairs
        while( i < otherPairs.size() && j < _pairs.size() ) {
          int result = strcmp( otherPairs[i].first, _pairs[j].first );
          
          if( result == 0 ) {
            map[ _pairs[j].second ] = otherPairs[i].second;
            i++;
            j++;
          } else if( result < 0 ) {
            i++;
          } else {
            if( missing )
              missing->push_back( _pairs[j] );

            j++;
          }
        }

        while( missing && j < _pairs.size() ) {
          missing->push_back( _pairs[j] );
          j++;
        }
      }

      std::vector< std::pair<const char*, int> >& pairs() {
        return _pairs;
      }
    };
  }
}

#endif // INDRI_TERMRECORDER_HPP

