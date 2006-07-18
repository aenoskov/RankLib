

//
// SnippetBuilder
//
// This code is based largely on the code I wrote for the PHP and Java
// interfaces.
//
// 17 July 2006 -- tds
//            

#include "SnippetBuilder.hpp"
#include <algorithm>

//
// _getRawNodes
//

void indri::api::SnippetBuilder::_getRawNodes( std::vector<std::string>& nodeNames, const indri::api::QueryAnnotationNode* node ) {
  if( node->type == "RawScorerNode" ) {
    nodeNames.push_back( node->name );
  } else {
    for( int i=0; i<node->children.size(); i++ ) {
      _getRawNodes( nodeNames, node->children[i] );
    }
  }
}

//
// _documentMatches
//

std::vector<indri::index::Extent> indri::api::SnippetBuilder::_documentMatches( int document, 
                                                            const std::map< std::string, std::vector<indri::api::ScoredExtentResult> >& annotations,
                                                            const std::vector<std::string>& nodeNames ) {
  std::vector<indri::index::Extent> extents;
  
  
  
  for( int i=0; i<nodeNames.size(); i++ ) {
    std::map< std::string, std::vector<indri::api::ScoredExtentResult> >::const_iterator iter;
    const std::string& nodeName = nodeNames[i];
    
    // are there annotations for this node?  if not, keep going
    iter = annotations.find( nodeName );
    if( iter == annotations.end() ) {
      continue;
    }
    
    // there are annotations, so get just the ones for this document
    const std::vector<indri::api::ScoredExtentResult>& matches = iter->second;
    for( int j=0; j<matches.size(); j++ ) {
      if( matches[j].document == document )
        extents.push_back( indri::index::Extent( matches[j].begin, matches[j].end ) );
    }
  }
  
  // now we have some extents; we'll sort them by start position
  std::sort( extents.begin(), extents.end(), indri::index::Extent::begins_before_less() );
  std::vector<indri::index::Extent> coalesced;
  
  // remove and coalesce duplicates
  if( extents.size() ) {
    int begin = extents[0].begin;
    int end = extents[0].end;
    
    for( int i=1; i<extents.size(); i++ ) {
      if( extents[i].begin > end ) {
        coalesced.push_back( indri::index::Extent( begin, end ) );
        begin = extents[i].begin;
        end = extents[i].end;
      } else if( end < extents[i].end ) {
        end = extents[i].end;
      }
    } 
    
    // add that final match
    coalesced.push_back( indri::index::Extent( begin, end ) );
  }
  
  return coalesced;
}

//
// _buildRegions
//

std::vector<indri::api::SnippetBuilder::Region> indri::api::SnippetBuilder::_buildRegions( const std::vector<indri::index::Extent>& extents,
                                                                   int positionCount, int matchWidth, int windowWidth ) {
  std::vector<indri::api::SnippetBuilder::Region> regions;
  int wordCount = 0;
  
  if( extents.size() == 0 ) 
    return regions;
  
  Region region;
  
  region.extents.push_back( extents[0] );
  region.begin = extents[0].begin - matchWidth / 2;
  region.end = extents[0].end + matchWidth / 2;
  
  region.begin = std::max( region.begin, 0 );
  region.end = std::min( positionCount, region.end );
  
  for( int i=1; i<extents.size() && wordCount < windowWidth; i++ ) {
    if( extents[i].begin - region.end <= matchWidth/2 ) {
      int oldEnd = region.end;
      region.end = std::max( extents[i].end + matchWidth / 2, region.end );
      region.extents.push_back( extents[i] );
      wordCount += region.end - oldEnd;
    } else {
      regions.push_back( region );
      region.extents.clear();
      region.extents.push_back( extents[i] );
      
      region.begin = extents[i].begin - matchWidth / 2;
      region.end = extents[i].end + matchWidth / 2;
      
      region.begin = std::max( region.begin, 0 );
      region.end = std::min( positionCount, region.end );
      wordCount += region.end - region.begin;
    }
  }
  
  regions.push_back( region );
  return regions;
}

//
// _sanitizeText
//

