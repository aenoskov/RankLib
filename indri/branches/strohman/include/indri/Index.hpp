
//
// Index
//
// 15 November 2004 -- tds
//

#ifndef INDRI_INDEX_HPP
#define INDRI_INDEX_HPP

#include <string>

#include "indri/DocListIterator.hpp"
#include "indri/DocListFileIterator.hpp"
#include "indri/FieldListIterator.hpp"
#include "indri/VocabularyIterator.hpp"
#include "indri/TermList.hpp"
#include "indri/TermListFileIterator.hpp"

namespace indri {
  namespace index {
    class Index {
    public:
      /// Field data
      struct FieldDescription {
        /// name of the field
        std::string name;
        /// does the field contain numeric data
        bool numeric;
      };
      
      //
      // Counts
      //
      
      virtual int term( const std::string& term );
      virtual std::string term( int termID );
            
      virtual UINT64 documentCount();
      virtual UINT64 uniqueTermCount();

      virtual UINT64 termCount( const std::string& term );
      virtual UINT64 termCount();

      virtual UINT64 fieldTermCount( const std::string& field );
      virtual UINT64 fieldTermCount( const std::string& field, const std::string& term );

      virtual UINT64 fieldDocumentCount( const std::string& field );
      virtual UINT64 fieldDocumentCount( const std::string& field, const std::string& term );

      //
      // Lists
      //
      
      indri::index::DocListIterator* docListIterator( int termID );
      indri::index::DocListIterator* docListIterator( const std::string& term );
      indri::index::DocListFileIterator* docListFileIterator();
      indri::index::FieldListIterator* fieldListIterator( int fieldID );
      indri::index::FieldListIterator* fieldListIterator( const std::string& field );
      indri::index::TermList* termList( int documentID );
      indri::index::TermListFileIterator* termListsIterator();
      indri::index::VocabularyIterator* vocabularyIterator();
    };
  }
}

#endif // INDRI_INDEX_HPP

