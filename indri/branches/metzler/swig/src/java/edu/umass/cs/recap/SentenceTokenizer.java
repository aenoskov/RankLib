package edu.umass.cs.recap;
import java.util.StringTokenizer;
/*
 * Created on Nov 17, 2004
 *
 */

/**
 * @author Don Metzler
 */
public class SentenceTokenizer extends StringTokenizer {

	// term delimiters
	protected static final String delims = " \b\t\n\f\r\'\"\\~`@#$%^&*()-=_+{}|[]:;,/<>";

	// context
	protected String prevTerm = null;
	protected String curTerm = null;
	protected String nextTerm = null;
	
	// whether or not we're on the first token or not
	protected boolean firstToken = true;

	// unparsed sentence length
	protected int unparsedSentenceLength = 0;
	
	public SentenceTokenizer( String s ) {
		super( s, delims, true );
		firstToken = true;
	}
	
	// returns the next *sentence*
	public String nextSentence() {
		String ret = new String("");		
		boolean foundSentence = false;
		unparsedSentenceLength = 0;
		
		// this ensures that hasMoreTokens() returns true
		// if the sentence we're tokenizing only contains a
		// single term
		if( firstToken ) {
			if( hasMoreTokens() )
				curTerm = nextToken(); //nextNonDelimToken(); //nextToken();
			else
				curTerm = "";
			firstToken = false;
		}
		
		boolean lastDelim = false;

		while( hasMoreTokens() ) {			
			//ret += curTerm + " ";
			if( !lastDelim )
				ret += curTerm;

			nextTerm = nextToken(); //nextNonDelimToken(); //nextToken();
			if( delims.indexOf( nextTerm ) != -1 ) {
				lastDelim = true;
				ret += nextTerm;
				continue;
			}
			else
				lastDelim = false;

			// this is true if we're out of non-delimiter tokens
			if( nextTerm == null )
				break;

			char lastChar = curTerm.charAt( curTerm.length() - 1 );

			if( lastChar == '?' || lastChar == '!' || lastChar == ';' ) {
		        foundSentence = true;
		    }
			else if( lastChar == '.' ) {
				// next word is lowercase => not sentence boundary
		        if( nextTerm != "" && nextTerm.charAt( 0 ) < 'A' || nextTerm.charAt( 0 ) > 'Z' ) {
		        	foundSentence = false;
		        }
		        // current term uppercase and contains more than 1 '.' => not sentence boundary
		        else if( curTerm.charAt( 0 ) >= 'A' && curTerm.charAt( 0 ) <= 'Z' && curTerm.indexOf( ".", 0 ) < curTerm.length()-1 ) {
		        	foundSentence = false;
		        }
		        // current term uppercase and shorter than 5 characters => not sentence boundary
		        else if( curTerm.charAt( 0 ) >= 'A' && curTerm.charAt( 0 ) <= 'Z' && curTerm.length() < 5 ) {
		        	foundSentence = false;
		        }
		        else 
		        	foundSentence = true;
			}
			else {
				foundSentence = false;
			}	
					
			prevTerm = curTerm;
			curTerm = nextTerm;
			
			if( foundSentence ) {
				//System.out.println("SENTENCE(1) = " + ret);
				unparsedSentenceLength = ret.length();
				return removeDelims( removeTags( ret ) );
				//return ret;
			}
		}

		//ret += curTerm + " ";
		ret += curTerm;
		//System.out.println("SENTENCE(2) = " + ret);
		unparsedSentenceLength = ret.length();
		return removeDelims( removeTags( ret ) );
		//return ret;
	}
	
	// returns the next token that is not a delimiter, or null if there are no more
/*	private String nextNonDelimToken() {
		String ret = null;
		
		int pos = 0;
		try {
			while( pos >= 0 ) {
				ret = nextToken();
				pos = delims.indexOf( ret );
				unparsedSentenceLength = newUnparsedSentenceLength;
				newUnparsedSentenceLength += ret.length();
			}
		}
		catch( Exception e ) { ret = null; } // this occurs if we run out of tokens
		
		return ret;
	}*/
	
	// returns the unparsed sentence length
	public int getUnparsedSentenceLength() {		
		return unparsedSentenceLength;
	}
	
	// TODO: put this into a "RecapTools" class
	// removes any tags from the string
	private String removeTags( String s ) {
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
			if( token.equals(">") && tok0.equals("<") ) { /* tok1 is tag name, so don't add it to the return string */ }
			else { ret += tok1; }
			tok0 = tok1;
			tok1 = token;
		}
		
		ret += tok1;
		
		return ret.replaceAll("<>", "");
	}
	
	// replaces all delimiters with a space
	private String removeDelims( String s ) {
		for( int i = 0; i < delims.length(); i++ )
			s.replace( delims.charAt(i), ' ' );

		return s;
	}	
}
