/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;

/**
 * @author metzler
 *
 */
public class Ranking extends ArrayList {

	public int instanceID = 0;
	
	public Ranking( int instanceID ) {
		super();
		this.instanceID = instanceID;
	}
	
	public class RankedItem {
		public String classID = null;
		public int rank = 0;
		public double score = 0.0;
		
		public RankedItem( String classID, int rank, double score ) {
			this.classID = classID;
			this.rank = rank;
			this.score = score;
		}
	}
	
}
