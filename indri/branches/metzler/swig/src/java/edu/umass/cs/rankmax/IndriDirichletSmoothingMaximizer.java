/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.io.RandomAccessFile;
import java.util.ArrayList;

import edu.umass.cs.indri.QueryEnvironment;
import edu.umass.cs.indri.ScoredExtentResult;

/**
 * @author metzler
 *
 */
public class IndriDirichletSmoothingMaximizer implements Ranker {

	protected QueryEnvironment indri = null;
	protected ArrayList queries = null;
	
	public IndriDirichletSmoothingMaximizer( String index, String queryFile ) {
		queries = getQueries( queryFile );
		
		indri = new QueryEnvironment();
		indri.addIndex( index );				
	}
	
	protected ArrayList getQueries(String queryFile) {
		RandomAccessFile in = null;
		ArrayList queries = new ArrayList();
		
		try { in = new RandomAccessFile( queryFile, "r" );	}
		catch( Exception e ) {
			System.err.println( "Error opening query file!" );
			return queries;
		}
		
		try {
			String str = null;
			while( ( str = in.readLine() ) != null )
				queries.add( str );
		}
		catch( Exception e ) {
			System.err.println( "Error reading query file!" );
			return queries;
		}
		
		return queries;
	}

	public Ranking [] getRankings( Parameters p ) {
		double mu = p.getParam( 0 );
		String [] rules = new String [] { "method:dirichlet,mu:" + mu }; 
		indri.setScoringRules( rules );	
		
		Ranking [] results = new Ranking[ queries.size() ];
		
		for( int i = 0; i < queries.size(); i++ ) {
			String query = (String)queries.get( i );
			ScoredExtentResult [] r = indri.runQuery( query, 1000 );
			results[i] = new IndriRanking( i+1, r, getDocumentIDs( r ) );
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

	public static void main( String [] args ) {
		IndriDirichletSmoothingMaximizer fxn = new IndriDirichletSmoothingMaximizer( args[0], args[1] );
		Evaluator eval = new AveragePrecisionEvaluator( args[2] );
		
		Parameters lower = new Parameters( new double [] { 0 } );
		Parameters upper = new Parameters( new double [] { 4000 } );
		Parameters [] params = new Parameters [] { lower, upper };
		NelderMeadMaximizer nm = new NelderMeadMaximizer( fxn, eval, params );
		nm.maximize();
	}
}
