/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author metzler
 *
 */
public class Parameters {

	protected double [] params = null;
	
	public Parameters( double [] p ) {
		params = p;
	}
	
	// returns k1*this + k2*b
	public Parameters add( Parameters b, double k1, double k2 ) {
		double [] newParams = new double[ params.length ];

		for( int i = 0; i < params.length; i++ )
			newParams[i] = k1*params[i] + k2*b.getParam( i );
		
		return new Parameters( newParams );
	}

	// returns k*this
	public Parameters scale( double k ) {
		double [] newParams = new double[ params.length ];

		for( int i = 0; i < params.length; i++ )
			newParams[i] = k*params[i];
		
		return new Parameters( newParams );		
	}
	
	public double getParam( int i ) {
		return params[i];
	}
	
	public String toString() {
		String ret = "[ ";
		
		for( int i = 0; i < params.length; i++ )
			ret += "" + i + ": " + params[i] + " ";
		
		return ret + "]";
	}
}
