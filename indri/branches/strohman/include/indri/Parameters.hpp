/*==========================================================================
  Copyright (c) 2004 University of Massachusetts.  All Rights Reserved.

  Use of the Lemur Toolkit for Language Modeling and Information Retrieval
  is subject to the terms of the software license set forth in the LICENSE
  file included with this software, and also available at
  http://www.cs.cmu.edu/~lemur/license.html 
  as well as the conditions below.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  3. The names "Indri", "Center for Intelligent Information Retrieval", 
  "CIIR", and "University of Massachusetts" must not be used to
  endorse or promote products derived from this software without
  prior written permission. To obtain permission, contact
  indri-info@ciir.cs.umass.edu.

  4. Products derived from this software may not be called "Indri" nor 
  may "Indri" appear in their names without prior written permission of 
  the University of Massachusetts. To obtain permission, contact 
  indri-info@ciir.cs.umass.edu.

  THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF MASSACHUSETTS AND OTHER
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
  THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.
  ==========================================================================
*/


//
// Parameters
//
// 29 April 2004 -- tds
//

#ifndef INDRI_PARAMETERS_HPP
#define INDRI_PARAMETERS_HPP

#include <string>
#include "indri/XMLNode.hpp"
#include "indri/delete_range.hpp"

/*! Parameters class for Indri index and retrieval methods. 
 *  Provides a map of key value pairs, where the key is a string
 *  and the values can be one of double, int, INT64, string, 
 *  or a Parameters object, encapsulated in a parameter_value struct.
 */
class Parameters {
public:
  /// Container for parameter values
  struct parameter_value {
    typedef std::map<std::string, parameter_value*> MValue;
    /// map of string to parameter_value pointers.
    MValue table;
    typedef std::vector<parameter_value*> VValue;
    /// vector of parameter_value pointers
    std::vector<parameter_value*> array;
    /// string representation of parameter value.
    std::string value;

  public:
    /// create
    parameter_value() {}
    /// Make a deep copy.
    /// @param other the parameter_value to copy.
    parameter_value( const parameter_value& other ) {
      value = other.value;

      for( size_t i=0; i<other.array.size(); i++ )
        array.push_back( new parameter_value( *other.array[i] ) );
      
      for( MValue::const_iterator iter = other.table.begin();
           iter != other.table.end();
           iter++ )
      {
        table.insert( std::make_pair( iter->first,
                                      new parameter_value( *(iter->second) ) ) );
      }
    }
    /// clean up
    ~parameter_value() {
      clear();
    }

    /// remove any current value, and any subvalues of this parameter
    void clear() {
      for( std::map<std::string, parameter_value*>::iterator iter = table.begin();
           iter != table.end();
           iter++ )
      {
        delete iter->second;
      }
      table.clear();

      delete_vector_contents<parameter_value*>(array);
      value = "";
    }

    /// convert single value to an entry in the vector array.
    void convertToArray() {
      if( !array.size() && ( table.size() || value.size() ) ) {
        parameter_value* child = new parameter_value;

        child->table = table;
        child->value = value;

        table.clear();
        value = "";

        array.push_back(child);
      }
    }
    /// Get the value.
    /// @return the value of the contents of the first element of array
    /// if present, otherwise the contents of value.
    const std::string& getValue() {
      if( !array.size() )
        return value;
      else
        return array[0]->value;
    }

  };

protected:
  parameter_value* _collection;
  bool _owned;

  parameter_value* _getRoot();
  parameter_value* _getPath( const std::string& path, Parameters::parameter_value* last, int offset = 0 );
  parameter_value* _createPath( const std::string& path );
  void _parseNextSegment( std::string& segment, int& arrayIndex, int& endOffset, const std::string& path, int beginOffset );
  parameter_value* _getSegment( const std::string& segment, int arrayIndex, Parameters::parameter_value* from );
  
  void _loadXML( class XMLNode* node );
  void _fillXML( class XMLNode* node );
  
