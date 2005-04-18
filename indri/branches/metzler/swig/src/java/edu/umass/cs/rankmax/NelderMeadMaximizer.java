/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.Arrays;

/**
 * @author Don Metzler
 *
 */
public class NelderMeadMaximizer extends Maximizer {

	protected double ALPHA = 1.0;
	protected double BETA = 1.0;
	protected double GAMMA = 0.5;
	protected double ETA = 0.5;
	
	protected int MIN_ITERS = 5;
	protected int MAX_ITERS = 100;
	protected double STOPPING_THRESHOLD = 0.01;
	
	protected int fxnEvaluations = 0;
	
	protected Vertex [] vertices = null;
	
	// construct a new Nelder-Mead maximizer using initial simplex defined by parameters a, b, c
	// this function works by minimizing the negative of the evaluation function (equivalent to maximizing)
	public NelderMeadMaximizer( Ranker r, Evaluator e, Parameters [] parameters ) {
		super( r, e );		
		
		vertices = new Vertex[ parameters.length ];
		
		// evaluate the function at each vertex of the simplex
		for( int i = 0; i < parameters.length; i++ ) {
			Parameters params = (Parameters)parameters[i]; 
			double val = eval( params );
			vertices[i] = new Vertex( params, val );
		}
	}
	
	// maximize function 
	public void maximize() {
		
		//while( Math.abs( oldScoreB - scoreB ) > 0.1 ) {
		for( int iter = 0; iter < MAX_ITERS; iter++ ) {
			if( iter >= MIN_ITERS && stop() ) { break; }

			// ORDER
			sortVertices();
			
			if( verbose ) {
				System.out.println( "ITERATION = " + iter );
				System.out.println( "BEST PARAMETER = " + getBestParam() );
				System.out.println( "BEST SCORE = " + getBestScore() );
			}

			Parameters paramMean = getParamMean();

			Parameters paramR = paramMean.add( getWorstParam(), 1.0 + ALPHA, -ALPHA );
			double scoreR = eval( paramR );
 
			// REFLECT
			if( getWorstScore() < scoreR && scoreR <= getBestScore() ) {
				vertices[ vertices.length - 1] = new Vertex( paramR, scoreR ) ;
				continue;
			}
			
			// EXPAND
			if( getBestScore() < scoreR ) {
				Parameters paramS = paramR.add( paramMean, 1.0 + BETA, -BETA );
				double scoreS = eval( paramS );
				if( getWorstScore() < scoreS ) {
					vertices[ vertices.length - 1] = new Vertex( paramS, scoreS ) ;
					continue;
				}
				else {
					vertices[ vertices.length - 1] = new Vertex( paramR, scoreR ) ;
					continue;					
				}
			}
			
			// CONTRACT
			if( scoreR < getNextWorstScore() ) {
				Parameters paramC = paramMean.add( getWorstParam(), 1.0 + GAMMA, -GAMMA );
				double scoreC = eval( paramC );
				
				if( getWorstScore() <= scoreC ) {
					vertices[ vertices.length - 1] = new Vertex( paramC, scoreC ) ;
					continue;					
				}
			}
			
			// SHRINK SIMPLEX
			for( int i = 1; i < vertices.length; i++ ) {
				Parameters paramTmp = getBestParam().add( vertices[i].params, 1.0 - ETA, ETA );
				double scoreTmp = eval( paramTmp );
				vertices[ i ] = new Vertex( paramTmp, scoreTmp ) ;
			}			
		}
		
		if( verbose )
			System.out.println("Total function evaluations = " + fxnEvaluations );
	}
	
	// computes the mean of the parameter values
	protected Parameters getParamMean() {
		Parameters mean = vertices[0].params; 
		
		for( int i = 1; i < vertices.length - 1; i++ ) 
			mean = mean.add( vertices[i].params, 1.0, 1.0 );
				
		return mean.scale( 1.0 / ( vertices.length - 1.0 ) );
	}

	// returns the parameter associated with the best function value
	protected Parameters getBestParam() {
		return vertices[ 0 ].params;
	}

	// returns the parameter associated with the worst function value
	protected Parameters getWorstParam() {
		return vertices[ vertices.length - 1].params;
	}

	// returns the best function value of the vertices
	protected double getBestScore() {
		return vertices[ 0 ].value;
	}


	// returns the worst function value of the vertices
	protected double getWorstScore() {
		return vertices[ vertices.length - 1 ].value;
	}

	// returns the worst function value of the vertices
	protected double getNextWorstScore() {
		return vertices[ vertices.length - 2 ].value;
	}
	
	// sorts the vertices of the simplex based on their function value
	protected void sortVertices() {
		Arrays.sort( vertices );

		/*System.out.println( "[NelderMeadMaximizer][sortParams()]" );
		System.out.println( "BEST PARAMETER = " + paramB );
		System.out.println( "BEST SCORE = " + scoreB );
		System.out.println( "GOOD PARAMETER = " + paramG );
		System.out.println( "GOOD SCORE = " + scoreG );
		System.out.println( "WORST PARAMETER = " + paramW );
		System.out.println( "WORST SCORE = " + scoreW );*/
	}

	protected boolean stop() {
		boolean toStop = false;
		
		// univariate case
		if( vertices.length == 2 )
			return false;
		
		double mean = 0.0;
		for( int i = 1; i < vertices.length; i++ )
			mean += vertices[i].value;

		mean /= 1.0*( vertices.length - 1.0 );
		
		double var = 0.0;
		for( int i = 1; i < vertices.length; i++ )
			var += ( vertices[i].value - mean )*( vertices[i].value - mean );
		
		var /= 1.0*vertices.length;
		
		if( Math.sqrt(var) < STOPPING_THRESHOLD )
			return true;
		
		return toStop;
	}
	
	public class Vertex implements Comparable {
		public Parameters params;
		public double value;
		
		public Vertex( Parameters params, double value ) {
			this.params = params;
			this.value = value;
		}
		
		public int compareTo( Object o ) {
			Vertex v = (Vertex)o;
			if( value < v.value )
				return 1;
			else if( value >= v.value )
				return -1;
			return 0;
		}
		
		public String toString() {
			return "[parameter = " + params + ", value = " + value + "]"; 
		}
	}
}
