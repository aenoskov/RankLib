/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.io.RandomAccessFile;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 * @author Don Metzler
 *
 */
public class TRECJudgments {

	public HashMap totalRelevant = null;
	public HashMap judgments = null;
		
	public TRECJudgments() {
		totalRelevant = new HashMap();
		judgments = new HashMap();
	}
	
	public boolean isRelevant( String queryID, String docID ) {
		return judgments.containsKey( getKey( queryID, docID ) );
	}
	
	public static String getKey( String queryID, String docID ) {
		return queryID + "," + docID;
	}
	
	public static TRECJudgments getTRECJudgments( String relFile ) {
		TRECJudgments results = new TRECJudgments();
		
		RandomAccessFile in = null;
		HashMap judgments = new HashMap();
		
		try { in = new RandomAccessFile( relFile, "r" ); }
		catch( Exception e ) {
			System.err.println( "Error opening relevance file!" );
			return results;
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
					judgments.put( TRECJudgments.getKey( queryID, docID ), null );
					Integer count = (Integer)results.totalRelevant.get( queryID );
					if( count == null )
						results.totalRelevant.put( queryID, new Integer( 1 ) );
					else
						count = new Integer( count.intValue() + 1 );
				}
			}
		}
		catch( Exception e ) {
			System.err.println( "Error reading relevance file!" );
			return results;
		}
		
		return results;
	}

}
