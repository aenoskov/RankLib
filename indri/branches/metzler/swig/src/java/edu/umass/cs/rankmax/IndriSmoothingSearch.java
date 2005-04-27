/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;

import edu.umass.cs.indri.QueryEnvironment;
import edu.umass.cs.indri.ScoredExtentResult;

/**
 * @author Don Metzler
 *
 */
public class IndriSmoothingSearch implements Ranker {

	protected QueryEnvironment indri = null;
	protected ArrayList queries = null;
	
	public IndriSmoothingSearch( String index, String queryFile ) {
		queries = RankMaxTools.getQueries( queryFile );
		
		indri = new QueryEnvironment();
		indri.addIndex( index );				
	}
	
	public Parameters getDefaultStartParam() {
		return new Parameters( new double [] { 2500.0 } );
	}
	
	public Parameters getRandomStartParam() {
		return new Parameters( new double [] { Math.random() * 5000.0 } );
	}
	
	public Ranking [] getRankings( Parameters p ) {
		double mu = p.getParam( 0 );
		String [] rules = new String [] { "method:dirichlet,mu:" + mu }; 
		indri.setScoringRules( rules );	
		
		Ranking [] results = new Ranking[ queries.size() ];
		
		for( int i = 0; i < queries.size(); i++ ) {
			String query = (String)queries.get( i );
			ScoredExtentResult [] r = indri.runQuery( query, 1000 );
			results[i] = new IndriRanking( (i+1)+"", r, getDocumentIDs( r ) );
		}
		
		return results;
	}

	protected String [] getDocumentIDs( ScoredExtentResult [] results ) {
		int [] ids = new int[ results.length ];
		String [] docNames = null;
		
		for( int i = 0; i < results.length; i++ )
			ids[ i ] = results[i].document;

		docNames = indri.documentMetadata( ids, "docno" );
		
		return docNames;
	}
}
