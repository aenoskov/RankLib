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
				curTerm = nextToken();
			else
				curTerm = "";
			firstToken = false;
		}
		
		boolean lastDelim = false;

		while( hasMoreTokens() ) {			
			//ret += curTerm + " ";
			if( !lastDelim )
				ret += curTerm;

			nextTerm = nextToken();
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
				unparsedSentenceLength = ret.length();
				return RecapTools.removeDelims( RecapTools.removeTags( ret ), delims );
			}
		}

		if( !lastDelim )
			ret += curTerm;
		unparsedSentenceLength = ret.length();
		return RecapTools.removeDelims( RecapTools.removeTags( ret ), delims );
	}
		
	// returns the unparsed sentence length
	public int getUnparsedSentenceLength() {		
		return unparsedSentenceLength;
	}
}
