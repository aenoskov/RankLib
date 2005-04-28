/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;

/**
 * @author Don Metzler
 *
 * A Ranking is an ArrayList of RankedItems
 */
public class Ranking extends ArrayList {

	public String instanceID = null;
	
	public Ranking( String instanceID ) {
		super();
		this.instanceID = instanceID;
	}
	
	public boolean add( Object o ) {
		if( !( o instanceof RankedItem ) )
			return false;		
		super.add( o );
		return true;
	}
	
	public static class RankedItem implements Comparable {
		public String classID = null;
		public double score = 0.0;
		
		public RankedItem( String classID, double score ) {
			this.classID = classID;
			this.score = score;
		}

		public int compareTo( Object o ) {
			RankedItem item = (RankedItem)o;
			if( score > item.score )
				return -1;
			else if( score < item.score )
				return 1;
			else if( score == item.score )
				return classID.compareTo( item.classID );			
			return 0;
		}
		
		public String toString() {
			return "class id: " + classID + ", score: " + score + "\n"; 
		}
	}
	
	// prints the results in TREC format:
	// instanceID Q0 classID rank score run_identifier
	public void print( int num, String runID ) {
		int max = size();
		if( num != 0 && num < size() )
			max = num;
		for( int i = 0; i < max; i++ ) {
			RankedItem item = (RankedItem)get( i );
			System.out.println( instanceID + " Q0 " + item.classID + " " + (i+1) + " " + item.score + " " + runID ); 
		}
	}
	
	public String toString() {
		String ret = "instance id: " + instanceID + "\n";		
		for( int i = 0; i < size(); i++ )
			ret += "rank " + (i+1) + ": " + (RankedItem)get( i ); 
		return ret;
	}
}
