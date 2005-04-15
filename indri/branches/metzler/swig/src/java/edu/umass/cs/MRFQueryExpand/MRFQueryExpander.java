/*
 * Created on Apr 15, 2005
 *
 */
package edu.umass.cs.MRFQueryExpand;

import java.util.HashMap;
import edu.umass.cs.indri.ParsedDocument;
import edu.umass.cs.indri.QueryEnvironment;
import edu.umass.cs.indri.ScoredExtentResult;

/**
 * @author Don Metzler
 *
 * Query expansion based on Markov Random Field model
 */
public class MRFQueryExpander {

	private static final String delims = " \b\t\n\f\r\'\"\\~`@#$%^&*()-=_+{}|[]:;,/<>.";
	private static final double LOW_SCORE = -100.0;
	private static final int CUTOFF = 5;
	
	public static void expand( String query, QueryEnvironment env, int fbTerms, int fbDocs ) {
		// run query and get results		
		ScoredExtentResult [] results = env.runQuery( query, fbDocs );
		int [] docSet = getDocSet( results );
		HashMap vocab = getVocabulary( results, env );
		String [] terms = getTerms( vocab );
		expand( terms, vocab, env, fbTerms, docSet );
	}
	
	private static void expand( String [] terms, HashMap vocab, QueryEnvironment env, int fbTerms, int [] docSet ) {
		int vocabSize = terms.length;
		int [] counter = new int[ fbTerms ];
		int numQueries = 0;
		boolean done = false;

		while( !done ) {
			String query = "";
			boolean skip = false;
			for( int i = 0; i < fbTerms; i++ ) {
				String term = terms[ counter [ i ] ];
				Integer count = (Integer)vocab.get( term );
				if( count == null || count.intValue() < CUTOFF )
					skip = true;
				query += term + " ";
			}
			if( !skip ) {
				QueryFormulator qf = new QueryFormulator( query, 1.0, 0.85, 0.10, 0.0, 0.05,
						                                  null,  0.0, 0.00, 0.00, 0.0, 0.00,
														  null,  0.0, 0.00, 0.00, 0.0, 0.00 );
				query = qf.formulateQuery( QueryFormulator.SEQUENTIAL_DEPENDENCE );
				double score = runAndScoreQuery( query, env, docSet );
				System.out.println( "QUERY = " + query + ", SCORE = " + score );
				numQueries++;
			}
			for( int i = 0; i < fbTerms; i++ ) {
				if( counter[i] + 1 < vocabSize ) {					
					counter[i]++;
					break;
				}
				else {
					if( i == fbTerms - 1 )
						done = true;
					counter[i] = 0;
				}
			}
		}
		
		System.out.println( "Total number of queries run = " + numQueries );
	}

	private static double runAndScoreQuery( String query, QueryEnvironment env, int [] docSet ) {
		double score = 0.0;
		ScoredExtentResult [] results = env.runQuery( query, docSet, docSet.length );
		for( int i = 0; i < results.length; i++ )
			score += results[i].score;
		
		if( results.length < docSet.length )
			score += ( docSet.length - results.length ) * ( LOW_SCORE );		
		
		return score;
	}
	
	private static HashMap getVocabulary( ScoredExtentResult [] results, QueryEnvironment env ) {
		ParsedDocument [] docs = env.documents( results );
		HashMap vocab = new HashMap();
		
		for( int i = 0; i < docs.length; i++ ) {
			String text = docs[i].text;
			for( int j = 0; j < docs[i].positions.length; j++ ) {
				ParsedDocument.TermExtent extent = docs[i].positions[j];
				String term = parse( text.substring( extent.begin, extent.end ) );
				if( term != null ) {
					Integer count = (Integer)vocab.get( term );
					if( count == null)
						vocab.put( term, new Integer(1) );
					else
						vocab.put( term, new Integer( count.intValue() + 1 ) );
				}
			}
		}
		
		return vocab;
	}

	private static int [] getDocSet( ScoredExtentResult [] results ) {
		int [] set = new int[ results.length ];
		for( int i = 0; i < results.length; i++ )
			set[ i ] = results[i].document;
		return set;
	}
	
	private static String [] getTerms( HashMap vocab ) {
		String [] terms = (String [])vocab.keySet().toArray( new String[] { } );
		//for( int i = 0; i < terms.length; i++ )
		//	System.out.println("i = " + i + ", term = " + terms[i] );
		return terms;
	}
	
	private static String parse( String s ) {
		for( int i = 0; i < delims.length(); i++ )
			s = s.replace( delims.charAt(i), ' ' );
		
		s = s.toLowerCase().trim();
		if( s.length() == 0 )
			s = null;
		
		return s;
	}
	
	public static void main( String args[] ) {
		QueryEnvironment indri = new QueryEnvironment();
		indri.addServer("indri2.cs.umass.edu:10000");
		String query = "white house";
		QueryFormulator qf = new QueryFormulator( query, 1.0, 0.85, 0.10, 0.0, 0.05,
												  null,  0.0, 0.00, 0.00, 0.0, 0.00,
												  null,  0.0, 0.00, 0.00, 0.0, 0.00 );
		MRFQueryExpander.expand( qf.formulateQuery( QueryFormulator.SEQUENTIAL_DEPENDENCE ), indri, 2, 5 );
		indri.close();
	}
	
}
