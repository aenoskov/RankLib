/*
 * Created on Apr 5, 2005
 *
 */
package edu.umass.cs.recap;

import java.util.ArrayList;

/**
 * @author Don Metzler
 *
 * Wraps the following information for "analyze" queries:
 *     - text of sentences
 *     - positions of sentences within RecapStyledDocument
 */
public class AnalyzeQuery {

	protected ArrayList queries = null;
	protected ArrayList positions = null;
	
	public AnalyzeQuery( String text, int begin, int end ) {		
		queries= new ArrayList();
		positions = new ArrayList();
		
		SentenceTokenizer tok = new SentenceTokenizer( text );
		
		// add each sentence and its position
		while( tok.hasMoreTokens() ) {
			String sent = tok.nextSentence();
			int length = tok.getUnparsedSentenceLength();
			System.out.println( "SENTENCE = {" + sent + "}, length = " + length );
			queries.add( sent );
			positions.add( new Match( begin, begin+length ) );
			begin = begin + length;
		}
	}
	
	public ArrayList getQueries() {
		return queries;
	}
	
	public ArrayList getPositions() {
		return positions;
	}
}