  INT64 _multiplier( const std::string& value ) {
    if( !value.length() )
      return 1;

    char suffix = value[ value.length()-1 ];
    
    switch( suffix ) {
      case 'K':
      case 'k':
        return 1000;

      case 'M':
      case 'm':
        return 1000000;

      case 'G':
      case 'g':
        return 1000000000;
    }

    return 1;
  }

  bool _isBoolean( const std::string& value ) {
    if( !value.length() )
      return false;

    char first = value[0];
    
    switch(first) {
      case 'Y':
      case 'y':
      case 'N':
      case 'n':
      case 'T':
      case 't':
      case 'F':
      case 'f':
        return true;
    }

    return false;
  }

  bool _asBoolean( const std::string& value ) {
    char first = value[0];
    
    switch(first) {
      case 'Y':
      case 'y':
      case 'T':
      case 't':
      case '1':
         return true;
   
      case 'F':
      case 'f':
      case 'N':
      case 'n':
      case '0':
        return false;
    }

    return false;
  }

public:
  /// Create
  Parameters();
  /// Clone
  /// @param other the object to copy.
  Parameters( const Parameters& other );
  /// Initialize with a parameter_value
  /// @param value the item to insert.
  Parameters( parameter_value* value );
  /// Initialize with a key path and parameter_value
  /// @param path the key.
  /// @param value the item to insert for that key.
  Parameters( const std::string& path, parameter_value* value );
  /// Clean up.
  ~Parameters();
  /// @return the value of the parameter as a double
  operator double () {
    const std::string& value = _getRoot()->getValue();
    return atof( value.c_str() );
  }

  operator bool () {
    const std::string& value = _getRoot()->getValue();
    return _asBoolean(value);
  }

  /// Converts the value to an int, scaling by the multiplier if supplied.
  /// Valid values are (case insensitive) K = 1000, M = 1000000, 
  /// G = 1000000000.
  /// @return the value of the parameter as an int
  operator int () {
    const std::string& value = _getRoot()->getValue();

    if( _isBoolean(value) )
      return _asBoolean(value);

    int multiplier = (int) _multiplier( value );

    if( multiplier > 1 ) {
      std::string prefix = value.substr( 0, value.length() );
      return multiplier * atoi( prefix.c_str() );
    }

    return atoi( value.c_str() );
  }

  /// Converts the value to an INT64, scaling by the multiplier if supplied.
  /// Valid values are (case insensitive) K = 1000, M = 1000000, 
  /// G = 1000000000.
  /// @return the value of the parameter as an INT64
  operator INT64 () {
    const std::string& value = _getRoot()->getValue();
    INT64 multiplier = _multiplier( value );

    if( _isBoolean(value) )
      return _asBoolean(value);

    if( multiplier > 1 ) {
      std::string prefix = value.substr( 0, value.length() );
      return multiplier * string_to_i64( prefix.c_str() );
    }

    return string_to_i64( value );
  }

  /// @return the value of the parameter as a string
  operator std::string () {
    std::string value = _getRoot()->getValue();
    return value;
  }

  /// assignment via deep copy.
  /// @param other the item to copy.
  /// @return this object
  const Parameters& operator= ( const Parameters& other ) {
    _collection->value = other._collection->value;
    
    delete_vector_contents( _collection->array );
    delete_map_contents( _collection->table );

    for( size_t i=0; i<other._collection->array.size(); i++ ) {
      _collection->array.push_back( new parameter_value( *other._collection->array[i] ) );
    }
    
    parameter_value::MValue::iterator iter;

    for( iter = other._collection->table.begin();
         iter != other._collection->table.end();
         iter++ ) {
      _collection->table.insert( std::make_pair( iter->first, 
                                 new parameter_value( *iter->second ) ) );
    }
   
    return *this;
  }

