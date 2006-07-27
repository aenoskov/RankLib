
//
// MD5Writer
//
// 27 July 2006 -- tds
//
// Builds MD5 hashes of input data for use in duplicate
// detection.
//
// Contains a MD5 implementation by L. Peter Deutsch.
// Please see MD5Writer.cpp for the associated 
// copyright notice.
//

#ifndef INDRI_MD5WRITER_HPP
#define INDRI_MD5WRITER_HPP

#include "indri/ObjectHandler.hpp"
#include "indri/UnparsedDocument.hpp"
#include <fstream>
#include <string>

namespace indri {
  namespace parse {
    class MD5Writer : public ObjectHandler<UnparsedDocument> {
    private:
      std::ofstream _out;

    public:
      MD5Writer( const std::string& path );
      ~MD5Writer();

      void handle( indri::parse::UnparsedDocument* document );
    };
  }
}

#endif // INDRI_MD5WRITER_HPP

