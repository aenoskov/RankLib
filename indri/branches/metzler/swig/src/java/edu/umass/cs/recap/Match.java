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
	public double type;

	// match with no type
	public Match( int begin, int end ) {
		this.begin = begin;
		this.end = end;
		this.type = 0;
	}
	
	// match with a specified type
	public Match( int begin, int end, double type ) {
		this.begin = begin;
		this.end = end;
		this.type = type;
	}

	// compare two Match objects using begin, end, and type info
	public int compareTo( Object o ) {
		Match m = (Match)o;
		if ( begin == m.begin ) {
			if( end < m.end )
				return -1;
			else if( end > m.end )
				return 1;
			else if( end == m.end && type < m.type )
				return -1;
			else if( end == m.end && type > m.type )
				return 1;
			return 0;
		}
		else if( begin < m.begin )
			return -1;
		return 1;
	}

	// compare two Match objects using only begin and end info and not type info
	public int extentCompareTo( Match m ) {		
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

	public String toString() {
		return "[begin="+begin+",end="+end+",type="+type+"]";
	}

}