  /// Retrieve the n'th entry.
  /// @param index, the index of the entry to retrieve.
  /// @return a Parameters object.
  Parameters get( int index );
  /// Retrieve the entry associated with name.
  /// @param name the key value.
  /// @return a Parameters object.
  Parameters get( const std::string& name );
  /// Retrieve the entry associated with name.
  /// @param name the key value.
  /// @return a Parameters object.
  Parameters get( const char* name );

  bool get( const std::string& name, bool def );
  /// Retrieve the entry associated with name.
  /// @param name the key value.
  /// @param def the default value for the key
  /// @return the value associated with the key or def if no entry
  /// exists.
  int get( const std::string& name, int def );
  /// Retrieve the entry associated with name.
  /// @param name the key value.
  /// @param def the default value for the key
  /// @return the value associated with the key or def if no entry
  /// exists.
  double get( const std::string& name, double def );
  /// Retrieve the entry associated with name.
  /// @param name the key value.
  /// @param def the default value for the key
  /// @return the value associated with the key or def if no entry
  /// exists.
  INT64 get( const std::string& name, INT64 def );
  std::string get( const std::string& name, const char* def );
  /// Retrieve the entry associated with name.
  /// @param name the key value.
  /// @param def the default value for the key
  /// @return the value associated with the key or def if no entry
  /// exists.
  std::string get( const std::string& name, const std::string& def );

  /// Retrieve the n'th entry.
  /// @param index the index of the entry to retrieve.
  /// @return a Parameters object.
  Parameters operator[] ( int index );
  /// Retrieve the entry indexed by path.
  /// @param path the key of the entry to retrieve.
  /// @return a Parameters object.
  Parameters operator[] ( const std::string& path );
  /// Retrieve the entry indexed by path.
  /// @param path the key of the entry to retrieve.
  /// @return a Parameters object.
  Parameters operator[] ( const char* path );
  /// Create a new empty parameter_value for the key given in path
  /// @param path the key to create the value for
  /// @return the Parameters object initialized with the new value.
  Parameters append( const std::string& path );
  /// Remove an entry from the table. Does nothing if the key does not
  /// exist.
  /// @param path the key to remove.
  void remove( const std::string& path );

  void set( const std::string& name, bool value );
  void set( const std::string& name, const char* value );
  /// Set the value  for the given key.
  /// @param name the key
  /// @param value the value
  void set( const std::string& name, const std::string& value );
  /// Set the value  for the given key.
  /// @param name the key
  /// @param value the value
  void set( const std::string& name, int value );
  /// Set the value  for the given key.
  /// @param name the key
  /// @param value the value
  void set( const std::string& name, UINT64 value );
  /// Set the value  for the given key.
  /// @param name the key
  /// @param value the value
  void set( const std::string& name, double value );
  /// Set the value of the Parameters object
  /// @param value the value
  void set( const std::string& value );

  /// Clear the parameter tree
  void clear();

  /// @return the size of the object.
  size_t size();
  /// @param index the index to probe.
  /// @return true if an entry exists for this index, false otherwise.
  bool exists( int index );
  /// @param name the key to probe.
  /// @return true if an entry exists for this key, false otherwise.
  bool exists( const std::string& name );

  /// Convert to XML
  /// @return an XMLNode containing the parameters
  XMLNode* toXML();

  /// Return the singleton instance of the Parameters class.
  static Parameters& instance();

  /// Initialize from a string
  /// @param text the text to parse
  void load( const std::string& text );
  /// Initialize from a file
  /// @param filename the filename to parse
  void loadFile( const std::string& filename );
  /// Initialize from the command line
  /// @param argc the number of command line arguments
  /// @param argv the command line arguments
  void loadCommandLine( int argc, char** argv );
  /// Write the Parameters table to the given string
  /// @param text the string to write into.
  void write( std::string& text );
  /// Write the Parameters table to the given filename
  /// @param filename the filename to write into.
  void writeFile( const std::string& filename );
};

#endif // INDRI_PARAMETERS_HPP
