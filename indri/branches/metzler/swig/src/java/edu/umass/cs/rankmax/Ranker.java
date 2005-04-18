/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public interface Ranker {

	// returns a ranking for a given setting of parameters
	public Ranking [] getRankings( Parameters p );

}
