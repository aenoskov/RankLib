/*
 * Created on Dec 8, 2004
 *
 */
package edu.umass.cs.recap;

import java.awt.Color;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;

import edu.umass.cs.indri.DocumentVector;
import edu.umass.cs.indri.ParsedDocument;
import edu.umass.cs.indri.QueryAnnotation;
import edu.umass.cs.indri.QueryAnnotationNode;
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

	// Cached metadata and ParsedDocuments
	private RecapCache cache = null;
	
	// StyledDocument's font size
	private int docFontSize;
	
	// create a new Indri Retrieval Engine 
	public RetrievalEngine( QueryEnvironment indri ) {
		this.indri = indri;
		this.cache = new RecapCache();
		this.docFontSize = 12;
	}
	
	// runs a query with the given parameters
	public Pair runQuery( String query, int numResults ) {
		Vector allScores = new Vector();		
					
		System.err.println( "Running query: " + query );
		
		QueryAnnotation annotation = indri.runAnnotatedQuery( query, numResults );
		ScoredExtentResult [] results = annotation.getResults();
		
		Vector viewableResults = new Vector();
		for( int i = 0; i < results.length; i++ ) {
			ScoredDocInfo info = new ScoredDocInfo( results[i].document, results[i].score, 
					                                results[i].begin, results[i].end, new Vector() );
			viewableResults.add( info );
		}
		
		setMetadata( viewableResults );
		
		return new Pair( annotation, viewableResults );		
	}
	
	// runs a query with the given parameters
	public Vector runQuery( String query, String queryOp, String queryExtent, int numResults ) {
		Vector queries = null;
		Vector allScores = new Vector();
		
		if( queryExtent.equals( "document" ) ) { // no extent restriction
			queryExtent = "";
		}
		else { // extent restriction
			queryExtent = "[" + queryExtent + "]";
		}
		
		queries = getQuerySentences( query );
		
		//long startTime1 = System.currentTimeMillis();
		String initialQuery = "";
		for( int queryNum = 0; queryNum < queries.size(); queryNum++ )
			initialQuery += (String)queries.elementAt( queryNum );
		initialQuery = "#combine( " + initialQuery + " )";
		
		ScoredExtentResult [] score = indri.runQuery( initialQuery, 1000 );
		int [] ids = new int[ score.length ];
		for( int i = 0; i < score.length; i++ )
			ids[ i ] = score[i].document;
		
		// run a query for each sentence
		for( int queryNum = 0; queryNum < queries.size(); queryNum++ ) {
			String q = (String)queries.elementAt( queryNum );
			System.err.println( "[" + (queryNum+1) + "/" + queries.size() + "] Running query: " + queryOp + queryExtent + "(" + q + ")" );
			//ScoredExtentResult [] scores = indri.runQuery( queryOp + queryExtent + "(" + q + ")", 1000 );
			ScoredExtentResult [] scores = indri.runQuery( queryOp + queryExtent + "(" + q + ")", ids, 1000 );
			allScores.add( scores );
		}
		//long endTime1 = System.currentTimeMillis();

		/*allScores.clear();
		long startTime2 = System.currentTimeMillis();
		// run a query for each sentence
		for( int queryNum = 0; queryNum < queries.size(); queryNum++ ) {
			String q = (String)queries.elementAt( queryNum );
			System.err.println( "[" + (queryNum+1) + "/" + queries.size() + "] Running query: " + queryOp + queryExtent + "(" + q + ")" );
			ScoredExtentResult [] scores = indri.runQuery( queryOp + queryExtent + "(" + q + ")", 1000 );
			allScores.add( scores );
		}
		long endTime2 = System.currentTimeMillis();
		
		System.out.println("Method 1 time = " + ( endTime1 - startTime1 )/1000.0 + " seconds.");
		System.out.println("Method 2 time = " + ( endTime2 - startTime2 )/1000.0 + " seconds."); */
		
		System.err.println( "Combining scores...");
		Vector results = scoreResults( allScores, queryExtent );
		
		Collections.sort( results );

		Vector viewableResults = new Vector();
		for( int i = results.size() - 1; i >= results.size() - numResults; i-- ) {
			if( i <= 0 ) // in case we don't actually retrieve as many docs as was requested
				break;
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			viewableResults.add( results.elementAt( i ) );
		}

		setMetadata( viewableResults );
		
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
	private Vector scoreResults( Vector scores, String queryExtent ) {
		Vector ret = new Vector();

		HashMap candidateDocs = new HashMap();

		// create a hashmap to keep track of candidate documents
		for( int queryNum = 0; queryNum < scores.size(); queryNum++ ) {
			ScoredExtentResult [] queryScores = (ScoredExtentResult [])scores.elementAt( queryNum );
			for( int i = 0; i < queryScores.length; i++ ) {
				ScoredExtentResult r = (ScoredExtentResult)queryScores[i];
				Integer docNum = new Integer( r.document );
				candidateDocs.put( docNum, new ScoredDocInfo( docNum.intValue(), 0.0, 0, 0, new Vector() ) );
			}			
		}

		// TODO: make this work for fields other than
		// "sentence"
		String [] metadata = null;
		// get the document vectors for each candidate
		int [] docIDs = new int[ candidateDocs.size() ];
		Iterator iter = candidateDocs.keySet().iterator();
		int num = 0;
		while( iter.hasNext() ) {
			Integer i = (Integer)iter.next();
			docIDs[ num++ ] = i.intValue();
		}
		metadata = indri.documentMetadata( docIDs, "numsentences" );

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
			iter = candidateDocs.keySet().iterator();
			num = 0;
			while( iter.hasNext() ) {
				Integer i = (Integer)iter.next();
				double score = 0.0;
				score = getDocScoreProb( i , results, metadata[num++] );
				ScoredDocInfo info = (ScoredDocInfo)candidateDocs.get( i );
				Vector v = (Vector)results.get( i );
				info.score += Math.log( score );
				if( v != null && v.size() > 0 )
					//info.extents.addAll( v ); // add all matches
					info.extents.add( v.elementAt( 0 ) ); // only add the best match
				
			}
		}

		// construct the final ranked list
		iter = candidateDocs.keySet().iterator();
		while( iter.hasNext() ) {
			Integer i = (Integer)iter.next();
			ScoredDocInfo info = (ScoredDocInfo)candidateDocs.get( i );
			ret.add( info );
		}
		
		return ret;
	}
	
	// scores a single document by probabilistically combining
	// scores
	private double getDocScoreProb( Integer i, HashMap results, String metadata ) {
		double score = EPSILON;
		
		int numExtents = 1;
		if( metadata != null && !metadata.equals("") )
			numExtents = Integer.parseInt( metadata );
		
		Vector v = (Vector)results.get( i );
		if( v == null || v.size() == 0 )
			score = EPSILON;
		else {
			for( int j = 0; j < v.size(); j++ ) {
				ScoredExtentResult s = (ScoredExtentResult)v.elementAt( j );
				double tmpScore = Math.exp( 1.0*s.score ); // / ( 1.0 * numExtents );
				if( tmpScore > score )
					score = tmpScore;
				//score += Math.exp( 1.0*s.score ) / ( 1.0 * numExtents );
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
		
		// arrays to fill in with data
		String [] docNames = new String[ docs.size() ]; //indri.documentMetadata( ids, "docno" );
		String [] docDates = new String[ docs.size() ]; //indri.documentMetadata( ids, "date" );
		ParsedDocument [] parsedDocs = new ParsedDocument[ docs.size() ]; //indri.documents( ids );
		
		getMetadata( ids, docNames, docDates, parsedDocs );
				
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
				info.date = Integer.parseInt(docNames[i].substring(4,6));
				info.month = Integer.parseInt(docNames[i].substring(2,4));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(6,8));				
			}
			else if( docNames[i].startsWith("AP") ) {
				info.date = Integer.parseInt(docNames[i].substring(6,8));
				info.month = Integer.parseInt(docNames[i].substring(4,6));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(2,4));
			}
			else if( docNames[i].startsWith("FT") ) {
				info.date = Integer.parseInt(docDates[i].trim().substring(4,6));
				info.month = Integer.parseInt(docDates[i].trim().substring(2,4));
				info.year = 1900 + Integer.parseInt(docDates[i].trim().substring(0,2));				
			}
			else if( docNames[i].startsWith("SJMN") ) {				
				info.date = Integer.parseInt(docDates[i].trim().substring(4,6));
				info.month = Integer.parseInt(docDates[i].trim().substring(2,4));
				info.year = 1900 + Integer.parseInt(docDates[i].trim().substring(0,2));				
			}
			// For all other sources the default date is set to January 1st, 1989.
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

		Style defaultStyle = StyleContext.getDefaultStyleContext().getStyle(StyleContext.DEFAULT_STYLE);
		StyleConstants.setFontSize( defaultStyle, docFontSize );

		ParsedDocument theDoc = getParsedDocument( info.getDocNum() );
		
		try { doc.insertString( 0, theDoc.text , defaultStyle ); }
		catch( Exception e ) { /* do nothing */ }
				
		return doc;
	}

	// returns a RecapStyledDocument for a given ScoredDocInfo record
	public DefaultStyledDocument getAnnotatedDocument( int docID, QueryAnnotation annotation ) {
		DefaultStyledDocument doc = new DefaultStyledDocument();
		
		Style defaultStyle = StyleContext.getDefaultStyleContext().getStyle(StyleContext.DEFAULT_STYLE);
		StyleConstants.setFontSize( defaultStyle, docFontSize );
		
		initializeHighlighting( doc, defaultStyle );
		
		ParsedDocument theDoc = getParsedDocument( docID );
        
		// insert document text into RecapStyledDocument
		try { doc.insertString( 0, theDoc.text , defaultStyle ); }
		catch( Exception e ) { /* do nothing */ }

		QueryAnnotationNode root = annotation.getQueryTree();
		Map matches = annotation.getAnnotations();
		
		annotationHelper( docID, doc, theDoc, root, matches, 0 );
				
		return doc;
	}

	private int annotationHelper( int docID, DefaultStyledDocument doc,
								   ParsedDocument theDoc, QueryAnnotationNode root,
								   Map matches, int highlightNum ) {
		// process root
		if( root.type.equals( "RawScorerNode" ) ) {
			ScoredExtentResult [] extents = (ScoredExtentResult [])matches.get( root.name );
			if( extents != null && extents.length > 0 ) {
				for( int i = 0; i < extents.length; i++ ) {
					if( extents[i].document != docID )
						continue;
					int extentBegin = theDoc.positions[extents[i].begin].begin;
					int extentEnd = theDoc.positions[extents[i].end - 1].end;
					
					try {
						doc.replace( extentBegin, extentEnd - extentBegin,
							     	 doc.getText(extentBegin, extentEnd-extentBegin),
									 doc.getStyle("highlight"+highlightNum) );						
					}
					catch(Exception e) {  }					
				}
				highlightNum = ( highlightNum + 1 ) % 5;
			}			
		}
		//System.out.println( root.type + " " + root.name + " "  + root.queryText );
		
		// process children
		QueryAnnotationNode [] children = root.children;
		if( children == null || children.length == 0 )
			return highlightNum;
		else {
			for(int i = 0; i < children.length; i++ )
				highlightNum = annotationHelper( docID, doc, theDoc, children[i], matches, highlightNum );
		}
		return highlightNum;
	}

	
