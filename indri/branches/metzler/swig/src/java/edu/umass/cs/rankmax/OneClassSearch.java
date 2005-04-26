/*
 * Created on Apr 25, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * @author Don Metzler
 *
 */
public class OneClassSearch implements Ranker {

	protected ClassificationData data = null;

	protected ArrayList instances = null;
	protected int numFeatures = 0;
	protected int numInstanceIDs = 0;

	public OneClassSearch( String instanceFile ) {
		this.data= RankMaxTools.readSparseInstances( instanceFile );
		this.instances = data.getInstances();
		this.numFeatures = data.getNumFeatures();
		
		HashMap idLookup = new HashMap();
		
		// create rankings for each unique instance id
		for( int i = 0; i < instances.size(); i++ ) {
			Instance inst = (Instance)instances.get( i );
			String id = inst.getInstanceID();
			
			if( !idLookup.containsKey( id ) )
				idLookup.put( id, null );
		}
		
		this.numInstanceIDs = idLookup.size();
	}
	
	public Parameters getRandomStartPoint() {
		int num = numFeatures;
		Parameters p = new Parameters( num, 0.0 );
		
		for( int i = 0; i < num; i++ )
			p.setParam( i, Math.random() );
		
		p.simplexNormalize();
		
		return p;
	}
	
	public Ranking[] getRankings( Parameters p ) {
		Ranking [] results = new Ranking[ numInstanceIDs ];
		
		int lastID = 0;
		int offset = -1;
		for( int i = 0; i < instances.size(); i++ ) {
			Instance inst = (Instance)instances.get( i );
			int newID = Integer.parseInt( inst.getInstanceID() );
			if( lastID != newID || offset == -1 ) {
				results[ ++offset ] = new Ranking( inst.getInstanceID() );
				lastID = newID;
			}
			double score = inst.dotProd( p, 0 );
			results[ offset ].add( new Ranking.RankedItem( inst.getLabel(), score ) );
		}
		
		//System.out.println( p );
		//System.out.println( results[ 0 ] );
		
		return results;
	}

	public static void main( String [] args ) {
		OneClassSearch fxn = new OneClassSearch( args[0] );
		Evaluator eval = new AveragePrecisionEvaluator( args[1] );
		//Evaluator eval = new PrecisionAtNEvaluator( args[1], 1 );
		Parameters p0 = fxn.getRandomStartPoint();
		//Parameters p0 = new Parameters( fxn.numFeatures, 1.0 );
		p0.simplexNormalize();
		Maximizer m = new CoordinateAscentMaximizer( fxn, eval, p0, true );
		m.setVerbose( true );
		m.maximize();
	}

}
