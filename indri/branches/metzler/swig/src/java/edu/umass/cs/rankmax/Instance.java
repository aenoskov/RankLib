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
	
	protected String instanceID = null;
	protected String classLabel = null;
	
	public Instance( String instanceID, String classLabel ) {
		this.instanceID = instanceID;
		this.classLabel = classLabel;
	}
	
	public abstract double dotProd( Parameters p, int offset );
	
	public String getInstanceID() {
		return instanceID;
	}
	
	public String getLabel() {
		return classLabel;
	}
}
