/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.StringTokenizer;

/**
 * @author Don Metzler
 *
 */
public class RankMaxTools {
		
	public static ArrayList getQueries( String queryFile ) {
		RandomAccessFile in = null;
		ArrayList queries = new ArrayList();
		
		try { in = new RandomAccessFile( queryFile, "r" );	}
		catch( Exception e ) {
			System.err.println( "Error opening query file!" );
			return queries;
		}
		
		try {
			String str = null;
			while( ( str = in.readLine() ) != null )
				queries.add( str );
		}
		catch( Exception e ) {
			System.err.println( "Error reading query file!" );
			return queries;
		}
		
		return queries;
	}

	public static ClassificationData readSparseInstances( String instanceFile ) {
		RandomAccessFile in = null;
		ClassificationData data = new ClassificationData();
		
		try { in = new RandomAccessFile( instanceFile, "r" );	}
		catch( Exception e ) {
			System.err.println( "Error opening instance file!" );
			return null;
		}
		
		try {
			String str = null;
			String instanceID = null;
			String classLabel = null;
			ArrayList indices = new ArrayList();
			ArrayList values = new ArrayList();
			while( ( str = in.readLine() ) != null ) {
				StringTokenizer tok = new StringTokenizer( str, " :" );
				instanceID = tok.nextToken();
				classLabel = tok.nextToken().toLowerCase();
				indices.clear();
				values.clear();
				while( tok.hasMoreTokens() ) {
					int idx = Integer.parseInt( tok.nextToken() );
					double val = Double.parseDouble( tok.nextToken() );
					if( idx > 20 )
						break;
					indices.add( idx+"" );
					values.add( val+"" );
					/*indices.add( tok.nextToken() );
					values.add( tok.nextToken() );*/					
				}
				data.addInstance( instanceID, classLabel, indices, values );
			}
		}
		catch( Exception e ) {
			e.printStackTrace();
			System.err.println( "Error reading instance file!" );
			return null;
		}
		
		return data;
	}
		
}
