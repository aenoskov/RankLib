/*
 * Created on Dec 8, 2004
 *
 */
package edu.umass.cs.recap;

import java.awt.Color;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;

import edu.umass.cs.indri.DocumentVector;
import edu.umass.cs.indri.ParsedDocument;
import edu.umass.cs.indri.QueryEnvironment;
import edu.umass.cs.indri.ScoredExtentResult;

/**
 * @author Don Metzler
 *
 * Provides an interface to the underlying retrieval engine
 *
 */
public class RetrievalEngine {

	// score for an unmatched sentence
	private final double EPSILON = 1E-100;

	// Indri retrieval engine wrapper
	private QueryEnvironment indri = null;

	// create a new Indri Retrieval Engine 
	public RetrievalEngine( QueryEnvironment indri ) {
		this.indri = indri;
	}
	
	// runs a query with the given parameters
	public Vector runQuery( String query, String queryOp, String queryExtent,
							String queryCombiner, int numResults ) {
		Vector queries = null;
		Vector allScores = new Vector();		
	
		if( queryExtent.equals( "document" ) ) { // no extent restriction
			queryExtent = "";
		}
		else { // extent restriction
			queryExtent = "[" + queryExtent + "]";
		}
		
		if( queryCombiner.equals( "none" ) ) { // don't segment
			queries = new Vector();
			queries.add( parse( query ) );
		} 
		else { // segment by sentence
			queries = getQuerySentences( query );
		}
		
		// run a query for each sentence
		for( int queryNum = 0; queryNum < queries.size(); queryNum++ ) {
			String q = (String)queries.elementAt( queryNum );
			ScoredExtentResult [] scores = indri.runQuery( queryOp + queryExtent + "(" + q + ")", 1000 );
			allScores.add( scores );
		}

		Vector results = scoreResults( allScores, queryExtent, queryCombiner );
		
		Collections.sort( results );
		setMetadata( results );

		Vector viewableResults = new Vector();
		for( int i = results.size() - 1; i >= results.size() - numResults; i-- ) {
			if( i <= 0 ) // in case we don't actually retrieve as many docs as was requested
				break;
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			viewableResults.add( results.elementAt( i ) );
		}
		
		return viewableResults;		
	}
	
	// tokenizes query string
	private Vector getQuerySentences( String in ) {
		Vector ret = new Vector();
		SentenceTokenizer tok = new SentenceTokenizer( in );
		
		while( tok.hasMoreTokens() ) {
			String sentence = parse( tok.nextSentence() );
			//System.out.println( "SENTENCE = " + sentence );
			ret.add( sentence );
		}
		
		return ret;
	}
	
	private String parse( String in ) {
		String ret = new String();
	
		StringTokenizer tok = new StringTokenizer( in, 	" .?!\b\t\n\f\r\'\"\\~`@#$%^&*()-=_+{}|[]:;,/<>" );
		while( tok.hasMoreTokens() )
			ret += tok.nextToken() + " ";
		
		return ret;
	}

	// returns a vector of ScoredDocInfo objects
	private Vector scoreResults( Vector scores, String queryExtent, String queryCombiner ) {
		Vector ret = new Vector();

		HashMap candidateDocs = new HashMap();

		// create a hashmap to keep track of candidate documents
		for( int queryNum = 0; queryNum < scores.size(); queryNum++ ) {
			ScoredExtentResult [] queryScores = (ScoredExtentResult [])scores.elementAt( queryNum );
			for( int i = 0; i < queryScores.length; i++ ) {
				ScoredExtentResult r = (ScoredExtentResult)queryScores[i];
				Integer docNum = new Integer( r.document );
				candidateDocs.put( docNum, new ScoredDocInfo( docNum.intValue(), 0.0, new Vector() ) );
			}			
		}
				
		// score the documents
		for( int queryNum = 0; queryNum < scores.size(); queryNum++ ) {
			HashMap results = new HashMap();
			// collect results into a form we can use for scoring
			ScoredExtentResult [] queryScores = (ScoredExtentResult [])scores.elementAt( queryNum );
			for( int i = 0; i < queryScores.length; i++ ) {
				ScoredExtentResult r = (ScoredExtentResult)queryScores[i];
				//System.out.println( r.score );
				Integer docNum = new Integer( r.document );
				Vector v = (Vector)results.get( docNum );
				if( v == null )
					v = new Vector();
				v.add( r );
				results.put( docNum, v );
			}
		
			// update each document's score
			Iterator iter = candidateDocs.keySet().iterator();
			while( iter.hasNext() ) {
				Integer i = (Integer)iter.next();
				double score = 0.0;
				if( queryCombiner.equals( "prob" ) ) 
					score = getDocScoreProb( i , results, queryExtent );
				else if( queryCombiner.equals( "none") ) {
					Vector v = (Vector)results.get( i );
					if( v.size() > 1 )
						System.err.println( "WARNING: dropping some scores!" );
					score = Math.exp( ( (ScoredExtentResult)v.elementAt( 0 ) ).score );
				}
				else {
					System.err.println("Unrecognized query combiner: " + queryCombiner );
				}
				ScoredDocInfo info = (ScoredDocInfo)candidateDocs.get( i );
				Vector v = (Vector)results.get( i );
				info.score += Math.log( score );
				if( v != null && v.size() > 0 )
					info.extents.addAll( v ); // add all matches
					//info.extents.add( v.elementAt( 0 ) ); // only add the best match
				
			}
		}

		// construct the final ranked list
		Iterator iter = candidateDocs.keySet().iterator();
		while( iter.hasNext() ) {
			Integer i = (Integer)iter.next();
			ScoredDocInfo info = (ScoredDocInfo)candidateDocs.get( i );
			ret.add( info );
		}
		
		return ret;
	}
	
