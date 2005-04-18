/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;

/**
 * @author metzler
 *
 */
public class ClassificationSearch implements Ranker {

	protected ClassificationData data = null;

	protected ArrayList instances = null;
	protected String [] classNames = null;
	protected int numClasses = 0;
	protected int numFeatures = 0;

	public ClassificationSearch( String instanceFile ) {
		this.data= RankMaxTools.readSparseInstances( instanceFile );
		this.instances = data.getInstances();
		this.classNames = data.getClassNames();
		this.numClasses = data.getNumClasses();
		this.numFeatures = data.getNumFeatures();
	}
	
	public Parameters getRandomStartPoint() {
		int num = numFeatures * numClasses;
		Parameters p = new Parameters( num, 0.0 );
		
		for( int i = 0; i < num; i++ )
			p.setParam( i, Math.random() );
		
		p.simplexNormalize();
		
		return p;
	}
	
	public Ranking[] getRankings( Parameters p ) {
		Ranking [] results = new Ranking[ instances.size() ];
		
		for( int i = 0; i < instances.size(); i++ ) {
			Instance inst = (Instance)instances.get( i );
			results[ i ] = new Ranking( inst.getInstanceID() );
			for( int classID = 0; classID < numClasses; classID++ ) {
				double score = inst.dotProd( p, classID*numFeatures );
				results[ i ].add( new Ranking.RankedItem( classNames[ classID ], score ) );
			}
		}
		
		return results;
	}

	public static void main( String [] args ) {
		ClassificationSearch fxn = new ClassificationSearch( args[0] );
		//Evaluator eval = new AveragePrecisionEvaluator( args[1] );
		Evaluator eval = new PrecisionAtNEvaluator( args[1], 1 );
		Parameters p0 = fxn.getRandomStartPoint();		
		Maximizer m = new SteepestAscentMaximizer( fxn, eval, p0, true );
		m.setVerbose( true );
		m.maximize();
	}

}
