/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author metzler
 *
 */
abstract public class ScoreFunction {

	// returns the score for the given instance (higher score => better score)
	abstract public double score( Instance inst );  
	
}
