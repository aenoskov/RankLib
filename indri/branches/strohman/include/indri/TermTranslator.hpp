
//
// TermTranslator
//
// 14 January 2005 -- tds
//

#ifndef INDRI_TERMTRANSLATOR_HPP
#define INDRI_TERMTRANSLATOR_HPP

#include "indri/HashTable.hpp"
#include <vector>
#include "indri/TermBitmap.hpp"

namespace indri {
  namespace index {
    class TermTranslator {
    private:
      TermBitmap* _bitmap;
      int _previousFrequentCount;
      int _currentFrequentCount;

      std::vector<int>* _frequentMap;
      HashTable<int, int>* _wasInfrequentMap;

    public:
      ~TermTranslator() {
        delete _frequentMap;
      }

      TermTranslator( int previousFrequentCount,
                      int currentFrequentCount,
                      std::vector<int>* frequentMap,
                      HashTable<int, int>* wasInfrequentMap,
                      TermBitmap* bitmap ) 
        :
        _bitmap(bitmap),
        _frequentMap(frequentMap),
        _wasInfrequentMap(wasInfrequentMap)
      {
        _previousFrequentCount = previousFrequentCount;
        _currentFrequentCount = currentFrequentCount;
      }

      int operator() ( int termID ) {
        int result = 0;

        if( termID < _previousFrequentCount ) {
          // common case, termID is a frequent term
          result = (*_frequentMap)[termID];
        } else {
          // term may have become frequent, so check the wasInfrequentMap
          int* value = (*_wasInfrequentMap).find( termID );

          if( value ) {
            result = *value;
          } else {
            // term wasn't frequent and isn't now either, so get it from the bitmap
            result = _currentFrequentCount + _bitmap->get( termID - _previousFrequentCount + 1 );
          }
        }

        return result;
      }
    };
  }
}

#endif // INDRI_TERMTRANSLATOR_HPP



