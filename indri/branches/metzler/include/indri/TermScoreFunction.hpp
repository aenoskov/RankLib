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
// TermScoreFunction
//
// 23 January 2004 -- tds
//

#ifndef INDRI_TERMSCOREFUNCTION_HPP
#define INDRI_TERMSCOREFUNCTION_HPP

class TermScoreFunction {
public:
  virtual double scoreOccurrence( int occurrences, int contextLength ) = 0;
  virtual double scoreOccurrence( int occurrences, int contextLength, int documentOccurrences, int documentLength ) = 0;
};

#endif // INDRI_TERMSCOREFUNCTION_HPP

