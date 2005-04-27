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

	public Parameters getDefaultStartParam() {
		int num = numFeatures;
		Parameters p = new Parameters( num, 1.0 );
		return p;
	}

	public Parameters getRandomStartParam() {
		int num = numFeatures;
		Parameters p = new Parameters( num, 0.0 );
		
		for( int i = 0; i < num; i++ )
			p.setParam( i, Math.random() );
		
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
				
		return results;
	}
}
