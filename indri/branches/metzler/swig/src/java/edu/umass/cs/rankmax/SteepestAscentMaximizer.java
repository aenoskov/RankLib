/*
 * Created on Apr 19, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public class SteepestAscentMaximizer extends Maximizer {

	public SteepestAscentMaximizer( Ranker r, Evaluator e, Parameters parameters ) {
		super( r, e, parameters );
		this.param = parameters;
	}
	
	public void maximize() {
		double curVal = eval( param );
		Parameters gradient = null;
		Bracket bracket = null;

		for( int iter = 0; iter < maxNumIters; iter++ ) {
			verbosePrint( "[SteepestAscentMaximizer.maximize] ITERATION = " + iter );
			//verbosePrint( "[SteepestAscentMaximizer.maximize] CURRENT PARAMETER = " + param );
			verbosePrint( "[SteepestAscentMaximizer.maximize] FXN VALUE = " + curVal );

			// find direction of steepest ascent
			gradient = getFiniteDifferenceGradient( -25.0, 25.0 );
			verbosePrint( "[SteepestAscentMaximizer.maximize] gradient: " + gradient );
			
			// try to bracket a maxima in the direction of the gradient
			bracket = bracket( gradient, 10E-16, -25.0, 25.0 );		
			verbosePrint( "bracket: " + bracket );
			
			// take a step in an increasing direction
			if( bracket.isBracket() )
				curVal = lineSearch( gradient, bracket ); // do a line search to find optimal step size
			else {
				if( bracket.fc > curVal ) {
					param = param.add( gradient, 1.0, bracket.c );
					curVal = bracket.fc;
				}
				else if( bracket.fb > curVal ) {
					param = param.add( gradient, 1.0, bracket.b );
					curVal = bracket.fb;
				}
				else if( bracket.fa > curVal ) {
					param = param.add( gradient, 1.0, bracket.a );
					curVal = bracket.fa;
				}
			}
			if( onSimplex )
				param.simplexNormalize();
		}
		
		verbosePrint( "[SteepestAscentMaximizer.maximize] Total function evaluations = " + fxnEvaluations );
	}	
}
