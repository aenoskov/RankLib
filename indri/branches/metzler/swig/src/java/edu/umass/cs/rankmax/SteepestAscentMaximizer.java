/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public class SteepestAscentMaximizer extends Maximizer {

	// maximum number of iterations
	protected int MAX_ITERS = 100;

	// width of line search bracket
	protected double BRACKET_WIDTH = 1.0;
	
	// current parameter setting
	protected Parameters param = null;
	
	// determines whether or not to keep the optimization constrainted to a simplex
	protected boolean onSimplex = false;
	
	public SteepestAscentMaximizer( Ranker r, Evaluator e, Parameters parameters, boolean onSimplex ) {
		super( r, e );
		this.param = parameters;
		this.onSimplex = onSimplex;
	}
	
	public void maximize() {
		double curVal = eval( param );

		for( int iter = 0; iter < MAX_ITERS; iter++ ) {
			if( verbose ) {
				System.out.println( "ITERATION = " + iter );
				//System.out.println( "CURRENT PARAMETER = " + param );
				System.out.println( "FXN VALUE = " + curVal );
			}

			// perform one optimization step
			for( int i = 0; i < param.size(); i++ ) {
				curVal = lineSearch( curVal, i );			
				if( onSimplex )
					param.simplexNormalize();
			}
		}
		
		if( verbose )
			System.out.println( "Total function evaluations = " + fxnEvaluations );
	}
	
	protected double lineSearch( double curVal, int coordinate ) {
		double newVal = curVal;
		
		double originalVal = param.getParam( coordinate );
		
		param.setParam( coordinate, originalVal - BRACKET_WIDTH );
		double a = eval( param );
		param.setParam( coordinate, originalVal + BRACKET_WIDTH );
		double b = eval( param );
		param.setParam( coordinate, originalVal );

		if( a > curVal && a > b ) { // non-bracket
			param.setParam( coordinate, originalVal - BRACKET_WIDTH );
			newVal = a;
		}
		else if( b > curVal && b > a ) { // non-bracket
			param.setParam( coordinate, originalVal + BRACKET_WIDTH );
			newVal = b;
		}
		else { // bracket [ a, curVal, c ]
			System.out.println( "coordinate: " + coordinate + ", a: " + a + ", curVal:" + curVal + ", b: " + b );
		}
		
		return newVal;
	}
		
}
