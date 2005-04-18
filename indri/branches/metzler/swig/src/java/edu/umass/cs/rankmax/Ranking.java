/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;
import java.util.Collections;

/**
 * @author Don Metzler
 *
 * A Ranking is an ArrayList of RankedItems
 */
public class Ranking extends ArrayList {

	public int instanceID = 0;
	
	public Ranking( int instanceID ) {
		super();
		this.instanceID = instanceID;
	}
	
	public boolean add( Object o ) {
		if( !( o instanceof RankedItem ) )
			return false;
		
		super.add( o );
		Collections.sort( this );
		
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
	}
	
}
