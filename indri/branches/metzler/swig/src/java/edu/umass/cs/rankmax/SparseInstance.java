/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;

/**
 * @author Don Metzler
 *
 * Sparse vector representation.
 */
public class SparseInstance extends Instance {

	protected int [] indices = null;
	protected double [] values = null;
	
	public SparseInstance( String classLabel, ArrayList indices, ArrayList values ) {
		super( classLabel );
		
		this.indices = new int[ indices.size() ];
		this.values = new double[ values.size() ];
		
		for( int i = 0; i < indices.size(); i++ ) {
			this.indices[ i ] = Integer.parseInt( (String)indices.get( i ) );
			this.values[ i ] = Double.parseDouble( (String)values.get( i ) );
		}
	}

	// do sparse dot product
	public double dotProd( Parameters p, int offset ) {
		double val = 0.0;
		for( int i = 0; i < indices.length; i++ )
			val += p.getParam( offset + indices[ i ] ) * values[ i ];
		return val;
	}

	// TODO: make this more efficient
	public int getMaxIndex() {
		int maxIndex = -1;
		for( int i = 0; i < indices.length; i++ ) {
			if( indices[ i ] > maxIndex )
				maxIndex = indices[ i ];
		}
		return maxIndex;
	}
}
