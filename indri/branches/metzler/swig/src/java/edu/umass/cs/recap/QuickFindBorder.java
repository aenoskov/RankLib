package edu.umass.cs.recap;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
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

	private Vector matches = null;
	private int byteLength = -1;
	
	public QuickFindBorder( Vector matches, int byteLength ) {
		// add space onto the right only
		super( 0, 0, 0, 20 );
		
		this.matches = matches;
		this.byteLength = byteLength;
	}
	
	public boolean isBorderOpaque() {
		return true;
	}
	
	public void paintBorder(Component c, Graphics g, int x, int y, int width, int height ) {
		for( int i = 0; i < matches.size(); i++ ) {
			Match m = (Match)matches.elementAt(i);
			int top = (int)(height*m.begin/(byteLength*1.0));
			int bottom = (int)(height*m.end/(byteLength*1.0));
	
			g.setColor( new Color(1.0f, 0.0f, 0.0f, 0.5f) );
			g.fillRect( width - right, top, width, bottom-top );
			g.setColor( new Color(1.0f, 0.0f, 0.0f, 1.0f) );
			g.drawRect( width - right, top, width, bottom-top );
			//g.fillRect( 0, top, width, bottom-top );
		}
	}
}
