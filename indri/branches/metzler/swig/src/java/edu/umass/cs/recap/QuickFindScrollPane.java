package edu.umass.cs.recap;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.Vector;

import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.border.Border;
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
public class QuickFindScrollPane extends JScrollPane implements MouseListener {
	
	private int byteLength = -1;
	private Vector matches = null;
	private Border border = null;

	private ArrayList queryPositions = null;	
	
	public QuickFindScrollPane( Vector matches, ArrayList queryPositions, int byteLength ) {
		this.matches = matches;
		this.byteLength = byteLength;
		this.queryPositions = queryPositions;
		
		border = new QuickFindBorder( matches, byteLength );		
		this.setBorder( border );
		
		this.addMouseListener( this );
	}

	public void setMatches( Vector matches ) {		
		this.matches = matches;
		border = new QuickFindBorder( matches, byteLength ); 
		this.setBorder( border );
	}
	
	public void mouseClicked(MouseEvent event) {
		Point p = event.getPoint();
		
		int height = this.getHeight();
		int width = this.getWidth();
		
		for( int i = 0; i < matches.size(); i++ ) {
			Match m = (Match)matches.elementAt(i);
			int top = (int)(height*m.begin/(byteLength*1.0));
			int bottom = (int)(height*m.end/(byteLength*1.0));
			
			Rectangle r = new Rectangle( 0, top, width, bottom-top );
			if( r.contains( p ) ) {
				JTextPane textPane = (JTextPane)this.getViewport().getComponent( 0 );
				int curPos = textPane.getCaretPosition();
				textPane.grabFocus();
				if( curPos > m.end )
					textPane.setCaretPosition( m.begin );
				else
					textPane.setCaretPosition( m.end );
				textPane.setCaretPosition( m.begin );
				textPane.moveCaretPosition( m.end );
				
				Match queryPos = (Match)queryPositions.get( i );
				System.out.println( "MATCH = " + queryPos );
			}
		}
	}

	public void mouseEntered(MouseEvent arg0) {}

	public void mouseExited(MouseEvent arg0)  {}

	public void mousePressed(MouseEvent arg0) {}

	public void mouseReleased(MouseEvent arg0) {}
}
