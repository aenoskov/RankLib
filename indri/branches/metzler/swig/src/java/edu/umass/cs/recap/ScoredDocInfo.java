package edu.umass.cs.recap;
import java.util.Vector;

/*
 * Created on Nov 6, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/**
 * @author Don Metzler
 *
 * Encapsulates pertinent information about a document
 * after it has been scored
 * 
 */
public class ScoredDocInfo implements Comparable {

	// document information
	public int docID;
	public String docName;
	public int begin;
	public int end;
	public double score;
	public Vector extents;
	public Vector queryPositions;

	// date information
	public int date;
	public int month;
	public int year;
	
	public ScoredDocInfo( int docID, double score, int begin, int end, Vector extents, Vector queryPositions ) {
		this.docID = docID;
		this.score = score;
		this.begin = begin;
		this.end = end;
		this.extents = extents;
		this.queryPositions = queryPositions;
		this.date = -1;
		this.month = -1;
		this.year = -1;
	}
		
	public String toString() {
		return "[docName="+docName+",docID="+docID+",score="+score+",|extents|="+extents.size()+",(" + month + "/" + date + "/" + year + ")]";
	}

	public int compareTo(Object o) {
		ScoredDocInfo info = (ScoredDocInfo)o;
		if( score < info.score )
			return -1;
		else if( score > info.score )
			return 1;
		return 0;
	}
}
