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
// FileTreeIterator
//
// 26 May 2004 -- tds
//

#ifndef INDRI_FILETREEITERATOR_HPP
#define INDRI_FILETREEITERATOR_HPP

#include "indri/DirectoryIterator.hpp"
#include <stack>
namespace indri
{
  namespace file
  {
    
    /*! Provides iteration over a directory tree.
     */
    class FileTreeIterator {
    private:
      static FileTreeIterator _end;
      std::stack<DirectoryIterator*> _stack;
      std::string _current;
  
      void _nextCandidate();
      void _next();

    public:
      FileTreeIterator();
      FileTreeIterator( const std::string& path );
      ~FileTreeIterator();

      void operator ++ (int);
      void operator ++ ();
      bool operator == ( const FileTreeIterator& other ) const;
      bool operator != ( const FileTreeIterator& other ) const;
      const std::string& operator* ();

      static const FileTreeIterator& end();
    };
  }
}

#endif // INDRI_FILETREEITERATOR_HPP
