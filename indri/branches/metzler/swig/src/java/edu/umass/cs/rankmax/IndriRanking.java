/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import edu.umass.cs.indri.ScoredExtentResult;

/**
 * @author metzler
 *
 */
public class IndriRanking extends Ranking {

	public IndriRanking( int queryID, ScoredExtentResult [] results, String [] docNames ) {
		super( queryID );
		for( int i = 0; i < results.length; i++ )
			add( new Ranking.RankedItem( docNames[i], i+1, results[i].score ) );
	}
	
}
