
//
// DiskIndex
//
// 8 December 2004 -- tds
//

#ifndef INDRI_DISKINDEX_HPP
#define INDRI_DISKINDEX_HPP

#include "indri/Index.hpp"
#include "indri/File.hpp"
#include "lemur/Keyfile.hpp"
#include "indri/TermData.hpp"
#include "indri/FieldStatistics.hpp"
#include "indri/CorpusStatistics.hpp"
#include <vector>
#include <string>

namespace indri {
  namespace index {
    class DiskIndex : public Index {
    private:
      Mutex _lock;

      Keyfile _frequentStringToTerm;
      Keyfile _infrequentStringToTerm;

      Keyfile _frequentIdToTerm;
      Keyfile _infrequentIdToTerm;

      File _documentLengths;
      File _documentStatistics;

      File _invertedFile;
      File _directFile;

      std::vector<FieldStatistics> _fieldData;
      std::vector<File> _fieldFiles;
      int _documentBase;

      indri::index::TermData* _fetchTermData( int termID );
      indri::index::TermData* _fetchTermData( const char* termString );

      CorpusStatistics _corpusStatistics;

    public:
      int documentBase();

      int field( const char* fieldName );
      int field( const std::string& fieldName );

      int term( const char* term );
      int term( const std::string& term );
      std::string term( int termID );

      int documentLength( int documentID );
      UINT64 documentCount();
      UINT64 uniqueTermCount();

      UINT64 termCount( const std::string& term );
      UINT64 termCount();

      UINT64 fieldTermCount( const std::string& field );
      UINT64 fieldTermCount( const std::string& field, const std::string& term );

      UINT64 fieldDocumentCount( const std::string& field );
      UINT64 fieldDocumentCount( const std::string& field, const std::string& term );

      //
      // Lists
      //
      
      virtual DocListIterator* docListIterator( int termID ) = 0;
      virtual DocListIterator* docListIterator( const std::string& term ) = 0;
      virtual DocListFileIterator* docListFileIterator() = 0;
      virtual DocExtentListIterator* fieldListIterator( int fieldID ) = 0;
      virtual DocExtentListIterator* fieldListIterator( const std::string& field ) = 0;
      virtual const TermList* termList( int documentID ) = 0;
      virtual TermListFileIterator* termListFileIterator() = 0;
      virtual VocabularyIterator* vocabularyIterator() = 0;
    };
  }
}

#endif // INDRI_DISKINDEX_HPP
