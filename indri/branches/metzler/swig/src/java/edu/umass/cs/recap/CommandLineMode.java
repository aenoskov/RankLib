/*
 * Created on Dec 8, 2004
 *
 */
package edu.umass.cs.recap;

import java.io.RandomAccessFile;
import java.util.StringTokenizer;
import java.util.Vector;

/**
 * @author Don Metzler
 *
 * Provides a command line interface to Recap
 */
public class CommandLineMode {

	private String query = null;
	private int queryID = 0;
	private String queryExtent = null;
	private String queryOp = null;
	private int numResults = 0;
	
	// command takes the form:
	// queryFile,queryID,extent,operator,combiner,numResults
	public CommandLineMode( String command ) throws Exception {
		StringTokenizer str = new StringTokenizer( command, ", " );
		String queryFile = null;
		try {			
			queryFile = str.nextToken();
			this.queryID = Integer.parseInt( str.nextToken() );
			this.queryExtent = str.nextToken();
			this.queryOp = str.nextToken();
			this.numResults = Integer.parseInt( str.nextToken() );
		}
		catch( Exception e ) {
			throw new Exception( "Error parsing command: " + command );
		}
		
		// read query from queryFile
		RandomAccessFile in = null;
		
		try { in = new RandomAccessFile( queryFile, "r" ); }
		catch( Exception e ) {
			throw new Exception("Unable to open file: " + queryFile );
		}

		query = "";
		try {
			String s = null;
			while( (s = in.readLine()) != null )
				query += s + "\n";
		}
		catch( Exception e ) {
			throw new Exception("Error while reading from query file!");
		}
	}
	
	// performs the specified command line commands
	// using the given retrieval engine
	public void run( RetrievalEngine retEngine ) {
		Vector results = retEngine.runQuery( query, queryOp, queryExtent, numResults );
		
		// display the results
		for( int i = 0; i < results.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			System.out.println( queryID + " Q0 " + info.docName + " " + ( i+1 ) + " " + info.score + " recap" );
		}
	}
}
