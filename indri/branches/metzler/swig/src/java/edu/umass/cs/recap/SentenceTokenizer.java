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
	
	public SentenceTokenizer( String s ) {
		super( s, delims );
		firstToken = true;
	}
	
	// returns the next *sentence*
	public String nextSentence() {
		String ret = new String("");
		boolean foundSentence = false;

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
		
		while( hasMoreTokens() ) {
			ret += curTerm + " ";

			nextTerm = nextToken();

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
				return ret;
			}
		}

		ret += curTerm + " ";
		//System.out.println("SENTENCE(2) = " + ret);
		return ret;
	}
}
