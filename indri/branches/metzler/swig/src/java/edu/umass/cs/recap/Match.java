package edu.umass.cs.recap;
/*
 * Created on Nov 7, 2004
 */

/**
 * @author Don Metzler
 */
public class Match implements Comparable {

	public int begin;
	public int end;
	
	public Match( int begin, int end ) {
		this.begin = begin;
		this.end = end;
	}

	public int compareTo( Object o ) {
		Match m = (Match)o;
		if ( begin == m.begin ) {
			if( end < m.end )
				return -1;
			else if( end > m.end )
				return 1;
			return 0;
		}
		else if( begin < m.begin )
			return -1;
		return 1;
	}
}
