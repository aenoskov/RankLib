/*
 * Created on Feb 15, 2005
 *
 */
package edu.umass.cs.recap;

import java.util.HashMap;
import java.util.Map;

import edu.umass.cs.indri.ParsedDocument;

/**
 * @author Don Metzler
 *
 * Simple LRU cache.
 */

public class RecapCache extends HashMap {

	private static final int CACHE_SIZE = 1000;
	
	public void put( int [] ids, String [] names, String [] dates, ParsedDocument [] docs ) {
		for( int i = 0; i < ids.length; i++ )
			put( new Integer( ids[i] ), new Entry( names[i], dates[i], docs[i] ) );
	}
	
	protected boolean removeEldestEntry( Map.Entry eldest ) {
		return size() > CACHE_SIZE;
	}
	
	public class Entry {		
		public String name = null;
		public String date = null;
		public ParsedDocument doc = null;
		
		public Entry( String name, String date, ParsedDocument doc ) {
			this.name = name;
			this.date = date;
			this.doc = doc;
		}
	}
}
