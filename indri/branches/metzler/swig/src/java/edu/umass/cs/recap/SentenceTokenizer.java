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
	private static final String delims = " \b\t\n\f\r\'\"\\~`@#$%^&*()-=_+{}|[]:;,/<>";

	// sentence we're segmenting
	private String sentence = null;

	// context
	private String prevTerm = null;
	private String curTerm = null;
	private String nextTerm = null;
	
	public SentenceTokenizer( String s ) {
		super( s, delims );
		s = sentence;
		if( hasMoreTokens() )
			curTerm = nextToken();
		else
			curTerm = "";
	}
	
	// returns the next *sentence*
	public String nextSentence() {
		String ret = new String("");
		boolean foundSentence = false;
		
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
