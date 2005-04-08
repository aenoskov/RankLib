package edu.umass.cs.recap;
import java.awt.Color;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;

/*
 * Created on Nov 7, 2004
 *
 */

/**
 * @author Don Metzler
 *
 * Multi-purpose DefaultStyledDocument subclass.
 * Used for:
 *    - sentence highlighting in "analyze" mode
 *    - annotation highlighting in "explore" mode
 *    - general text highlighting         
 */
public class RecapStyledDocument extends DefaultStyledDocument {

	// default style of this document
	protected Style defaultStyle = null;  
	
	// sentence matches in "analyze" mode
	protected Vector sentenceMatches = null;
	protected Vector viewableSentenceMatches = null;

	protected ArrayList queryPositions = null;
	
	// annotation matches in "explore" mode
	protected Vector annotationMatches = null;
	
	// annotation highlighting styles and colors 
	protected final String [] highlightNames = new String [] {
			"annotation0", "annotation1", "annotation2", "annotation3", "annotation4" };
	protected final Color [] colorNames = new Color [] {
			Color.GREEN, Color.CYAN, Color.RED, Color.MAGENTA, Color.ORANGE };  

	// tags to be ignored (stripped out of text before being displayed)
	protected final String [] ignoreTags = new String [] {
			"person", "organization", "location", "date", "time", "percent", "money" };
	
	private HashMap ignoreMap = null; 

	// general text highlighting
	protected Match highlight = null;
	
	// position lookup
	private int [] positionLookup = null;
	
	// create a new RecapStyledDocument
	public RecapStyledDocument( String text, Style defaultStyle ) {
		// add text to document with default style
		try { insertString( 0, text, defaultStyle ); }
		catch( Exception e ) { /* do nothing */ }
		this.defaultStyle = defaultStyle;

		ignoreMap = new HashMap();
		
		initStyles();
		initIgnoreTags();
		
		// initialize match vectors
		sentenceMatches = new Vector();
		viewableSentenceMatches = new Vector();
		annotationMatches = new Vector();
		//namedEntityMatches = new Vector();

		queryPositions = new ArrayList();

		positionLookup = new int[ text.length() ];
		initNamedEntityMatches();
	}
	
	public void addSentenceMatch( int begin, int end, double score, Match queryPos ) {
		Match m = new Match( begin, end, score );
		sentenceMatches.add( m );
		viewableSentenceMatches.add( m );
		applyStyle( "sentmatch", begin, end );
		
		queryPositions.add( queryPos );
	}

	public void applySentenceMatchThreshold(double threshold) {
		Collections.sort( sentenceMatches );
		viewableSentenceMatches.clear();
		for( int i = 0; i < sentenceMatches.size(); i++ ) {
			Match m = (Match)sentenceMatches.elementAt( i );
			if( m.type >= threshold ) {
				applyStyle( "sentmatch", m.begin, m.end );
				viewableSentenceMatches.add( m );
			}
			else
				applyStyle( "nostyle", m.begin, m.end );
		}
	}
	
	public void addAnnotationMatch( int begin, int end, int highlightNum ) {
		annotationMatches.add( new Match( begin, end, highlightNum ) );
		applyStyle( "annotation"+highlightNum, begin, end );
	}
	
	public void setHighlight( int begin, int end ) {
		if( highlight != null ) {
			applyStyleDirect( "default", highlight.begin, highlight.end );
			render( highlight.begin, highlight.end ); // renders the previously highlighted portion
		}
		highlight = new Match( begin, end );
		applyStyleDirect( "highlight", begin, end );
		render( begin, end );
	}

	// returns the screen position corresponding to the actual position within the
	// document
	public int getScreenPos( int pos ) {
		return positionLookup[ pos - 1 ]; 
	}
	
/*	protected void setSentenceMatches( Vector matches ) {
		viewableSentenceMatches = matches;
	}*/

	public Vector getSentenceMatches() {
		return sentenceMatches;
	}
	
/*	protected void setViewableSentenceMatches( Vector matches ) {
		viewableSentenceMatches = matches;
	}*/
	
	public Vector getViewableSentenceMatches() {
		return viewableSentenceMatches;
	}

	public ArrayList getQueryPositions() {
		return queryPositions;
	}
	
	// applies a style to the given segment of text
	protected void applyStyle( String style, int begin, int end ) {
		try { replace( positionLookup[begin-1], positionLookup[end-1] - positionLookup[begin-1], getText(positionLookup[begin-1], positionLookup[end-1]-positionLookup[begin-1]), getStyle( style ) );	}
		catch(Exception e) { /* do nothing */ }							
	}

	// applies a style to the given segment of text
	protected void applyStyleDirect( String style, int begin, int end ) {
		try { replace( begin, end - begin, getText(begin, end-begin), getStyle( style ) );	}
		catch(Exception e) { /* do nothing */ }							
	}
	
