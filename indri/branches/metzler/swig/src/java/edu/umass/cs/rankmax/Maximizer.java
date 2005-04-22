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
	// this is pretty heuristic and not guaranteed to find a bracket even if one does
	// exist. however, it should find a majority of the 'well-behaved' brackets.
	protected Bracket bracket( Parameters direction, double initialStepSize, double minAlpha, double maxAlpha ) {		
		Bracket bracket = new Bracket( Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY,
									   0.0, 0.0,
									   Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY );
		double alpha, stepSize, lastVal;
		boolean increase, decrease;
		
		if( initialStepSize == 0.0 ) {
			System.err.println( "[Maximizer.bracket] initialStepSize set to 0 -- setting to Double.MIN_VALUE!" );
			initialStepSize = Double.MIN_VALUE;
		}
		
		// value of current parameter setting
		// in order to find a bracket we need to find two points a and b such that:
		// a < param < b AND f(a) < f(param) > f(b)
		double val = eval( param );
		bracket.fb = val;
		
		// search in the positive direction
		alpha = initialStepSize;
		stepSize = initialStepSize;
		increase = false;
		decrease = false;
		lastVal = val;
		while( alpha <= maxAlpha ) {
			double newVal = eval( param.add( direction, 1.0, alpha ) );
			
			if( newVal > lastVal ) { increase = true; }
			if( newVal < lastVal ) { decrease = true; }
			
			//System.out.println( "[RIGHT] alpha: " + alpha + ", lastVal: " + lastVal + ", newVal: " + newVal );
			
			if( !increase && decrease ) { // right end of bracket
				bracket.c = alpha;
				bracket.fc = newVal;
				break;
			}
			else if( increase && decrease ) { // entire bracket
				bracket.a = 0;
				bracket.fa = val;
				bracket.b = alpha - stepSize;
				bracket.fb = lastVal;
				bracket.c = alpha;
				bracket.fc = newVal;
				return bracket;
			}
			
			stepSize *= 2; // double the step size each time
			if( alpha != maxAlpha && alpha + stepSize > maxAlpha )
				alpha = maxAlpha;
			alpha += stepSize;
			lastVal = newVal;
		}
		if( !decrease ) {
			bracket.c = maxAlpha;
			bracket.fc = lastVal;
		}
		
		// search in the negative direction
		alpha = -initialStepSize;
		stepSize = initialStepSize;
		increase = false;
		decrease = false;
		lastVal = val;
		while( alpha >= minAlpha ) {
			double newVal = eval( param.add( direction, 1.0, alpha ) );
			
			if( newVal > lastVal ) { increase = true; }
			if( newVal < lastVal ) { decrease = true; }

			//System.out.println( "[LEFT] alpha: " + alpha + ", lastVal: " + lastVal + ", newVal: " + newVal );
			
			if( !increase && decrease ) { // left end of bracket
				bracket.a = alpha;
				bracket.fa = newVal;
				break;
			}
			else if( increase && decrease ) { // entire bracket
				bracket.a = alpha;
				bracket.fa = newVal;
				bracket.b = alpha - stepSize;
				bracket.fb = lastVal;
				bracket.c = 0.0;
				bracket.fc = val;
				return bracket;
			}
			
			stepSize *= 2; // double the step size each time
			if( alpha != minAlpha && alpha - stepSize < minAlpha )
				alpha = minAlpha;
			alpha -= stepSize;
			lastVal = newVal;
		}
		if( !decrease ) {
			bracket.a = minAlpha;
			bracket.fa = lastVal;
		}
		
		return bracket;
	}
	
	// performs the line search max_{\alpha} f(param + \alpha * direction)
	// implemented as golden section search based on implementation from Numerical Recipes in C
	// TODO: implement Brent's method?
	protected double lineSearch( Parameters direction, Bracket bracket ) {
		// points we keep track of
		Parameters x0 = null;
		Parameters x1 = null;
		Parameters x2 = null;
		Parameters x3 = null;
		
		// function evaluation values
		double f1 = 0.0;
		double f2 = 0.0;
		
		x0 = param.add( direction, 1.0, bracket.a ); // param + minAlpha
		x3 = param.add( direction, 1.0, bracket.c); // param + maxAlpha
		
		if( Math.abs( bracket.c - bracket.b ) > Math.abs( bracket.b - bracket.a ) ) {
			x1 = param;
			x2 = param.add( direction, 1.0, C*( bracket.c - bracket.b ) );
		}
		else {
			x2 = param;
			x1 = param.add( direction, 1.0, -C*( bracket.a - bracket.a ) );
		}
		
		f1 = eval( x1 );
		f2 = eval( x2 );

		//while( Math.abs( x3 - x0 ) > TOL*( Math.abs( x1 ) + Math.abs( x2 ) ) ) {
		// TODO: implement better stopping criteria
		for( int iter = 0; iter < 10; iter++ ) {
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
	}
	
	// computes a finite difference estimate of the derivative for each coordinate
	// and constructs an approximate gradient vector
	public Parameters getFiniteDifferenceGradient( double minAlpha, double maxAlpha ) {
		Parameters gradient = new Parameters( param.size(), 0.0 );
		Parameters direction = new Parameters( param.size(), 0.0 );
		Bracket bracket = null;
		
		double curVal = eval( param );
		
		// compute estimates of partial derivatives
		for( int coordinate = 0; coordinate < param.size(); coordinate++ ) {
			if( coordinate > 0 )
				direction.setParam( coordinate - 1, 0.0 );
			direction.setParam( coordinate, 1.0 );
			
			// try to find some variation in the coordinate using bracketing
			bracket = bracket( direction, 10E-32, minAlpha, maxAlpha );

			double partial = 0.0;
			if( bracket.fa > curVal )
				partial = ( bracket.fa - curVal ) / ( bracket.a - 0.0 );
			else if( bracket.fb > curVal )
				partial = ( bracket.fb - curVal ) / ( bracket.b - 0.0 );
			else if( bracket.fc > curVal )
				partial = ( bracket.fc - curVal ) / ( bracket.c - 0.0 );
			
			gradient.setParam( coordinate, partial );
		}
		
		return gradient;
	}
	
	public void setVerbose( boolean b ) {
		verbose = b;
	}
	
	public class Bracket {
		public double a, fa;
		public double b, fb;
		public double c, fc;
		
		public Bracket() {
			this.a = 0.0;
			this.fa = 0.0;
			this.b = 0.0;
			this.fb = 0.0;
			this.c = 0.0;
			this.fc = 0.0;			
		}
		
		public Bracket( double a, double fa, double b, double fb, double c, double fc ) {
			this.a = a;
			this.fa = a;
			this.b = b;
			this.fb = b;
			this.c = c;
			this.fc = c;
		}
		
		// returns whether or not this is actually a bracket or not
		public boolean isBracket() {
			if( a < b && b < c && fa < fb && fb > fc )
				return true;
			return false;
		}
		
		public String toString() {
			return "[ " + a + " (" + fa + "), " + b + " (" + fb + "), " + c + " (" + fc + ") ]";
		}
	}
}
