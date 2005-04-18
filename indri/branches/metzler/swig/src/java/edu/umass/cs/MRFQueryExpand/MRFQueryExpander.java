/*
 * Created on Apr 15, 2005
 *
 */
package edu.umass.cs.MRFQueryExpand;

import java.util.ArrayList;
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
	private static final int CUTOFF = 15;
	
	private static final String [] stopwords = { "a","about","above","according","across","after","afterwards","again","against","albeit","all","almost","alone","along","already","also","although","always","am","among","amongst","an","and","another","any","anybody","anyhow","anyone","anything","anyway","anywhere","apart","are","around","as","at","av","be","became","because","become","becomes","becoming","been","before","beforehand","behind","being","below","beside","besides","between","beyond","both","but","by","can","cannot","canst","certain","cf","choose","contrariwise","cos","could","cu","day","do","does","doesn't","doing","dost","doth","double","down","dual","during","each","either","else","elsewhere","enough","et","etc","even","ever","every","everybody","everyone","everything","everywhere","except","excepted","excepting","exception","exclude","excluding","exclusive","far","farther","farthest","few","ff","first","for","formerly","forth","forward","from","front","further","furthermore","furthest","get","go","had","halves","hardly","has","hast","hath","have","he","hence","henceforth","her","here","hereabouts","hereafter","hereby","herein","hereto","hereupon","hers","herself","him","himself","hindmost","his","hither","hitherto","how","however","howsoever","i","ie","if","in","inasmuch","inc","include","included","including","indeed","indoors","inside","insomuch","instead","into","inward","inwards","is","it","its","itself","just","kind","kg","km","last","latter","latterly","less","lest","let","like","little","ltd","many","may","maybe","me","meantime","meanwhile","might","moreover","most","mostly","more","mr","mrs","ms","much","must","my","myself","namely","need","neither","never","nevertheless","next","no","nobody","none","nonetheless","noone","nope","nor","not","nothing","notwithstanding","now","nowadays","nowhere","of","off","often","ok","on","once","one","only","onto","or","other","others","otherwise","ought","our","ours","ourselves","out","outside","over","own","per","perhaps","plenty","provide","quite","rather","really","round","said","sake","same","sang","save","saw","see","seeing","seem","seemed","seeming","seems","seen","seldom","selves","sent","several","shalt","she","should","shown","sideways","since","slept","slew","slung","slunk","smote","so","some","somebody","somehow","someone","something","sometime","sometimes","somewhat","somewhere","spake","spat","spoke","spoken","sprang","sprung","stave","staves","still","such","supposing","than","that","the","thee","their","them","themselves","then","thence","thenceforth","there","thereabout","thereabouts","thereafter","thereby","therefore","therein","thereof","thereon","thereto","thereupon","these","they","this","those","thou","though","thrice","through","throughout","thru","thus","thy","thyself","till","to","together","too","toward","towards","ugh","unable","under","underneath","unless","unlike","until","up","upon","upward","upwards","us","use","used","using","very","via","vs","want","was","we","week","well","were","what","whatever","whatsoever","when","whence","whenever","whensoever","where","whereabouts","whereafter","whereas","whereat","whereby","wherefore","wherefrom","wherein","whereinto","whereof","whereon","wheresoever","whereto","whereunto","whereupon","wherever","wherewith","whether","whew","which","whichever","whichsoever","while","whilst","whither","who","whoa","whoever","whole","whom","whomever","whomsoever","whose","whosoever","why","will","wilt","with","within","without","worse","worst","would","wow","ye","yet","year","yippee","you","your","yours","yourself","yourselves" };
	
	public static void expand( String query, QueryFormulator formulator, QueryEnvironment env, int fbTerms, int fbDocs ) {
		// run query and get results
		formulator.setQuery( query, null, null );
		ScoredExtentResult [] results = env.runQuery( formulator.formulateQuery(), fbDocs );
		int [] docSet = getDocSet( results );
		HashMap vocab = getVocabulary( results, env );
		String [] terms = getTerms( vocab );
		expand( formulator, terms, vocab, env, fbTerms, docSet );
	}
	
	private static void expand( QueryFormulator formulator, String [] terms, HashMap vocab, QueryEnvironment env, int fbTerms, int [] docSet ) {
		int vocabSize = terms.length;
		int [] counter = new int[ fbTerms ];
		boolean done = false;
		
		int numQueries = 0;
		while( !done ) {
			String query = "";
			for( int i = 0; i < fbTerms; i++ )
				query += terms[ counter[ i ] ] + " ";
			formulator.setQuery( query, null, null );
			String expandedQuery = formulator.formulateQuery();
			double score = runAndScoreQuery( expandedQuery, env, docSet );
			System.out.println( query + "," + expandedQuery + "," + score );
			numQueries++;

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
				
		// remove stopwords
		for( int i = 0; i < stopwords.length; i++ )
			vocab.remove( stopwords[i] );
				
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
		ArrayList trimmedTerms = new ArrayList();
		for( int i = 0; i < terms.length; i++ ) {
			Integer count = (Integer)vocab.get( terms[i] );
			if( count.intValue() >= CUTOFF )
				trimmedTerms.add( terms[i] );
		}
		terms = (String [])trimmedTerms.toArray( new String[] { } );
		//for( int i = 0; i < terms.length; i++ )
		//	System.out.println("i = " + i + ", term = " + terms[i] );
		return terms;
	}
	
	private static String parse( String s ) {
		for( int i = 0; i < delims.length(); i++ )
			s = s.replace( delims.charAt(i), ' ' );
		
		String newS = "";
		for( int i = 0; i < s.length(); i++ ) {
			if( s.charAt( i ) != ' ' )
				newS += s.charAt( i );
		}
		
		s = newS.toLowerCase().trim();
		if( s.length() == 0 )
			s = null;
		
		return s;
	}
	
	public static void main( String args[] ) {
		QueryEnvironment indri = new QueryEnvironment();
		indri.addServer("indri2.cs.umass.edu:10000");
		String query = "iraq weapons mass destruction";
		QueryFormulator qf = new QueryFormulator( query, 1.0, 0.85, 0.10, 0.0, 0.05,
												  null,  0.0, 0.00, 0.00, 0.0, 0.00,
												  null,  0.0, 0.00, 0.00, 0.0, 0.00 );
		qf.setType( QueryFormulator.SEQUENTIAL_DEPENDENCE );
		//qf.setType( QueryFormulator.FULL_DEPENDENCE );
		MRFQueryExpander.expand( query, qf, indri, 3, 10 );
		indri.close();
	}
	
}
