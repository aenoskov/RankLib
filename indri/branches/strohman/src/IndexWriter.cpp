
//
// IndexWriter
//
// 18 November 2004 -- tds
//
// Writes out a single index from one or more indexes.
//

#include "indri/IndexWriter.hpp"
#include "indri/Index.hpp"
#include "indri/DocListFileIterator.hpp"
#include <vector>
#include <queue>
#include "greedy_vector"

using namespace indri::index;
typedef std::priority_queue<DocListFileIterator*,
                            std::vector<DocListFileIterator*>,
                            DocListFileIterator::iterator_less> iterator_pqueue;

//
// indexwriter_fetchMatchingInvertedLists
//

void indexwriter_fetchMatchingInvertedLists( greedy_vector<DocListFileIterator*>& iterators, iterator_pqueue& queue ) {
  DocListFileIterator* first = docLists.top();
  iterators.push_back( first );
  const char* firstTerm = first->currentEntry()->termData->term;
  docLists.pop();

  while( !strcmp( firstTerm, docLists.top()->currentEntry()->termData->term ) ) {
    iterators.push_back( docLists.top() );
    docLists.pop();
  }
}

//
// indexwriter_pushInvertedLists
//

void indexwriter_pushInvertedLists( greedy_vector<DocListFileIterator*>& iterators, iterator_pqueue& queue ) {
  for( int i=0; i<iterators.size(); i++ ) {
    if( iterators[i]->finished() ) {
      delete iterators[i];
    } else {
      queue.push( iterators[i] );
    }
  }
}

//
// writeInvertedLists
//

void writeInvertedLists( std::vector<index::index::Index*>& indexes ) {
  
  // write a combined inverted list in vocabulary order
  // in the process, create a new list of termIDs from the old list
  
  std::priority_queue<DocListFileIterator*,
                      std::vector<DocListFileIterator*>,
                      DocListFileIterator::iterator_less> docLists;
  
  // clear out the term buffer
  char term[Keyfile::MAX_TERM_LENGTH+1];
  term[0] = 0;
  int termID = 1;

  for( int i=0; i<indexes.size(); i++ )
    docLists.push( indexes[i]->docListFileIterator() );

  greedy_vector<DocListFileIterator*> iterators;

  while( docLists.size() ) {
    // fetch useful doc lists
    indexwriter_fetchMatchingInvertedLists( iterators, docLists );

    // loop 1: merge statistics for term
    indexwriter_writeStatistics( iterators );

    // go through lists one by one, adding data to the final invlist, adding skips, etc.
    indexwriter_addInvertedListData( iterators );

    // for each list, we also need to store that this term was found
    indexwrite_storeMatchInformation( iterators );

    // push back all doc lists with useful information
    indexwriter_pushInvertedLists( iterators, docLists );
  }


  writer.flush();
  file.close();
}

void writeTopVocabulary(  ) {
  // not quite clear how to do this -- should this be done inline or as a separate task?
  
  
}

