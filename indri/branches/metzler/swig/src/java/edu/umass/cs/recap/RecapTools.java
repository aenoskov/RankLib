/*
 * Created on Apr 5, 2005
 *
 */
package edu.umass.cs.recap;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.Map;

import edu.umass.cs.indri.ParsedDocument;
import edu.umass.cs.indri.QueryAnnotation;
import edu.umass.cs.indri.QueryAnnotationNode;
import edu.umass.cs.indri.ScoredExtentResult;

/**
 * @author Don Metzler
 * 
 * Snippet code by Trevor Strohman
 *
 */

public class RecapTools {
	
	public static class Match {
		public static class Comparator implements java.util.Comparator {
			public int compare( Object o, Object t ) {
				Match one = (Match) o;
				Match two = (Match) t;
				
				return one.begin - two.begin;
			}
		}
		
		public Match( int b, int e ) {
			begin = b;
			end = e;
		}
		
		public boolean overlaps( Match e ) {
			return ( (begin > e.end || end < e.begin) == false );
		}
		
		public void expand( Match e ) {
			if( begin > e.begin )
				begin = e.begin;
			if( end < e.end )
				end = e.end;
		}
		
		public int getBegin() {
			return begin;
		}
		
		public int getEnd() {
			return end;
		}
		
		public int begin;
		public int end;
	}
	
	public static void harvestNodes( ArrayList list, QueryAnnotationNode node ) {
		if( node.type.equals( "RawScorerNode" ) ) {
			list.add( node.name );
		} else if( node.children != null ) {
			for( int i=0; i<node.children.length; i++ ) {
				harvestNodes( list, node.children[i] );
			}
		}
	}
	
	public static ArrayList harvestNodes( QueryAnnotation annotation ) {
		QueryAnnotationNode node = annotation.getQueryTree();
		ArrayList list = new ArrayList();
		harvestNodes( list, node );
		return list;
	}
	
	public static ArrayList getMatches( QueryAnnotation annotation, int document ) {
		ArrayList nodeNames = harvestNodes( annotation );
		Iterator iter = nodeNames.iterator();
		Map annotations = annotation.getAnnotations();
		ArrayList results = new ArrayList();
		
		while( iter.hasNext() ) {
			String name = (String) iter.next();
			ScoredExtentResult[] resultList = (ScoredExtentResult[]) annotations.get( name );
			
			for( int i=0; i<resultList.length; i++ ) {
				if( resultList[i].document == document ) {
					results.add( new Match( resultList[i].begin, resultList[i].end ) );
				}
			}
		}
		
		Collections.sort( results, new Match.Comparator() );
		ArrayList trimmed = new ArrayList();
		iter = results.iterator();
		Match current = null;
		
		while( iter.hasNext() ) {
			if( current == null ) {
				current = (Match) iter.next();
			} else {
				Match next = (Match) iter.next();
				
				if( current.overlaps( next ) ) {
					current.expand( next );
				} else {
					trimmed.add( current );
					current = next;
				}
			}
		}
		
		if( current != null ) {
			trimmed.add( current );
		}
		
		return trimmed;
	}
	
	public static ArrayList buildMatchSegments( ArrayList matches, int matchWidth ) {
		Iterator iter = matches.iterator();
		ArrayList segment = null;
		Match current = null;
		ArrayList segments = new ArrayList();
		
		while( iter.hasNext() ) {
			Match next = (Match) iter.next();
			
			if( current == null ) {
				segment = new ArrayList();
				segment.add( next );
			} else {
				if( (next.getBegin() - matchWidth) <= current.getEnd() ) {
					segment.add( next );
				} else {
					segments.add( segment );
					segment = new ArrayList();
					segment.add( next );
				}
			}
			current = next;
		}
		
		if( segment != null && segment.size() > 0 )
			segments.add( segment );
		
		return segments;
	}
	
