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
// Parameters
//
// 29 April 2004 -- tds
//

#ifndef INDRI_PARAMETERS_HPP
#define INDRI_PARAMETERS_HPP

#include <string>
#include "indri/XMLNode.hpp"
#include "indri/delete_range.hpp"

class Parameters {
public:
  struct parameter_value {
    typedef std::map<std::string, parameter_value*> MValue;
    MValue table;
    typedef std::vector<parameter_value*> VValue;
    std::vector<parameter_value*> array;
    std::string value;

  public:
    parameter_value() {}
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
    
    ~parameter_value() {
      for( std::map<std::string, parameter_value*>::iterator iter = table.begin();
           iter != table.end();
           iter++ )
      {
        delete iter->second;
      }

      delete_vector_contents<parameter_value*>(array);
    }
    
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
  Parameters();
  Parameters( const Parameters& other );
  Parameters( parameter_value* value );
  Parameters( const std::string& path, parameter_value* value );

  ~Parameters();

  operator double () {
    const std::string& value = _getRoot()->getValue();
    return atof( value.c_str() );
  }

  operator bool () {
    const std::string& value = _getRoot()->getValue();
    return _asBoolean(value);
  }

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

  operator std::string () {
    std::string value = _getRoot()->getValue();
    return value;
  }

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

  Parameters get( int index );
  Parameters get( const std::string& name );
  Parameters get( const char* name );

  bool get( const std::string& name, bool def );
  int get( const std::string& name, int def );
  double get( const std::string& name, double def );
  INT64 get( const std::string& name, INT64 def );
  std::string get( const std::string& name, const char* def );
  std::string get( const std::string& name, const std::string& def );

  Parameters operator[] ( int index );
  Parameters operator[] ( const std::string& path );
  Parameters operator[] ( const char* path );

  Parameters append( const std::string& path );
  void remove( const std::string& path );
  void set( const std::string& name, bool value );
  void set( const std::string& name, const char* value );
  void set( const std::string& name, const std::string& value );
  void set( const std::string& name, int value );
  void set( const std::string& name, UINT64 value );
  void set( const std::string& name, double value );
  void set( const std::string& value );

  size_t size();
  bool exists( int index );
  bool exists( const std::string& name );

  XMLNode* toXML();

  static Parameters& instance();

  void load( const std::string& text );
  void loadFile( const std::string& filename );
  void loadCommandLine( int argc, char** argv );

  void write( std::string& text );
  void writeFile( const std::string& filename );
};

#endif // INDRI_PARAMETERS_HPP
