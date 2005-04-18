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

	// finite difference derivative width
	protected double WIDTH = 0.05;
	
	// current parameter setting
	protected Parameters param = null;
	
	// determines whether or not to keep the optimization constrainted to a simplex
	protected boolean onSimplex = false;
	
	public SteepestAscentMaximizer( Ranker r, Evaluator e, Parameters parameters, boolean onSimplex ) {
		super( r, e );
		this.param = parameters;
		this.onSimplex = onSimplex;
		
		approximateWidth();
	}
	
	public void maximize() {
		double curVal = eval( param );
		Parameters newParam = new Parameters( param.size(), 0.0 );
		Parameters tmpParam = new Parameters( param.size(), 0.0 );

		for( int iter = 0; iter < MAX_ITERS; iter++ ) {
			if( verbose ) {
				System.out.println( "ITERATION = " + iter );
				System.out.println( "CURRENT PARAMETER = " + param );
				System.out.println( "FXN VALUE = " + curVal );
			}
			
			double alpha = getStepSize( iter );
			
			// perform one optimization step
			for( int i = 0; i < param.size(); i++ ) {
				if( i == 0 )
					tmpParam.setParam( param.size() - 1, 0.0 );
				else
					tmpParam.setParam( i - 1, 0.0 );
				tmpParam.setParam( i, WIDTH ); // TODO: replace this with a line search
				double newVal = eval( param.add( tmpParam, 1.0, 1.0 ) );
				if( verbose )
					System.out.println( "DERIVATIVE AT COORDINATE " + i + " = " + ( newVal - curVal ) / WIDTH );
				newParam.setParam( i, param.getParam( i ) + alpha * ( newVal - curVal ) / WIDTH );
			}
			if( onSimplex )
				newParam.simplexNormalize();
			
			// update parameter setting
			param = newParam;
			curVal = eval( param );
		}
		
		if( verbose )
			System.out.println( "Total function evaluations = " + fxnEvaluations );
	}
	
	protected double getStepSize( int iter ) {
		return 1.0 / Math.sqrt( iter + 1 );
	}
	
	// approximates the appropriate value of 'h', the finite difference width
	// idea from: http://www.nezumi.demon.co.uk/consult/deriv.htm
	protected void approximateWidth() {
		double x = 0.0;
		for( int i = 0; i < param.size(); i++ )
			x += param.getParam( i );
		//WIDTH = Math.abs( x / param.size() ) * Math.sqrt( 10E-7 );
		WIDTH = Math.abs( x / param.size() ) * Math.sqrt( 10E-16 );
	}
	
}
