
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
#include "indri/DiskTermData.hpp"
#include <vector>
#include <string>

namespace indri {
  namespace index {
    class DiskIndex : public Index {
    private:
      Mutex _lock;

      std::string _path;

      Keyfile _frequentStringToTerm;
      Keyfile _infrequentStringToTerm;

      Keyfile _frequentIdToTerm;
      Keyfile _infrequentIdToTerm;
      File _frequentTermsData;

      File _documentLengths;
      File _documentStatistics;

      File _invertedFile;
      File _directFile;

      std::vector<FieldStatistics> _fieldData;
      std::vector<File> _fieldFiles;
      int _documentBase;
      int _infrequentTermBase;

      indri::index::DiskTermData* _fetchTermData( int termID );
      indri::index::DiskTermData* _fetchTermData( const char* termString );

      CorpusStatistics _corpusStatistics;
      void _readManifest( const std::string& manifestPath );

    public:
      void open( const std::string& base, const std::string& relative );
      void close();

      const std::string& path();
      int documentBase();

      int field( const char* fieldName );
      int field( const std::string& fieldName );
      std::string field( int fieldID );

      int term( const char* term );
      int term( const std::string& term );
      std::string term( int termID );

      int documentLength( int documentID );
      UINT64 documentCount();
      UINT64 documentCount( const std::string& term );
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
      
      DocListIterator* docListIterator( int termID );
      DocListIterator* docListIterator( const std::string& term );
      DocListFileIterator* docListFileIterator();
      DocExtentListIterator* fieldListIterator( int fieldID );
      DocExtentListIterator* fieldListIterator( const std::string& field );
      const TermList* termList( int documentID );
      TermListFileIterator* termListFileIterator();

      VocabularyIterator* vocabularyIterator();
      VocabularyIterator* frequentVocabularyIterator();
      VocabularyIterator* infrequentVocabularyIterator();

      DocumentDataIterator* documentDataIterator();
    };
  }
}

#endif // INDRI_DISKINDEX_HPP
