
//
// DiskTermData
//
// 13 December 2004 -- tds
//

#ifndef INDRI_DISKTERMDATA_HPP
#define INDRI_DISKTERMDATA_HPP

namespace indri {
  namespace index {
    struct DiskTermData {
      enum {
        WithOffsets = 0x01,
        WithString = 0x02,
        WithTermID = 0x04
      };

      TermData* termData;
      int termID;

      INT64 startOffset;
      INT64 length;
    };
  }
}


//
// disktermdata_decompress
//

inline void disktermdata_compress( RVLCompressStream& stream, indri::index::DiskTermData* diskData, int fieldCount, int mode ) {
  ::termdata_compress( stream, diskData->termData, fieldCount );

  if( mode & indri::index::DiskTermData::WithTermID ) {
    stream << diskData->termID;
  }

  if( mode & indri::index::DiskTermData::WithString ) {
    stream << diskData->termData->term;
  }

  if( mode & indri::index::DiskTermData::WithOffsets ) {
    stream << diskData->startOffset
           << diskData->length;
  }
}

//
// disktermdata_decompress
//

inline indri::index::DiskTermData* disktermdata_decompress( RVLDecompressStream& stream, void* buffer, int fieldCount, int mode ) {
  indri::index::DiskTermData* diskData = (indri::index::DiskTermData*) buffer;

  int termDataSize = ::termdata_size( fieldCount );
  char* termLocation = (char*)buffer + sizeof(indri::index::DiskTermData) + termDataSize;
  indri::index::TermData* termDataLocation = (indri::index::TermData*) ((char*)buffer + sizeof(indri::index::DiskTermData));

  diskData->termData = termDataLocation;
  diskData->termData->term = termLocation;

  // set first byte of string to zero
  termLocation[0] = 0;

  ::termdata_decompress( stream, diskData->termData, fieldCount );

  if( mode & indri::index::DiskTermData::WithTermID ) {
    stream >> diskData->termID;
  } else {
    diskData->termID = 0;
  }

  if( mode & indri::index::DiskTermData::WithString ) {
    stream >> termLocation;
  }

  if( mode & indri::index::DiskTermData::WithOffsets ) {
    stream >> diskData->startOffset
           >> diskData->length;
  } else {
    diskData->startOffset = 0;
    diskData->length = 0;
  }

  return diskData;
}

//
// disktermdata_decompress
//

inline indri::index::DiskTermData* disktermdata_decompress( RVLDecompressStream& stream, int fieldCount, int mode ) {
  // how much space are we going to need?
  int termDataSize = ::termdata_size( fieldCount );
  int totalSize = termDataSize + (Keyfile::MAX_KEY_LENGTH+2) + sizeof(indri::index::DiskTermData);

  return ::disktermdata_decompress( stream, malloc( totalSize ), fieldCount, mode );
}

//
// disktermdata_delete
//

inline void disktermdata_delete( indri::index::DiskTermData* diskData ) {
  free( diskData );
}

#endif // INDRI_DISKTERMDATA_HPP