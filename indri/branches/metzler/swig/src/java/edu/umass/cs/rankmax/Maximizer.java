/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author metzler
 *
 */
abstract public class Maximizer {

	// ranker and evaluator used for maximization
	protected Ranker r = null;
	protected Evaluator e = null;
	
	// default constructor
	public Maximizer( Ranker r, Evaluator e ) {
		this.r = r;
		this.e = e;
	}
	
	// maximizes output from ranker using evaluator
	abstract public void maximize();
	
}