//	 returns a RecapStyledDocument for a given ScoredDocInfo record
	public RecapStyledDocument getMarkedDocument( ScoredDocInfo info ) {
		RecapStyledDocument doc = new RecapStyledDocument();
		
		Style defaultStyle = StyleContext.getDefaultStyleContext().getStyle(StyleContext.DEFAULT_STYLE);
		StyleConstants.setFontSize( defaultStyle, docFontSize );
		
		Style s = doc.addStyle( "match", defaultStyle );
		StyleConstants.setForeground( s, Color.blue );
		StyleConstants.setItalic( s, true );
				
		ParsedDocument theDoc = getParsedDocument( info.docID );
        
		// insert document text into RecapStyledDocument
		try { doc.insertString( 0, theDoc.text , defaultStyle ); }
		catch( Exception e ) { /* do nothing */ }

		for( int i = 0; i < info.extents.size(); i++ ) {
			ScoredExtentResult extent = (ScoredExtentResult)info.extents.elementAt( i );
			int extentBegin = theDoc.positions[extent.begin].begin;
			int extentEnd = theDoc.positions[extent.end - 1].end;

			doc.addMatch( extentBegin, extentEnd );
			
			try {
				doc.replace( extentBegin, extentEnd - extentBegin,
					     	 doc.getText(extentBegin, extentEnd-extentBegin),
							 doc.getStyle("match") );
			}
			catch(Exception e) { /* do nothing */ }
		}
		
		doc.setByteLength( theDoc.text.length() );
		
		return doc;
	}
	
	public ParsedDocument getParsedDocument( int docID ) {
		ParsedDocument theDoc = null;		
		RecapCache.Entry entry = (RecapCache.Entry)cache.get( new Integer( docID ) );
		if( entry == null ) { // need to get the ParsedDocument
			int [] id = new int[1];
			id[0] = docID;
			ParsedDocument [] docs = indri.documents( id );
			theDoc = docs[0];
		}
		else // got the ParsedDocument from the cache
			theDoc = entry.doc;
		return theDoc;
	}

	private void getMetadata( int [] ids, String [] docNames, String [] docDates, ParsedDocument [] parsedDocs ) {
		boolean [] found = new boolean[ ids.length ];
		int [] lookup = new int[ ids.length ];
		int numMissing = 0;
		for( int i = 0; i < ids.length; i++ ) {
			RecapCache.Entry entry = (RecapCache.Entry)cache.get( new Integer( ids[i] ) );
			if( entry == null ) { // need to look up this value 
				found[i] = false;
				lookup[ numMissing++ ] = i;
			}
			else { // found in cache, so fill it in
				found[i] = true;
				docNames[i] = entry.name;
				docDates[i] = entry.date;
				parsedDocs[i] = entry.doc;
			}
		}
				
		if( numMissing == 0 ) // we're done
			return;
		
		int [] missingIDs = new int[ numMissing ];
		
		numMissing = 0;
		for( int i = 0; i < ids.length; i++ ) { 
			if( !found[i] )
				missingIDs[ numMissing++ ] = ids[ i ];
		}
		
		String [] tmpNames = indri.documentMetadata( missingIDs, "docno" );
		String [] tmpDates = indri.documentMetadata( missingIDs, "date" );
		ParsedDocument [] tmpDocs = indri.documents( missingIDs );
		
		// fill in the missing values
		for( int i = 0; i < numMissing; i++ ) {
			docNames[ lookup[i] ] = tmpNames[i];
			docDates[ lookup[i] ] = tmpDates[i];
			parsedDocs[ lookup[i] ] = tmpDocs[i];
		}
		
		// add new information to the cache
		cache.put( missingIDs, tmpNames, tmpDates, tmpDocs );
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
	
	public void increaseDocFontSize() {
		docFontSize += 2;
	}
	
	public void decreaseDocFontSize() {
		docFontSize -= 2;
	}
	
	private void initializeHighlighting( DefaultStyledDocument doc, Style defaultStyle ) {
		String [] highlightNames = new String [] { "highlight0", "highlight1", "highlight2",
			                           			   "highlight3", "highlight4" };
		Color [] colorNames = new Color [] { Color.GREEN, Color.CYAN, Color.RED,
				                             Color.MAGENTA, Color.ORANGE };  
		
		for(int i = 0; i < highlightNames.length; i++ ) {
			Style s = doc.addStyle( highlightNames[i], defaultStyle );
			StyleConstants.setBackground( s, colorNames[i] );
			StyleConstants.setBold( s, true );
		}		
	}
}
