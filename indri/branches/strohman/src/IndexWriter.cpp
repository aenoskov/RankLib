
//
// IndexWriter
//
// 18 November 2004 -- tds
//
// Writes out a single index from one or more indexes.
//

void writeListData( InvertedListWriter& writer, InvertedListIterator* iterator ) {
}

void writeInvertedLists( std::vector<Index*>& indexes, std::vector< std::vector<int> >& termIDMapping ) {
  
  // write a combined inverted list in vocabulary order
  // in the process, create a new list of termIDs from the old list
  
  std::priority_queue<InvertedListIterator*,
                      std::vector<InvertedListIterator*>,
                      InvertedListIterator::inverted_less> invertedLists;
  
  for( int i=0; i<indexes.size(); i++ ) {
    invertedLists.push_back( indexes.invertedListIterator() );
    termIDMapping.push_back( std::vector<int>() );
  }
  
  // clear out the term buffer
  char term[Keyfile::MAX_TERM_LENGTH+1];
  term[0] = 0;
  int termID = 0;
  
  while( invertedLists.size() ) {
    InvertedListIterator* iter = invertedLists.top();
    
    if( strcmp( term, iter->term() ) != 0 ) {
      termID++;
      writer.startTerm( termID );
    }
    
    termIDMapping[ iter->number() ].push_back( termID );
    writeDataForSegmentTerm( writer, iter, termID );
    
    invertedLists.pop();
    
    if( iter->done() ) {
      delete iter;
    } else {
      invertedLists.push( iter );
    }
  }

  writer.flush();
  file.close();
}

void writeDocumentLists( std::vector< std::vector<int> >& termIDMapping ) {
  // are the mappings on disk when this happens or in memory?
  // certainly one mapping has to be in memory, but perhaps more than one.  
  // if vocab = 11 million terms, array is 44mb (big, if we're merging a lot of stuff)
  
}

void writeTopVocabulary(  ) {
  // not quite clear how to do this -- should this be done inline or as a separate task?
  
  
}

