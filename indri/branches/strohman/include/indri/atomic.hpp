
//
// atomic
//
// 15 December 2004 -- tds
//

#ifndef INDRI_ATOMIC_HPP
#define INDRI_ATOMIC_HPP

#ifndef WIN32
#include <bits/atomicity.h>
#endif

namespace indri {
  namespace atomic {
  #ifdef WIN32
    typedef volatile LONG value_type;

    inline void increment( value_type& variable ) {
      ::InterlockedIncrement( &variable );
    }

    inline void decrement( value_type& variable ) {
      ::InterlockedDecrement( &variable );
    }
  #else
    typedef _Atomic_word value_type;

    inline void increment( value_type& variable ) {
      __gnu_cxx::__atomic_add( &variable, 1 );
    }

    inline void decrement( value_type& variable ) {
      __gnu_cxx::__atomic_add( &variable, -1 );
    }
  #endif
  };
}

#endif // INDRI_ATOMIC_HPP

