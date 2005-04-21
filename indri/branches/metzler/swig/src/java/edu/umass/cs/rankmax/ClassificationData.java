/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * @author Don Metzler
 *
 */
public class ClassificationData {

	protected ArrayList instances = null;
	protected ArrayList classNames = null;
	protected HashMap classes = null;
	protected int numFeatures = 0;
	
	public ClassificationData() {
		this.instances = new ArrayList();
		this.classNames = new ArrayList();
		this.classes = new HashMap();
		this.numFeatures = 0;
	}
	
	public void addInstance( String instanceID, String classLabel, ArrayList indices, ArrayList values ) {
		//SparseInstance inst = new SparseInstance( instanceID, classLabel, indices, values );
		SparseInstance inst = new SparseInstance( instanceID, classLabel, indices, values, 0.0 );
		instances.add( inst );
		
		int numClasses = classes.size();
		if( classes.get( classLabel ) == null ) {
			classes.put( classLabel, new Integer( numClasses ) );
			classNames.add( classLabel );
		}
		
		int count = inst.getMaxIndex() + 1;
		if( count > numFeatures )
			numFeatures = count;
	}
	
	public ArrayList getInstances() {
		return instances;
	}
	
	public String [] getClassNames() {
		return (String [])classNames.toArray( new String [] { } );
	}
	
	public int getNumClasses() {
		return classes.size();
	}
	
	public int getNumFeatures() {
		return numFeatures;
	}
	
}
