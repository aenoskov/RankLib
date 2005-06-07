/*
 * Created on June 7, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.Collections;

/**
 * @author Don Metzler
 *
 */
public class BPrefEvaluator implements Evaluator {

	// TREC relevance judgments
	protected TRECJudgments trecJudgments = null;
	
	public BPrefEvaluator( String relFile ) {
		trecJudgments = TRECJudgments.getTRECJudgments( relFile );
	}

	public double evaluate( Ranking [] results ) {
		double avgBpref = 0.0;
		int total = 0;
		
		for( int i = 0; i < results.length; i++ ) {
			Ranking ranking = results[ i ];
			Collections.sort( results[i] );
			
			int totalRelevant = ( (Integer)trecJudgments.totalRelevant.get( ranking.instanceID ) ).intValue();
			if( totalRelevant == 0 )
				continue;
			else
				total++;
				
			double bpref = 0.0; 
			int numNonRelevantSeen = 0;
			for( int rank = 1; rank <= ranking.size(); rank++ ) {
				Ranking.RankedItem item = (Ranking.RankedItem)ranking.get( rank - 1 );
				if( trecJudgments.isRelevant( ranking.instanceID, item.classID ) )
					bpref += 1.0 - ( 1.0*numNonRelevantSeen / totalRelevant );
				else if( numNonRelevantSeen < totalRelevant && trecJudgments.isNonRelevant( ranking.instanceID, item.classID ) )
					numNonRelevantSeen++;
			}
			bpref /= totalRelevant;			
			avgBpref += bpref;
		}
		
		return avgBpref / total;		
	}

}
