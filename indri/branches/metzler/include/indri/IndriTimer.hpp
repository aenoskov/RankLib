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
// IndriTimer.hpp
//
// 13 August 2004 -- tds
//

#ifndef INDRI_INDRITIMER_HPP

#include <iostream>
#include "lemur/lemur-platform.h"

class IndriTimer {
private:
  UINT64 _start;

public:
  IndriTimer();

  static UINT64 currentTime();

  void start();
  UINT64 elapsedTime() const;

  void printElapsedSeconds( std::ostream& out ) const;
  void printElapsedMicroseconds( std::ostream& out ) const;
};

#endif // INDRI_INDRITIMER_HPP

