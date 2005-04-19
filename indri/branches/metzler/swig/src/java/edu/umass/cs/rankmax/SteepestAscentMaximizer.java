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
		double pointA = 0.0;
		double pointB = 0.0;
		
		// set up bracket
		// TODO: automatically find bracket
		if( onSimplex ) {
			pointA = 0.25*originalVal;
			pointB = 4.0*originalVal;
		}
		else {
			pointA = originalVal - BRACKET_WIDTH;
			pointB = originalVal + BRACKET_WIDTH;
		}
			
		param.setParam( coordinate, pointA );
		double fa = eval( param );
		param.setParam( coordinate, pointB );
		double fb = eval( param );
		param.setParam( coordinate, originalVal );

		if( fa >= curVal && fa > fb ) { // non-bracket
			param.setParam( coordinate, pointA );
			newVal = fa;
		}
		else if( fb >= curVal && fb > fa ) { // non-bracket
			param.setParam( coordinate, pointB );
			newVal = fb;
		}
		else { // bracket [ a, curVal, c ]
			//System.out.println( "originalVal: " + originalVal + ", coordinate: " + coordinate + ", a: " + fa + ", curVal:" + curVal + ", b: " + fb );
			newVal = goldenSectionSearch( coordinate, pointA, fa, originalVal, curVal, pointB, fb );
		}
		
		return newVal;
	}
		
	// golden section one dimension search
	protected double goldenSectionSearch( int coordinate,
										  double a, double fa,
										  double b, double fb,
										  double c, double fc ) {
		double val = 0.0;
		double d = 0.0;
		double R = ( 3.0 - Math.sqrt( 5.0 ) ) / 2.0;

		for( int iter = 0; iter < 50; iter++ ) {
			// get new point to evaluate
			if( c - b > b - a )
				d = b + R*( c - b );
			else
				d = b - R*( b - a );
		
			param.setParam( coordinate, d );
			double fd = eval( param );

			//System.out.println( "original bracket: [ " + a + "(" + fa + "), " + b + "(" + fb + "), " + c + "(" + fc + ") ]" );
		
			// TODO: simplify this logic once i make sure it's correct
			if( d > b ) {
				if( fb >= fd ) {
					c = d;
					fc = fd;
				}
				else {
					a = b;
					fa = fb;
					b = d;
					fb = fd;
				}
			}
			else {
				if( fb >= fd ) {
					a = d;
					fa = fd;
				}
				else {
					c = b;
					fc = fb;
					b = d;
					fb = fd;
				}
			}
		}			
		
		//System.out.println( "new bracket: [ " + a + "(" + fa + "), " + b + "(" + fb + "), " + c + "(" + fc + ") ]" );
		param.setParam( coordinate, b );
		
		return fb;
	}
}
