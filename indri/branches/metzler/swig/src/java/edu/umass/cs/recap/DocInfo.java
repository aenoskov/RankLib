package edu.umass.cs.recap;
/*
 * Created on Sep 28, 2004
 *
 * DocInfo Class
 * 
 */

/**
 * @author Don Metzler
 *
 * Encapsulation of useful document metdata
 * 
 */
public class DocInfo implements Comparable {

	private String docID;
	private int docNum;
	
	public DocInfo( String docID, int docNum ) {
		this.docID = docID;
		this.docNum = docNum;
	}
	
	public String getDocID() {
		return docID;
	}
		
	public int getDocNum() {
		return docNum;
	}
	
	public String toString() {
		return docID;
	}
	
	public int compareTo( Object info ) {
		return docID.compareTo( ((DocInfo)info).getDocID() );
	}
}
