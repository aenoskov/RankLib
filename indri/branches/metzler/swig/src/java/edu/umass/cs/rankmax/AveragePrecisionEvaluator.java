/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public class AveragePrecisionEvaluator implements Evaluator {

	// TREC relevance judgments
	protected TRECJudgments trecJudgments = null;
	
	public AveragePrecisionEvaluator( String relFile ) {
		trecJudgments = TRECJudgments.getTRECJudgments( relFile );
	}

	public double evaluate( Ranking [] results ) {
		double map = 0.0;
		int total = 0;
		
		for( int i = 0; i < results.length; i++ ) {
			Ranking ranking = results[i];
			
			// compute average precision for this ranking
			int numRelevant = 0;
			double avgp = 0.0;
			for( int rank = 1; rank <= ranking.size(); rank++ ) {
				Ranking.RankedItem item = (Ranking.RankedItem)ranking.get( i-1 );				
				if( trecJudgments.isRelevant( ranking.instanceID+"", item.classID ) ) {
					numRelevant++; 
					avgp += numRelevant / (1.0*rank);
				}
			}
			Integer count = (Integer)trecJudgments.totalRelevant.get( ""+ranking.instanceID );
			if( count != null ) {
				avgp /= ( 1.0*count.intValue() );
				map += avgp;
				total++;
			}
		}
		
		if( total != 0 )
			map /= ( 1.0*total );
		else
			map = 0.0; 
		
		return map;
	}

}
