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

	// ranker and evaluator used for maximization
	protected Ranker r = null;
	protected Evaluator e = null;

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

	public void setVerbose( boolean b ) {
		verbose = b;
	}
}