char* indri::api::SnippetBuilder::_sanitizeText( const char* text, int begin, int length ) {
  char* result = (char*) malloc( length+1 );
  memset( result, 0, length+1 );
  
  bool wasSpace = false;
  int end = begin+length;
  int j=0;
  
  for( int i=begin; i<end; i++ ) {
    char c = text[i];
    
    if( c == '<' ) {
      i++;

      // skip past any whitespace
      i += strspn( text + i, " \t\n\r" );
      
      if( !strncmp( "!--", text + i, 3 ) ) {
        // in comment, search for end of it:
        i += 3;
        char* endp = strstr( "-->", text + i );
        if( endp )
          i = endp - text + 2;
        else
          i = end;
      } else if( i < length-5 && !strncasecmp( "style", text + i, 5 ) ) {
        // style tag
        char* endp = strcasestr( "</style", text + i );
        if( endp )
          endp = strchr( endp, '>' );
        if( endp )
          i = endp - text;
        else 
          i = end;
      } else if( i < length-6 && !strncasecmp( "script", text + i, 6 ) ) {
        // script tag
        char* endp = strcasestr( "</script", text + i );
        if( endp )
          endp = strchr( endp, '>' );
        if( endp )
          i = endp - text;
        else
          i = end;
      } else {
        // regular old everyday tag, skip it
        char* endp = strchr( text + i, '>' );
        if( endp )
          i = endp - text;
        else
          i = end;
      }
    } else if( isspace(c) ) {
      // allow one space between words
      if( !wasSpace ) {
        wasSpace = true;
        result[j++] = ' ';
      }
    } else {
      result[j++] = c;
      wasSpace = false;
    }
  }
  
  result[j] = 0;
  return result;
}

//
// _addHighlightedText
//

void indri::api::SnippetBuilder::_addHighlightedRegion( std::string& snippet, char* region ) {
  if( _HTMLOutput ) {
    snippet += "<strong>";
    snippet += region;
    snippet += "</strong>";
  } else {
    for( char* c = region; *c; c++ ) {
      *c = toupper(*c);
    }
    snippet += region;
  }
  free(region);
}

//
// _addUnhighlightedText
//

void indri::api::SnippetBuilder::_addUnhighlightedRegion( std::string& snippet, char* region ) {
  if( _HTMLOutput ) 
    snippet += region;
  else
    snippet += region;
  
  free(region);
}

//
// _addEllipsis
//

void indri::api::SnippetBuilder::_addEllipsis( std::string& snippet ) {
  if( _HTMLOutput )
    snippet += "<strong>...</strong>";
  else
    snippet += "...";
}

//
// _completeSnippet
//

void indri::api::SnippetBuilder::_completeSnippet( std::string& snippet ) {
  if( _HTMLOutput )
    return;
  
  int i = 0;

  // add linebreaks
  while( i < snippet.size() ) {
    i += 50;
    while( i < snippet.size() && !isspace(snippet[i]) )
      i++;
    if( i < snippet.size() )
      snippet[i] = '\n';
  }
}

//
// build
//

std::string indri::api::SnippetBuilder::build( int documentID, const indri::api::ParsedDocument* document, indri::api::QueryAnnotation* annotation ) {
  // put together the match information we'll need later 
  int windowSize = 50;
  const char* text = document->text;
  std::vector<std::string> nodeNames;
  _getRawNodes( nodeNames, annotation->getQueryTree() );
  std::vector<indri::index::Extent> extents = _documentMatches( documentID, annotation->getAnnotations(), nodeNames );
  
  if( extents.size() == 0 )
    return std::string();
  
  // calculate the context width for a single match
  int matchWidth = windowSize / extents.size();
  matchWidth = std::max( 15, std::min<int>( 30, extents.size() ) );
  
  // first, we compute a list of regions
  std::vector<indri::api::SnippetBuilder::Region> regions;
  regions = _buildRegions( extents, document->positions.size(), matchWidth, windowSize );
    
  // now, we have to put all of these regions together into a snippet
  std::string snippet;
  int wordCount = 0;
  
  for( int i=0; i<regions.size() && windowSize > wordCount; i++ ) {
    Region& region = regions[i];
    
    if( region.begin != 0 && i == 0 ) {
      _addEllipsis( snippet );
    }
    
    int beginByte = document->positions[region.begin].begin;
    int endByte = document->positions[region.end-1].end;
    int current = beginByte;
    wordCount += region.end - region.begin;
    
    for( int j=0; j<region.extents.size(); j++ ) {
      int regionBegin = region.extents[j].begin;
      int regionEnd = region.extents[j].end;
      
      int beginMatch = document->positions[regionBegin].begin;
      int endMatch = document->positions[regionEnd-1].end;
      
      _addUnhighlightedRegion( snippet, _sanitizeText( text, current, beginMatch - current ) );
      _addHighlightedRegion( snippet, _sanitizeText( text, beginMatch, endMatch - beginMatch ) );
      
      current = endMatch;
    }
    
    _addUnhighlightedRegion( snippet, _sanitizeText( text, current, endByte - current ) );
    
    if( region.end != document->positions.size()-1 )
      _addEllipsis( snippet );
  }

  _completeSnippet( snippet );
  return snippet;
}                         

