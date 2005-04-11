package edu.umass.cs.recap;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.util.Collections;
import java.util.Vector;

import javax.swing.border.EmptyBorder;

/*
 * Created on Nov 7, 2004
 *
 */

/**
 * @author Don Metzler
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class QuickFindBorder extends EmptyBorder {

	private RecapStyledDocument doc = null;
	private Vector matches = null;
	private int byteLength = -1;
	
	public QuickFindBorder( RecapStyledDocument doc ) {
		// add space onto the right only
		super( 0, 0, 0, 20 );

		this.doc = doc;
		this.matches = (Vector)doc.getViewableSentenceMatches().clone();
		this.byteLength = doc.getLength();		

		Collections.sort( matches );		
	}
	
	public void setMatches( Vector m ) {
		matches = (Vector)m.clone();
		Collections.sort( matches );		
	}
	
	public boolean isBorderOpaque() {
		return true;
	}
	
	public void paintBorder(Component c, Graphics g, int x, int y, int width, int height ) {
		Match lastMatch = null;
		for( int i = 0; i < matches.size(); i++ ) {
			Match m = (Match)matches.elementAt(i);
			// this ensures we don't mark the same match twice
			if( lastMatch != null && m.extentCompareTo( lastMatch ) == 0 )
				continue;
			int begin = doc.getScreenPos( m.begin );
			int end = doc.getScreenPos( m.end );
			int top = (int)(height*begin/(byteLength*1.0));
			int bottom = (int)(height*end/(byteLength*1.0));
	
			g.setColor( new Color(1.0f, 0.0f, 0.0f, 0.5f) );
			g.fillRect( width - right, top, width, bottom-top );
			g.setColor( new Color(1.0f, 0.0f, 0.0f, 1.0f) );
			g.drawRect( width - right, top, width, bottom-top );
			//g.fillRect( 0, top, width, bottom-top );
			lastMatch = m;
		}
	}
}
