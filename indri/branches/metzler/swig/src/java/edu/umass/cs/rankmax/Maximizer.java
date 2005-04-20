/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
abstract public class Maximizer {

	// constants used in the golden section search
	static final double C = ( 3.0 - Math.sqrt( 5.0 ) ) / 2.0;
	static final double R = 1 - C;
	static final double TOL = 1E-4;
	
	// ranker and evaluator used for maximization
	protected Ranker r = null;
	protected Evaluator e = null;

	// current parameter value
	protected Parameters param = null;

	// number of function evaluations
	protected int fxnEvaluations = 0;
	
	// verbose output
	protected boolean verbose = false;
	
	// default constructor
	public Maximizer( Ranker r, Evaluator e ) {
		this.r = r;
		this.e = e;		
		this.fxnEvaluations = 0;
		this.verbose = false;
	}	
	
	// maximizes output from ranker using evaluator
	abstract public void maximize();
	
	// evaluates the Evaluator at the given parameter setting
	protected double eval( Parameters p ) {
		fxnEvaluations++;
		return e.evaluate( r.getRankings( p ) );
	}

	// brackets a maximum around the current parameter setting in the given direction
	// returns null if no bracket could be found
	// TODO: implement this
	protected double [] bracket( Parameters direction, double initialStepSize, double minAlpha, double maxAlpha ) {		
		double [] bracket = new double[2];
		double alpha, stepSize;
		
		// value of current parameter setting
		// in order to find a bracket we need to find two points a and b such that:
		// a < param < b AND f(a) < f(param) > f(b)
		double val = eval( param );
		
		// search in the positive direction
		alpha = initialStepSize;
		stepSize = initialStepSize;
		while( alpha <= maxAlpha ) {
			double fb = eval( param.add( direction, 1.0, stepSize ) );
			stepSize *= 2; // double the step size each time
			if( alpha != maxAlpha && alpha + stepSize > maxAlpha )
				alpha = maxAlpha;
			alpha += stepSize;
		}
		
		// search in the negative direction
		alpha = -initialStepSize;
		stepSize = initialStepSize;
		while( alpha >= minAlpha ) {
			double fa = eval( param.add( direction, 1.0, stepSize ) );
			stepSize *= 2; // double the step size each time
			if( alpha != minAlpha && alpha - stepSize < minAlpha )
				alpha = maxAlpha;
			alpha -= stepSize;
		}
		
		return bracket;
	}
	
	// performs the line search max_{\alpha} f(param + \alpha * direction)
	// implemented as golden section search based on implementation from Numerical Recipes in C
	// TODO: implement Brent's method?
	protected double lineSearch( Parameters direction, double minAlpha, double maxAlpha ) {
		// points we keep track of
		Parameters x0 = null;
		Parameters x1 = null;
		Parameters x2 = null;
		Parameters x3 = null;
		
		// function evaluation values
		double f1 = 0.0;
		double f2 = 0.0;
		
		x0 = param.add( direction, 1.0, minAlpha ); // param + minAlpha
		x3 = param.add( direction, 1.0, maxAlpha ); // param + maxAlpha
		
		if( Math.abs( maxAlpha - 0 ) > Math.abs( 0 - minAlpha ) ) {
			x1 = param;
			x2 = param.add( direction, 1.0, C*( maxAlpha - 0 ) );
		}
		else {
			x2 = param;
			x1 = param.add( direction, 1.0, -C*( 0 - minAlpha ) );
		}
		
		f1 = eval( x1 );
		f2 = eval( x2 );

		while( Math.abs( x3 - x0 ) > TOL*( Math.abs( x1 ) + Math.abs( x2 ) ) ) {
			if( f2 > f1 ) {
				x0 = x1;
				x1 = x2;
				x2 = x1.add( x3, R, C );
				f1 = f2;
				f2 = eval( x2 );
			}
			else {
				x3 = x2;
				x2 = x1;
				x1 = x2.add( x0, R, C );
				f2 = f1;
				f1 = eval( x1 );
			}
		}
		
		if( f1 > f2 ) {
			param = x1;
			return f1;
		}
		else {
			param = x2;			
			return f2;
		}
		
		return 0.0;
	}
	
	public void setVerbose( boolean b ) {
		verbose = b;
	}
}
