/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public class PrecisionAtNEvaluator implements Evaluator {

	// TREC relevance judgments
	protected TRECJudgments trecJudgments = null;
	
	// N -- the level at which to evaluate precision
	protected int N = 0;
	
	public PrecisionAtNEvaluator( String relFile, int N ) {
		trecJudgments = TRECJudgments.getTRECJudgments( relFile );
		this.N = N;
	}

	public double evaluate( Ranking[] results ) {
		double precAtN = 0.0;
		int total = 0;
		
		for( int i = 0; i < results.length; i++ ) {
			Ranking ranking = results[i];
			
			// compute precision at N for this ranking
			int numRelevant = 0;
			double avgp = 0.0;
			for( int rank = 1; rank <= N; rank++ ) {
				if( rank > ranking.size() ) { break; }
				Ranking.RankedItem item = (Ranking.RankedItem)ranking.get( rank - 1 );
				if( trecJudgments.isRelevant( ranking.instanceID, item.classID ) ) 
					numRelevant++;
			}
			Integer count = (Integer)trecJudgments.totalRelevant.get( ranking.instanceID );
			if( count != null ) {
				precAtN += numRelevant / N;
				total++;
			}
		}
		
		if( total != 0 )
			precAtN /= ( 1.0*total );
		else
			precAtN = 0.0; 
		
		return precAtN;
	}

}
