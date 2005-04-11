package edu.umass.cs.recap;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Vector;

import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.ToolTipManager;

/*
 * Created on Nov 7, 2004
 *
 */

/**
 * @author Don Metzler
 *
 * A ScrollPane implementation that adds a small border
 * to the right side of the vertical scroll and marks
 * a set of matches. The matches can then be clicked on to
 * allow for easy/quick jumping to the matches.
 *
 */
public class QuickFindScrollPane extends JScrollPane {
	
	private RecapStyledDocument doc = null;
	private Vector matches = null;
	private QuickFindBorder border = null;
	private int byteLength = -1;

	private ArrayList queryPositions = null;	
	
	public QuickFindScrollPane( RecapStyledDocument doc ) {
		this.doc = doc;
		this.byteLength = doc.getLength();

		this.matches = doc.getViewableSentenceMatches();
		this.queryPositions = doc.getViewableQueryPositions();
				
		border = new QuickFindBorder( doc );		
		this.setBorder( border );
		
		ToolTipManager.sharedInstance().registerComponent( this );
	}

	public void setMatches( Vector matches, ArrayList queryPositions ) {		
		this.matches = matches;
		this.queryPositions = queryPositions;
		border.setMatches( matches );
	}
	
	public Pair getMatch( Point p ) {
		int height = this.getHeight();
		int width = this.getWidth();

		for( int i = 0; i < matches.size(); i++ ) {
			Match m = (Match)matches.elementAt(i);
			int begin = doc.getScreenPos( m.begin );
			int end = doc.getScreenPos( m.end );
			int top = (int)(height*begin/(byteLength*1.0));
			int bottom = (int)(height*end/(byteLength*1.0));
			
			Rectangle r = new Rectangle( 0, top, width, bottom-top );
			if( r.contains( p ) ) {
				return new Pair( m, queryPositions.get( i ) );
			}
		}
		
		return null;
	}
	
	public Match processClick( MouseEvent event ) {
		Match queryPos = null;
		
		Pair p = getMatch( event.getPoint() );
		if( p == null )
			return null;

		Match m = (Match)p.left;		
		if( m != null ) {
			JTextPane textPane = (JTextPane)this.getViewport().getComponent( 0 );
			RecapStyledDocument styledDoc = (RecapStyledDocument)textPane.getDocument();
			int curPos = textPane.getCaretPosition();
			textPane.grabFocus();
			int begin = styledDoc.getScreenPos( m.begin );
			int end = styledDoc.getScreenPos( m.end );
			if( curPos > end )
				textPane.setCaretPosition( begin );
			else
				textPane.setCaretPosition( end );
			textPane.setCaretPosition( begin );
			textPane.moveCaretPosition( end );
			
			queryPos = (Match)p.right;						
		}
				
		return queryPos;
	}
	
	public String getToolTipText( MouseEvent e ) {
		Pair p = getMatch( e.getPoint() );
		if( p == null )
			return null;
		Match m = (Match)p.left;
		return "<html><b>Score:</b> " + m.type + "</html>";
	}
}
