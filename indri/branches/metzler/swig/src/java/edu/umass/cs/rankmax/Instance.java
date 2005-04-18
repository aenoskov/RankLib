/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public abstract class Instance {
	
	protected String classLabel = null;
	public String getLabel() { return classLabel; }
	
	public Instance( String classLabel ) {
		this.classLabel = classLabel;
	}
	
	public abstract double dotProd( Parameters p, int offset );
	
}