	// renders a portion of the document
	protected void render( int begin, int end ) {
		// TODO: tidy this up a bit
		for( int i = 0; i < sentenceMatches.size(); i++ ) {
			Match m = (Match)sentenceMatches.elementAt( i );
			int mBegin = positionLookup[ m.begin - 1 ];
			int mEnd = positionLookup[ m.end - 1 ];  
			if( ( mBegin <= begin && mEnd >= begin ) ||
				( mBegin >= begin && mEnd <= end ) ||
				( mBegin <= end && mEnd >= end ) )
				applyStyleDirect( "sentmatch", mBegin, mEnd );
		}
		
		for( int i = 0; i < annotationMatches.size(); i++ ) {
			Match m = (Match)annotationMatches.elementAt( i );
			int mBegin = positionLookup[ m.begin - 1 ];
			int mEnd = positionLookup[ m.end - 1 ];  
			if( ( mBegin <= begin && mEnd >= begin ) ||
				( mBegin >= begin && mEnd <= end ) ||
				( mBegin <= end && mEnd >= end ) )
				applyStyleDirect( "annotation"+(int)m.type, mBegin, mEnd );
		}
	}
	
	// goes through the text and applies the styles to the named entities
	private void initNamedEntityMatches() {
		String tok0 = null;
		String tok1 = null;
		int pos0 = 0;
		int pos1 = 0;
		String text = null;
		try { text = getText( 0, this.getLength() ); }
		catch( Exception e ) { /* do nothing */ }
		StringTokenizer tok = new StringTokenizer( text, "<>", true );
		int actualPosition = -1;
		try {
			tok0 = tok.nextToken().toLowerCase();
			pos0 = 0;
			actualPosition = setPositions( pos0, 0, tok0.length(), true );
			tok1 = tok.nextToken().toLowerCase();
			pos1 = tok0.length();
		}
		catch( Exception e ) { return; }
		while( tok.hasMoreTokens() ) {
			String token = tok.nextToken().toLowerCase();
			if( token.equals(">") && tok0.equals("<") ) {
				boolean hasKey = false;
				if( tok1.charAt( 0 ) == '/' )
					hasKey = ignoreMap.containsKey( tok1.substring( 1 ) );
				else
					hasKey = ignoreMap.containsKey( tok1 );
				if( hasKey )
					actualPosition = setPositions( pos0, actualPosition-1, 1 + tok1.length() + 1, false ) - 1;					
				else
					actualPosition = setPositions( pos1, actualPosition, tok1.length(), true );
			}
			else
				actualPosition = setPositions( pos1, actualPosition, tok1.length(), true );
			tok0 = tok1;
			pos0 = pos1;
			tok1 = token;
			pos1 = pos1 + tok0.length();
		}

		String newText = "";
		for( int i = 0; i < text.length(); i++ ) {
			//System.out.print("["+positionLookup[i]+"]"+"("+text.charAt(i)+")");
			if( i == 0 && positionLookup[ i ] != -1 )
				newText += text.charAt( i );
			else if( i > 0 && positionLookup[ i ] != positionLookup[ i - 1 ] )
				newText += text.charAt( i );
		}
		//System.out.println();
		
		try { replace( 0, this.getLength(), newText, null ); }
		catch( Exception e ) { /* do nothing */ }
	}

	private int setPositions( int startPos, int actualPosition, int length, boolean inc ) {
		for( int i = 1; i <= length; i++ ) {
			if( inc )
				positionLookup[ startPos - 1 + i ] = actualPosition + i;
			else
				positionLookup[ startPos - 1 + i ] = actualPosition;
		}
		return positionLookup[ startPos - 1 + length ];
	}
	
	// initializes the document styles
	private void initStyles() {		
		// default style
		Style s = addStyle( "nostyle", defaultStyle );

		// sentence match style
		s = addStyle( "sentmatch", defaultStyle );
		StyleConstants.setForeground( s, Color.blue );
		StyleConstants.setItalic( s, true );

		// annotation match styles
		for(int i = 0; i < highlightNames.length; i++ ) {
			s = addStyle( highlightNames[i], defaultStyle );
			StyleConstants.setBackground( s, colorNames[i] );
			StyleConstants.setBold( s, true );
		}
		
		// highlighting style
		s = addStyle( "highlight", defaultStyle );
		StyleConstants.setBackground( s, Color.LIGHT_GRAY );
		StyleConstants.setForeground( s, Color.BLACK );
		//StyleConstants.setBold( s, true );	
	}

	// initializes the ignore tags
	private void initIgnoreTags() {
		for( int i = 0; i < ignoreTags.length; i++ )
			ignoreMap.put( ignoreTags[i], null );
	}
	
	// makes a copy of this object, with no formatting
	public RecapStyledDocument unformattedClone() {
		RecapStyledDocument clone = null;
		try {
			clone = new RecapStyledDocument( getText( 0, getLength() ), defaultStyle );
		}
		catch( Exception e ) { /* do nothing */ }
		
		return clone;		
	}
	
	// makes a 'deep copy' of this object 
/*	public Object clone() {
		RecapStyledDocument clone = null;
		try {
			clone = new RecapStyledDocument( getText( 0, getLength() ), defaultStyle );
		}
		catch( Exception e ) {  }

		clone.setSentenceMatches( sentenceMatches );
		clone.setViewableSentenceMatches( viewableSentenceMatches );
		
		// copy sentence matches
		for( int i = 0; i < viewableSentenceMatches.size(); i++ ) {
			Match m = (Match)viewableSentenceMatches.elementAt( i );
			clone.applyStyle( "sentmatch", m.begin, m.end );
		}
		
		// copy annotation matches
		for( int i = 0; i < annotationMatches.size(); i++ ) {
			Match m = (Match)annotationMatches.elementAt( i );
			clone.addAnnotationMatch( m.begin, m.end, (int)m.type );
		}

		// copy highlight
		if( highlight != null )
			clone.setHighlight( highlight.begin, highlight.end );
		
		return clone;
	} */
}
