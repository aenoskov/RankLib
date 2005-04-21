/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public class Parameters {

	protected double [] params = null;

	// new parameters ( with given size, all values set to fixed value )
	public Parameters( int size, double val ) {
		params = new double[size];
		
		for(int i = 0; i < size; i++ )
			params[i] = val;
	}
	
	// new parameters based on array of doubles
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
	
	// normalizes the parameters such that: they are all non-negative and they sum to 1
	public void simplexNormalize() {
		double sum = 0.0;
		double min = 0.0;
		for( int i = 0; i < params.length; i++ ) {
			sum += params[i];
			if( params[i] < min )
				min = params[i];
		}
		if( min >= 0 )
			min = 0;
		else {
			System.err.println("min: " + min + ", sum[before]: " + sum + ", sum[after]: " + (sum - min*params.length) );
			sum += -min*params.length;
		}
		// TODO: what if sum == 0.0 ?
		for( int i = 0; i < params.length; i++ )
			params[i] = ( params[i] - min ) / sum;
	}
	
	// returns parameter at coordinate i
	public double getParam( int i ) {
		if( i < 0 || i >= params.length ) { // TODO: throw Exception?
			System.out.println( "RETURNING NaN!" );
			System.exit(0);
			return Double.NaN;
		}
		return params[i];
	}

	// sets parameter at corordinate i
	public void setParam( int i, double val ) {
		if( i < 0 || i >= params.length ) // TODO: throw Exception?
			return;
		params[i] = val;
	}
	
	// returns the size of the parameter vector
	public int size() {
		return params.length;
	}
	
	public String toString() {
		String ret = "[ ";
		
		for( int i = 0; i < params.length; i++ )
			ret += "" + i + ": " + params[i] + " ";
		
		return ret + "]";
	}
}
