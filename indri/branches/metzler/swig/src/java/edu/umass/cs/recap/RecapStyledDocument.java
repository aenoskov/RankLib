package edu.umass.cs.recap;
import java.awt.Color;
import java.util.Collections;
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
	
	// annotation matches in "explore" mode
	protected Vector annotationMatches = null;

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
	}
	
	public void addSentenceMatch( int begin, int end, double score ) {
		Match m = new Match( begin, end, score );
		sentenceMatches.add( m );
		viewableSentenceMatches.add( m );
		applyStyle( "sentmatch", begin, end );
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
	}
	
	public Vector getViewableSentenceMatches() {
		return viewableSentenceMatches;
	}
	
	// applies a style to the given segment of text
	protected void applyStyle( String style, int begin, int end ) {
		try { replace( begin, end - begin, getText(begin, end-begin), getStyle( style) );	}
		catch(Exception e) { /* do nothing */ }							
	}
	
	// renders a portion of the document
	protected void render( int begin, int end ) {
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
				applyStyle( "annotation"+m.type, m.begin, m.end );
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
	}
	
	// makes a 'deep copy' of this object 
	public Object clone() {
		RecapStyledDocument clone = null;
		try {
			clone = new RecapStyledDocument( getText( 0, getLength() ), defaultStyle );
		}
		catch( Exception e ) { /* do nothing */ }

		// copy sentence matches
		for( int i = 0; i < sentenceMatches.size(); i++ ) {
			Match m = (Match)sentenceMatches.elementAt( i );
			clone.addSentenceMatch( m.begin, m.end, m.type );
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
	}
}
