/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.io.RandomAccessFile;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 * @author metzler
 *
 */
public class AveragePrecisionEvaluator implements Evaluator {

	protected HashMap judgments = null;
	protected HashMap totalRelevant = null;
	
	public AveragePrecisionEvaluator( String relFile ) {
		totalRelevant = new HashMap();
		judgments = getJudgments( relFile );		
	}

	private HashMap getJudgments( String relFile ) {
		RandomAccessFile in = null;
		HashMap judgments = new HashMap();
		
		try { in = new RandomAccessFile( relFile, "r" ); }
		catch( Exception e ) {
			System.err.println( "Error opening relevance file!" );
			return judgments;
		}
		
		try {
			String str = null;
			while( ( str = in.readLine() ) != null ) {
				StringTokenizer tok = new StringTokenizer( str );
				String queryID = tok.nextToken();
				tok.nextToken();
				String docID = tok.nextToken();
				int judgment = Integer.parseInt( tok.nextToken() );
				if( judgment != 0 ) {
					judgments.put( queryID + "," + docID, null );
					Integer count = (Integer)totalRelevant.get( queryID );
					if( count == null )
						totalRelevant.put( queryID, new Integer( 1 ) );
					else
						count = new Integer( count.intValue() + 1 );
				}
			}
		}
		catch( Exception e ) {
			System.err.println( "Error reading relevance file!" );
			return judgments;
		}
		
		return judgments;
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
				String key = ranking.instanceID + "," + item.classID;
				if( judgments.containsKey( key ) ) {
					numRelevant++; 
					avgp += numRelevant / (1.0*item.rank);
				}
			}
			Integer count = (Integer)totalRelevant.get( ""+ranking.instanceID );
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