	// scores a single document by probabilistically combining
	// scores
	private double getDocScoreProb( Integer i, HashMap results, String queryExtent ) {
		double score = 0.0;

		// get the document vectors for each candidate
		int [] docIDs = new int[ 1 ];
		docIDs[ 0 ] = i.intValue();
		DocumentVector [] docs = indri.documentVectors( docIDs );
		
		Vector v = (Vector)results.get( i );
		if( v == null || v.size() == 0 )
			score = EPSILON;
		else {
			for( int j = 0; j < v.size(); j++ ) {
				ScoredExtentResult s = (ScoredExtentResult)v.elementAt( j );
				score += Math.exp( 1.0*s.score ) / 1.0 * extentCount( queryExtent, docs[0] );
			}
		}
		
		return score;
	}

	// counts the number of extents of type queryExtent in doc
	private int extentCount( String queryExtent, DocumentVector doc ) {
		int num = 0;
		
		if( queryExtent.equals("") ) // no extent => entire document is the single extent
			return 1;
		
		DocumentVector.Field [] f = doc.fields;
		for( int i = 0; i < f.length; i++ ) {
			if( queryExtent.equals( "["+f[i].name+"]" ) )
				num++;
		}
		
		return num;
	}
	
	// adds document metadata to the ScoredDocumentInfo
	// objects contained in the docs Vector
	private void setMetadata( Vector docs ) {
		int [] ids = new int[docs.size()];
		
		for( int i = 0; i < docs.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)docs.elementAt( i );
			ids[i] = info.docID;
		}
		
		String [] docNames = indri.documentMetadata( ids, "docno" );
		
		for( int i = 0; i < docs.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)docs.elementAt( i );
			info.docName = docNames[ i ];
			// TODO: is there a better way to do this??
			if( docNames[i].startsWith("WSJ") ) {
				info.date = Integer.parseInt(docNames[i].substring(7,9));
				info.month = Integer.parseInt(docNames[i].substring(5,7));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(3,5));
			}
			else if( docNames[i].startsWith("LA") ) {
				info.date = Integer.parseInt(docNames[i].substring(2,4));
				info.month = Integer.parseInt(docNames[i].substring(4,6));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(6,8));				
			}
			else if( docNames[i].startsWith("AP") ) {
				info.date = Integer.parseInt(docNames[i].substring(6,8));
				info.month = Integer.parseInt(docNames[i].substring(4,6));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(2,4));
			}
			// TODO: fix this to handle FBIS, FT, and SJMN
			else {
				info.date = 1;
				info.month = 1;
				info.year = 1989;
			}
			//System.out.println(info);
		}
	}
	
	public DefaultStyledDocument getDocument( DocInfo info ) {
		DefaultStyledDocument doc = new DefaultStyledDocument();
		
		// TODO: clean this up a bit
		int [] id = new int[1];
		id[0] = info.getDocNum();
		ParsedDocument [] docs = indri.documents( id );
		try {
			doc.insertString( 0, docs[0].text , null );
		}
		catch( Exception e ) { /* do nothing */ }
				
		return doc;
	}
	
	public RecapStyledDocument getMarkedDocument( ScoredDocInfo info ) {
		RecapStyledDocument doc = new RecapStyledDocument();
		
		// TODO: clean this up a bit
		int [] id = new int[1];
		id[0] = info.docID;
		ParsedDocument [] docs = indri.documents( id );
		try {
			doc.insertString( 0, docs[0].text , null );
		}
		catch( Exception e ) { /* do nothing */ }

		Style defaultStyle = StyleContext.getDefaultStyleContext().getStyle(StyleContext.DEFAULT_STYLE);
		Style s = doc.addStyle( "match", defaultStyle );
		StyleConstants.setForeground( s, Color.blue );
		StyleConstants.setItalic( s, true );
        
		for( int i = 0; i < info.extents.size(); i++ ) {
			ScoredExtentResult extent = (ScoredExtentResult)info.extents.elementAt( i );
			int extentBegin = docs[0].positions[extent.begin].begin;
			int extentEnd = docs[0].positions[extent.end - 1].end;

			doc.addMatch( extentBegin, extentEnd );
			
			try {
				doc.replace( extentBegin, extentEnd - extentBegin,
					     	 doc.getText(extentBegin, extentEnd-extentBegin),
							 doc.getStyle("match") );
			}
			catch(Exception e) { /* do nothing */ }
		}
		
		doc.setByteLength( docs[0].text.length() );
		
		return doc;
	}
	
	// returns a list of indexed fields
	public String [] getFieldList() {
		return indri.fieldList();
	}
	
	// returns a list of all the document IDs
	// for the documents indexed within the given
	// query environment
	public Vector getDocIDs() {
		Vector ret = new Vector();

		// TODO: make this work for collections
		//       with numDocs > MAX_INT
		int numDocs = (int)indri.documentCount();
		
		int [] ids = new int[ numDocs ];
		for( int i = 0; i < numDocs; i++ )
			ids[i] = i+1;
		
		String [] docIDs = indri.documentMetadata( ids, "docno" );
		
		for( int i = 0; i < numDocs; i++ )
			ret.add( new DocInfo( docIDs[i], i+1 ) );
		
		return ret;
	}
}
