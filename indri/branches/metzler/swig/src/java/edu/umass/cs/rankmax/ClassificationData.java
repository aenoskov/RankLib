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
	
	public void addInstance( String classLabel, ArrayList indices, ArrayList values ) {
		SparseInstance inst = new SparseInstance( classLabel, indices, values );
		instances.add( inst );
		
		int numClasses = classes.size();
		if( classes.get( classLabel ) == null ) {
			classes.put( classLabel, new Integer( numClasses ) );
			classNames.add( classLabel );
		}
		
		if( inst.getMaxIndex() > numFeatures )
			numFeatures = inst.getMaxIndex();
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
