/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author metzler
 *
 */
public interface Evaluator {

	// evaluates a set of rankings (higher score => better ranking)
	public double evaluate( Ranking [] results );
	
}