	public static String buildSnippet( QueryAnnotation annotation,
									   int document,
									   String documentText,
									   ParsedDocument.TermExtent[] positions,
									   int windowSize,
									   int smallContext,
									   int largeContext )	{
		ArrayList matches = getMatches( annotation, document );
		int matchWidth = (int) (windowSize / (float) matches.size());
		
		matchWidth = Math.max( smallContext, matchWidth );
		matchWidth = Math.min( largeContext, matchWidth );
		
		// come up with a list of match segments--these are the bits of the
		// document that will be shown together, separated by '...'
		ArrayList segments = buildMatchSegments( matches, matchWidth );
		Iterator iter = segments.iterator();
		StringBuffer builder = new StringBuffer();
		int lastEnd = 0;
		String ellipsis = "<strong>...</strong>";
		int words = 0;
		
		while( iter.hasNext() && words < windowSize ) {
			ArrayList segment = (ArrayList) iter.next();
			
			int begin = Integer.MAX_VALUE;
			int end = 0;
			
			Iterator inner = segment.iterator();
			while( inner.hasNext() ) {
				Match match = (Match) inner.next();
				begin = Math.min( match.getBegin() - matchWidth / 2, begin );
				end = Math.max( match.getEnd() + matchWidth / 2, end );
			}
			
			if( end < begin ) {
				begin = 0;
				end = matchWidth;
			}
			
			begin = Math.max( begin, 0 );
			end = Math.min( end, positions.length );
			
			if( begin != 0 ) {
				builder.append( ellipsis );
			}
			
			ArrayList fragments = new ArrayList();
			inner = segment.iterator();
			int lastEndChar = positions[begin].begin;
			
			while( inner.hasNext() ) {
				Match match = (Match) inner.next();
				
				String preMatchText = documentText.substring( lastEndChar, positions[ match.getBegin() ].begin );
				String matchText = documentText.substring( positions[ match.getBegin() ].begin,
						positions[ match.getEnd()-1 ].end );
				lastEndChar = positions[ match.getEnd()-1 ].end;
				
				builder.append( removeTags( preMatchText ) );
				builder.append( "<strong>" );
				builder.append( removeTags( matchText ) );
				builder.append( "</strong>" );
			}
			
			String endText = documentText.substring( lastEndChar, positions[ end-1 ].end );
			builder.append( removeTags( endText ) );
			words += (end - begin);
			
			lastEnd = end;
		}
		
		if( lastEnd != positions.length )
			builder.append( ellipsis );
		
		return builder.toString();
	}
	
	public static String removeTags( String input ) {
		// strip comments
		input = stripStringPair( input, "<!--", "-->" );
		// strip script 
		input = stripStringPair( input, "<script", "</script>" );
		// strip tags
		input = stripStringPair( input, "<", ">" );
		
		return input;
	}
	
	private static String stripStringPair( String input, String tagStart, String tagEnd ) {
		StringBuffer builder = null; 
		int start = 0;
		int begin;
		int end;
		
		while( true ) {
			begin = input.indexOf( tagStart, start );
			end = input.indexOf( tagEnd, begin );
			
			if( builder == null ) {
				if( begin < 0 ) {
					return input;
				} else {
					builder = new StringBuffer();
				}
			}
			
			if( begin < 0 ) {
				builder.append( input.substring( start ) );
				break;
			} else {
				builder.append( input.substring( start, begin ) );
			}
			
			start = end + tagEnd.length();
		}
		
		return builder.toString();
	}

	// replaces all delimiters with a space
	public static String removeDelims( String s, String delims ) {
		for( int i = 0; i < delims.length(); i++ )
			s.replace( delims.charAt(i), ' ' );

		return s;
	}	

/*	private String removeTags( String s ) {
		String ret = "";
		
		String tok0 = null;
		String tok1 = null;

		StringTokenizer tok = new StringTokenizer( s, "<>", true );
		
		try {
			tok0 = tok.nextToken();
			ret += tok0;
		}
		catch( Exception e ) { return ret; }

		try { tok1 = tok.nextToken(); }
		catch( Exception e ) { return ret; }

		while( tok.hasMoreTokens() ) {
			String token = tok.nextToken();
			if( !token.equals(">") || !tok0.equals("<") )
				ret += tok1;
			tok0 = tok1;
			tok1 = token;
		}
		
		ret += tok1;
		
		return ret.replaceAll("<>", "");
	} */

}
