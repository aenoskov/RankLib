/*
 * Created on Dec 8, 2004
 *
 */
package edu.umass.cs.recap;

import java.io.RandomAccessFile;
import java.util.StringTokenizer;

/**
 * @author Don Metzler
 *
 * Provides a command line interface to Recap
 */
public class CommandLineMode {

	private String query = null;
	private String queryExtent = null;
	private String queryOp = null;
	private String queryCombiner = null;
	private int numResults = 0;
	
	// command takes the form:
	// queryFile,extent,operator,combiner,numResults
	public CommandLineMode( String command ) throws Exception {
		StringTokenizer str = new StringTokenizer( command, ", " );
		String queryFile = null;
		try {
			queryFile = str.nextToken();
			this.queryExtent = str.nextToken();
			this.queryOp = str.nextToken();
			this.queryCombiner = str.nextToken();
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
		retEngine.runQuery( query, queryOp, queryExtent, queryCombiner, numResults );
	}
}
