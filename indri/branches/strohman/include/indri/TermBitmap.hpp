
//
// TermBitmap
//
// 12 January 2005 -- tds
//

/*!
  TermBitmap is used to convert termIDs when many DiskIndexes are merged together.
  The add() function has very strict preconditions; both from and to must increase
  on every call, and from must always be less than to.

  This data is stored in 32-byte bitmap chunks with the following form:
    4 bytes - fromBase
    4 bytes - toBase
    24 bytes - bitmap

  Each bit set in the bitmap region corresponds to a (from, to) pair.  
  
  Suppose the beginning of the bitmap looks like this:
    000100100110000....
  This could be represented by the following pairs:
    (1, 4)
    (2, 7)
    (3, 10)
    (4, 11)
  For instance, the (2, 7) pair says that the second non-zero bit is
  at index 7.  This (2, 7) pair is translated to mean that
  (fromBase + 2, toBase + 7) is a pair stored by some explicit add() call.

  To save on heap overhead, we manage blocks of 64K each in Buffer objects, 
  which are stored in the vector called _maps.

  The TermBitmap is used because, in the ideal case, it is much more space
  efficient than the simpler approach of using an array mapping.  In an 
  array, we'd need 32 bits for each (from, to) pair.  In the case where
  the (from, to) pairs are optimally dense [e.g. (1,1), (2,2), (3,3) ... ],
  the TermBitmap uses 1.33 bits per pair.
*/

#ifndef INDRI_TERMBITMAP_HPP
#define INDRI_TERMBITMAP_HPP

#include "indri/Buffer.hpp"

namespace indri {
  namespace index {
    class TermBitmap {
    private:
      std::vector<Buffer*> _maps;
      int _fromBase;
      int _toBase;
      int _lastFrom;
      char* _current;

      void _addBufferIfNecessary() {
        if( !_maps.size() || _maps.back()->position() == _maps.back()->size() )
          _maps.push_back( new Buffer( 64*1024 ) );
      }

      Buffer* _findBuffer( int from ) {
        int left = 0;
        int right = _maps.size()-1;

        while( right - left > 1 ) {
          int middle = (right - left) / 2;
          Buffer* mid = _maps[ middle ];

          if( from < *(INT32*) mid->front() ) {
            right = middle;
          } else {
            left = middle;
          }
        }

        if( right == left )
          return _maps[left];

        int rightFront = *(INT32*) _maps[right]->front();

        if( from < rightFront )
          return _maps[left];

        return _maps[right];
      }

      const char* _findInBuffer( Buffer* b, int from ) {
        const char* start = b->front();
        const char* end = b->front() + b->position();

        while( end - start > 32 ) {
          const char* mid = start + (((end - start) / 2) & ~31);
          INT32 middle = *(INT32*) mid;

          if( from >= middle )
            start = mid;
          else
            end = mid;
        }

        INT32 front = *(INT32*)start;
        assert( from >= front && from < (front + 192) ); 

        return start;
      }

    public:
      TermBitmap() {
        _lastFrom = 0;
        _toBase = -10000;
        _fromBase = -10000;
      }

      void add( int to ) {
        add( _lastFrom+1, to );
      }

      void add( int from, int to ) {
        assert( from <= to );

        const int availableSpace = ((32 - 8) * 8);
        int difference = to - _toBase;

        assert( difference >= 0 );

        if( difference >= availableSpace || _lastFrom < from-1 ) {
          _addBufferIfNecessary();

          // get the current buffer
          Buffer* back = _maps.back();
          _current = back->write( 32 );

          *(INT32*)_current = from;
          *(INT32*)(_current + 4) = to;
          memset( _current + 8, 0, (32 - 8) ); 

          _toBase = to;
          _fromBase = from;
          difference = 0;
        }

        _current[8+difference/8] |= 1<<(difference%8);
        _lastFrom = from;

        assert( get(from) == to );
      }

      int get( int from ) {
        // first, binary search through the buffers themselves
        Buffer* buffer = _findBuffer( from );
        const char* spot = _findInBuffer( buffer, from );

        // now, we have to scan through this buffer to find the right value
        int fromBase = *(INT32*)spot;
        int toBase = *(INT32*)(spot+4);
        spot += 8;
        int bits = 0;
        int found = 0;

        while( (fromBase + found) < from ) {
          // find a non-zero bit that's past index <bits>
          unsigned char c;
          bits++;
          
          // this goes to the index bits in the array, masks off previous bits, looking for non-zero bits
          while( !(c = spot[bits/8] & (0xff << (bits%8))) )
            bits = (bits & ~7) + 8;
          
          assert( bits < 24*8 );
          assert( c );

          // find the non-zero bit in c
          for( int i=(bits%8); i<8; i++ ) {
            if( c & 1<<i ) {
              bits = (bits & ~7) + i;
              break;
            }
          }

          // found another bit, so increment found counter
          found++;
        }

        assert( (fromBase + found) == from );
        return toBase + bits;
      }
    };
  }
}

#endif // INDRI_TERMBITMAP_HPP
