package edu.umass.cs.recap;
import java.awt.Color;
import java.util.ArrayList;
import java.util.Collections;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.ImageIcon;
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

	// named entity matches
	protected Vector namedEntityMatches = null;
	
	// annotation highlighting styles and colors 
	protected final String [] highlightNames = new String [] {
			"annotation0", "annotation1", "annotation2", "annotation3", "annotation4" };
	protected final Color [] colorNames = new Color [] {
			Color.GREEN, Color.CYAN, Color.RED, Color.MAGENTA, Color.ORANGE };  

	// general text highlighting
	protected Match highlight = null;
	
	// create a new RecapStyledDocument
	public RecapStyledDocument( String text, Style defaultStyle ) {
		// add text to document with default style
		try { insertString( 0, text, defaultStyle ); }
		catch( Exception e ) { /* do nothing */ }
		this.defaultStyle = defaultStyle;

		initStyles();
		
		// initialize match vectors
		sentenceMatches = new Vector();
		viewableSentenceMatches = new Vector();
		annotationMatches = new Vector();
		namedEntityMatches = new Vector();

		queryPositions = new ArrayList();
		
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
			applyStyle( "default", highlight.begin, highlight.end );
			render( highlight.begin, highlight.end ); // renders the previously highlighted portion
		}
		highlight = new Match( begin, end );
		applyStyle( "highlight", begin, end );
		render( begin, end );
	}

/*	protected void setSentenceMatches( Vector matches ) {
		viewableSentenceMatches = matches;
	}*/

	public Vector getSentenceMatches() {
		return sentenceMatches;
	}
	
	protected void setViewableSentenceMatches( Vector matches ) {
		viewableSentenceMatches = matches;
	}
	
	public Vector getViewableSentenceMatches() {
		return viewableSentenceMatches;
	}

	public ArrayList getQueryPositions() {
		return queryPositions;
	}
	
	// applies a style to the given segment of text
	protected void applyStyle( String style, int begin, int end ) {
		try { replace( begin, end - begin, getText(begin, end-begin), getStyle( style ) );	}
		catch(Exception e) { /* do nothing */ }							
	}
	
	// renders a portion of the document
	protected void render( int begin, int end ) {
		// TODO: tidy this up a bit
		for( int i = 0; i < sentenceMatches.size(); i++ ) {
			Match m = (Match)sentenceMatches.elementAt( i );
			if( ( m.begin <= begin && m.end >= begin ) ||
				( m.begin >= begin && m.end <= end ) ||
				( m.begin <= end && m.end >= end ) )
				applyStyle( "sentmatch", m.begin, m.end );
		}
		
		for( int i = 0; i < annotationMatches.size(); i++ ) {
			Match m = (Match)annotationMatches.elementAt( i );
			if( ( m.begin <= begin && m.end >= begin ) ||
				( m.begin >= begin && m.end <= end ) ||
				( m.begin <= end && m.end >= end ) )
				applyStyle( "annotation"+(int)m.type, m.begin, m.end );
		}

		for( int i = 0; i < namedEntityMatches.size(); i++ ) {
			Match m = (Match)namedEntityMatches.elementAt( i );
			if( ( m.begin <= begin && m.end >= begin ) ||
				( m.begin >= begin && m.end <= end ) ||
				( m.begin <= end && m.end >= end ) )
				// TODO: fix this to allow different styles for each named entity
				applyStyle( "ne:person", m.begin, m.end );
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
		try {
			tok0 = tok.nextToken().toLowerCase();
			pos0 = 0;
			tok1 = tok.nextToken().toLowerCase();
			pos1 = tok0.length();
		}
		catch( Exception e ) { return; }
		while( tok.hasMoreTokens() ) {
			String token = tok.nextToken().toLowerCase();
			if( token.equals(">") && tok0.equals("<") ) {
				Style s = null;
				if( tok1.charAt( 0 ) == '/' )
					s = getStyle( "ne:" + tok1.substring( 1 ) );
				else
					s = getStyle( "ne:" + tok1 );
				if( s != null ) {
					// TODO: fix this to change 'type' for different types of named entities
					namedEntityMatches.add( new Match( pos0, pos1 + tok1.length() + 1, -1 ) );
					applyStyle( s.getName(), pos0, pos1 + tok1.length() + 1 );
				}
			}
			tok0 = tok1;
			pos0 = pos1;
			tok1 = token;
			pos1 = pos1 + tok0.length();
		}
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
	
		// TODO: do this a bit more elegantly
		// named entity styles
		s = addStyle( "ne:person", defaultStyle );
		//StyleConstants.setIcon( s, new ImageIcon("edu/umass/cs/recap/images/recap-small.png" ) );
		StyleConstants.setForeground( s, Color.WHITE);
		StyleConstants.setFontSize( s, 1 );

		s = addStyle( "ne:organization", defaultStyle );
		//StyleConstants.setIcon( s, new ImageIcon("edu/umass/cs/recap/images/recap-small.png" ) );
		StyleConstants.setForeground( s, Color.WHITE);
		StyleConstants.setFontSize( s, 1 );

		s = addStyle( "ne:location", defaultStyle );
		//StyleConstants.setIcon( s, new ImageIcon("edu/umass/cs/recap/images/recap-small.png" ) );
		StyleConstants.setForeground( s, Color.WHITE);
		StyleConstants.setFontSize( s, 1 );

		s = addStyle( "ne:money", defaultStyle );
		//StyleConstants.setIcon( s, new ImageIcon("edu/umass/cs/recap/images/recap-small.png" ) );
		StyleConstants.setForeground( s, Color.WHITE);
		StyleConstants.setFontSize( s, 1 );

		s = addStyle( "ne:date", defaultStyle );
		//StyleConstants.setIcon( s, new ImageIcon("edu/umass/cs/recap/images/recap-small.png" ) );
		StyleConstants.setForeground( s, Color.WHITE);
		StyleConstants.setFontSize( s, 1 );

		s = addStyle( "ne:time", defaultStyle );
		//StyleConstants.setIcon( s, new ImageIcon("edu/umass/cs/recap/images/recap-small.png" ) );
		StyleConstants.setForeground( s, Color.WHITE);
		StyleConstants.setFontSize( s, 1 );

		s = addStyle( "ne:percent", defaultStyle );
		//StyleConstants.setIcon( s, new ImageIcon("edu/umass/cs/recap/images/recap-small.png" ) );
		StyleConstants.setForeground( s, Color.WHITE);
		StyleConstants.setFontSize( s, 1 );
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
