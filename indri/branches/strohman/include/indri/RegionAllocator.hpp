
//
// RegionAllocator
//
// 9 March 2005 -- tds
//

#ifndef INDRI_REGIONALLOCATOR_HPP
#define INDRI_REGIONALLOCATOR_HPP

#include "indri/delete_range.hpp"
#include "indri/Buffer.hpp"
#include <memory>

class RegionAllocator {
private:
  std::vector<Buffer*> _buffers;
  std::vector<void*> _malloced;

public:
  ~RegionAllocator() {
    delete_vector_contents( _buffers );

    for( size_t i=0; i<_malloced.size(); i++ )
      free( _malloced[i] );
  }

  void* allocate( size_t bytes ) {
    //return malloc( bytes ); // DEBUG!!!
    assert( bytes <= 1024*1024 );
    
    if( bytes > 1024*32 ) {
      _malloced.push_back( malloc( bytes ) );
      return _malloced.back();
    }
    
    bytes = (bytes+7) & ~7; // round up
    
    if( _buffers.size() && _buffers.back()->remaining() >= bytes ) {
      return _buffers.back()->write( bytes );
    }

    _buffers.push_back( new Buffer( 1024*1024 ) );
    return allocate( bytes );
  }
};

template<class T>
class StandardRegionAllocator {
private:
  RegionAllocator& _alloc;

public:
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;

  StandardRegionAllocator( RegionAllocator alloc ) :
    _alloc( alloc )
  {
  }

  pointer address( reference val ) {
    return &val;
  }

  const_pointer address( const_reference val ) {
    return &val;
  }

  template<class Other>
  pointer allocate( size_type count, const Other* hint ) {
    return _alloc.allocate( count * sizeof(T) );
  }

  void destroy( pointer p ) {
    // does nothing
  }

  void deallocate( pointer p, size_type count ) {
    // does nothing
  }

  size_type max_size() {
    return ~( (size_type) 0 );
  }

  void construct( pointer p, reference val ) {
    new(p) T(val);
  }

  template<class _Other>
  struct rebind {
    typedef std::allocator<_Other> other;
  };
};

#endif // INDRI_REGIONALLOCATOR_HPP

