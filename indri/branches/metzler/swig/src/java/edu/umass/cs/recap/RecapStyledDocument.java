package edu.umass.cs.recap;
import java.util.Vector;

import javax.swing.text.DefaultStyledDocument;
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
public class RecapStyledDocument extends DefaultStyledDocument {

	private String docName = null;
	private int byteLength = -1;
	private Vector byteMatches = null;
	
	public RecapStyledDocument() {
		docName = "FIXME";
		byteLength = -1;
		byteMatches = new Vector();
	}
	
	public void addMatch( int begin, int end ) {
		byteMatches.add( new Match( begin, end ) );
	}
	
	public Vector getMatches() {
		return byteMatches;
	}
	
	public void setByteLength( int length ) {
		this.byteLength = length;
	}
	
	public int getByteLength() {
		return byteLength;
	}
	
	public void setName( String name ) {
		docName = name;
	}
	
	public String getName() {
		return docName;
	}
}
