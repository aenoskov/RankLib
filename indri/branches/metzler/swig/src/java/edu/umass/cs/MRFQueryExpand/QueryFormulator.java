/*
 * Created on Apr 15, 2005
 *
 */
package edu.umass.cs.MRFQueryExpand;

/**
 * @author Don Metzler
 *
 * Formulates several different kinds of Indri queries based on title, description, and
 * narrative fields of TREC topics.
 *
 */
public class QueryFormulator {

	// types of query formulations
	static final int FULL_INDEPENDENCE = 1;
	static final int SEQUENTIAL_DEPENDENCE = 2;
	static final int FULL_DEPENDENCE = 3;
	
	// text from different parts of a TREC topic
	protected String titleText = null;
	protected String descriptionText = null;
	protected String narrativeText = null;
	
	// weights of different TREC topic fields
	protected double titleFieldWeight = 0.0;
	protected Weighting titleWeights = null;
	
	protected double descriptionFieldWeight = 0.0;
	protected Weighting descriptionWeights = null;
	
	protected double narrativeFieldWeight = 0.0;
	protected Weighting narrativeWeights = null;

	// type of formulation
	protected int type = FULL_INDEPENDENCE;
	
	public QueryFormulator( String titleText ) {
		this( titleText, null, null );
	}
	
	public QueryFormulator( String titleText, String descriptionText, String narrativeText ) {
		this( titleText, 1.0, descriptionText, 1.0, narrativeText, 1.0 );
	}
	
	public QueryFormulator( String titleText, double titleFieldWeight,
			                String descriptionText, double descriptionFieldWeight,
							String narrativeText, double narrativeFieldWeight ) {		
		this( titleText, titleFieldWeight, 1.0, 1.0, 1.0, 1.0,
			  descriptionText, descriptionFieldWeight, 1.0, 1.0, 1.0, 1.0,
			  narrativeText, narrativeFieldWeight, 1.0, 1.0, 1.0, 1.0 );
	}

	public QueryFormulator( String titleText, double titleFieldWeight, double titleTermWeight, double titleContigOrderedWeight, double titleNoncontigOrderedWeight, double titleUnorderedWeight, 
							String descriptionText, double descriptionFieldWeight, double descriptionTermWeight, double descriptionContigOrderedWeight, double descriptionNoncontigOrderedWeight, double descriptionUnorderedWeight,	
							String narrativeText, double narrativeFieldWeight, double narrativeTermWeight, double narrativeContigOrderedWeight, double narrativeNoncontigOrderedWeight, double narrativeUnorderedWeight ) {
		this.titleText = titleText;
		this.titleFieldWeight = titleFieldWeight;
		this.titleWeights = new Weighting( titleTermWeight, titleContigOrderedWeight, titleNoncontigOrderedWeight, titleUnorderedWeight );

		this.descriptionText = descriptionText;
		this.descriptionFieldWeight = descriptionFieldWeight;
		this.descriptionWeights = new Weighting( descriptionTermWeight, descriptionContigOrderedWeight, descriptionNoncontigOrderedWeight, descriptionUnorderedWeight );
		
		this.narrativeText = narrativeText;
		this.narrativeFieldWeight = narrativeFieldWeight;
		this.narrativeWeights = new Weighting( narrativeTermWeight, narrativeContigOrderedWeight, narrativeNoncontigOrderedWeight, narrativeUnorderedWeight );
		
		this.type = FULL_INDEPENDENCE;
	}

	public String formulateQuery() {
		String titleQuery = null;
		String descriptionQuery = null;
		String narrativeQuery = null;
		
		if( type == FULL_INDEPENDENCE ) {
			titleQuery = getFullIndependenceQuery( titleText );
			descriptionQuery = getFullIndependenceQuery( descriptionText );
			narrativeQuery = getFullIndependenceQuery( narrativeText );
		}
		else if( type == SEQUENTIAL_DEPENDENCE ) {
			titleQuery = getSequentialDependenceQuery( titleText, titleWeights );
			descriptionQuery = getSequentialDependenceQuery( descriptionText, descriptionWeights );
			narrativeQuery = getSequentialDependenceQuery( narrativeText, narrativeWeights );
		}
		else if( type == FULL_DEPENDENCE ) {
			titleQuery = getFullDependenceQuery( titleText, titleWeights );
			descriptionQuery = getFullDependenceQuery( descriptionText, descriptionWeights );
			narrativeQuery = getFullDependenceQuery( narrativeText, narrativeWeights );
		}
		else {
			System.err.println( "Unrecognized query formulation type!" );
			return null;
		}
		
		if( titleQuery != null && descriptionQuery == null && narrativeQuery == null )
			return titleQuery;
		else if( titleQuery == null && descriptionQuery != null && narrativeQuery == null ) 
			return descriptionQuery;
		else if( titleQuery == null && descriptionQuery == null && narrativeQuery != null )
			return narrativeQuery;
		else if( titleQuery != null && descriptionQuery != null && narrativeQuery == null )
			return "#weight( " + titleFieldWeight + " " + titleQuery + " " + descriptionFieldWeight + " " + descriptionQuery + " )";
		else if( titleQuery != null && descriptionQuery == null && narrativeQuery != null ) 
			return "#weight( " + titleFieldWeight + " " + titleQuery + " " + narrativeFieldWeight + " " + narrativeQuery + " )";
		else if( titleQuery == null && descriptionQuery != null && narrativeQuery != null )
			return "#weight( " + descriptionFieldWeight + " " + descriptionQuery + " " + narrativeFieldWeight + " " + narrativeQuery + " )";
		else if( titleQuery != null && descriptionQuery != null && narrativeQuery != null )
			return "#weight( " + titleFieldWeight + " " + titleQuery + " " + descriptionFieldWeight + " " + descriptionQuery + " " + narrativeFieldWeight + " " + narrativeQuery + " )";
		
		return null;
	}
	
	public void setQuery( String titleText, String descriptionText, String narrativeText ) {
		this.titleText = titleText;
		this.descriptionText = descriptionText;
		this.narrativeText = narrativeText;
	}

	public void setType( int type ) {
		this.type = type;
	}
	
	private String getFullIndependenceQuery( String text ) {
		if( text == null )
			return null;
		return "#combine( " + text + " )";
	}
	
	private String getSequentialDependenceQuery( String text, Weighting w ) {
		if( text == null )
			return null;
		
		String [] tokens = text.split(" ");
		
		if( tokens.length == 0 )
			return null;
		else if( tokens.length == 1 )
			return "#combine( " + tokens[0] + " )";
		
		String termQuery = tokens[0];
		String orderedQuery = "";
		String unorderedQuery = "";
		
		String lastTerm = tokens[0];
		for( int i = 1; i < tokens.length; i++ ) {
			String curTerm = tokens[i];
			termQuery += " " + curTerm;
			orderedQuery += "#1( " + lastTerm + " " + curTerm + " ) ";
			unorderedQuery += "#uw8( " + lastTerm + " " + curTerm + " ) ";
			lastTerm = curTerm;
		}		

		termQuery = "#combine( " + termQuery + " )";
		orderedQuery = "#combine( " + orderedQuery + ")";
		unorderedQuery = "#combine( " + unorderedQuery + ")";
		
		if( w.termWeight != 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight != 0.0 )
			return "#weight( " + w.termWeight + " " + termQuery + " " + w.contiguousOrderedWeight + " " + orderedQuery + " " + w.unorderedWeight + " " + unorderedQuery + " )"; 
		else if( w.termWeight != 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight == 0.0 )
			return "#weight( " + w.termWeight + " " + termQuery + " " + w.contiguousOrderedWeight + " " + orderedQuery + " )"; 
		else if( w.termWeight != 0.0 && w.contiguousOrderedWeight == 0.0 && w.unorderedWeight != 0.0 )
			return "#weight( " + w.termWeight + " " + termQuery + " " + w.unorderedWeight + " " + unorderedQuery + " )"; 
		else if( w.termWeight == 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight != 0.0 )
			return "#weight( " + w.contiguousOrderedWeight + " " + orderedQuery + " " + w.unorderedWeight + " " + unorderedQuery + " )"; 
		else if( w.termWeight != 0.0 && w.contiguousOrderedWeight == 0.0 && w.unorderedWeight == 0.0 )
			return termQuery; 
		else if( w.termWeight == 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight == 0.0 )
			return orderedQuery; 
		else if( w.termWeight == 0.0 && w.contiguousOrderedWeight == 0.0 && w.unorderedWeight != 0.0 )
			return unorderedQuery; 
		
		return null;
	}
	
	private String getFullDependenceQuery( String text, Weighting w ) {
		if( text == null )
			return null;

		String [] tokens = text.split(" ");
		
		if( tokens.length == 0 )
			return null;
		else if( tokens.length == 1 )
			return "#combine( " + tokens[0] + " )";
		
		String termQuery = "";
		String orderedQuery = "";
		String unorderedQuery = "";		
		
		for( int i = 1; i <  Math.pow(2, tokens.length); i++ ) {
			String binary = Integer.toBinaryString( i );
			int padding = tokens.length - binary.length();
			for( int j = 0; j < padding; j++ )
				binary = "0" + binary;
						
			boolean singleTerm = false;
			boolean contiguous = true;
			
			int firstOne = binary.indexOf( '1' );
			int lastOne = binary.lastIndexOf( '1' );			
			if( lastOne == firstOne )
				singleTerm = true;
			
			for( int j = binary.indexOf( '1' ) + 1; j <= binary.lastIndexOf( '1' ) - 1; j++ ) {
				if( binary.charAt( j ) == '0' ) {
					contiguous = false;
					break;
				}
			}
			
			if( singleTerm )
				termQuery += tokens[ firstOne ] + " ";
			else if( !singleTerm && contiguous ) {
				orderedQuery += "#1( ";
				unorderedQuery += "#uw" + ( lastOne - firstOne + 1 )*4 + "( ";
				for( int j = firstOne; j <= lastOne; j++ ) {
					orderedQuery += tokens[ j ] + " ";
					unorderedQuery += tokens[ j ] + " ";
				}
				orderedQuery += ") ";
				unorderedQuery += ") ";
			}
			else { // !singleTerm && !contiguous
				String tmp = "";
				int numTokens = 0;
				for( int j = 0; j < binary.length(); j++ ) {
					if( binary.charAt( j ) == '1' ) {
						numTokens++;
						tmp += tokens[ j ] + " ";
					}
				}
				unorderedQuery += "#uw" + (numTokens*4) + "( " + tmp + ") "; 
			}			
		}
		
		termQuery = "#combine( " + termQuery + ")";
		orderedQuery = "#combine( " + orderedQuery + ")";
		unorderedQuery = "#combine( " + unorderedQuery + ")";
				
		if( w.termWeight != 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight != 0.0 )
			return "#weight( " + w.termWeight + " " + termQuery + " " + w.contiguousOrderedWeight + " " + orderedQuery + " " + w.unorderedWeight + " " + unorderedQuery + " )"; 
		else if( w.termWeight != 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight == 0.0 )
			return "#weight( " + w.termWeight + " " + termQuery + " " + w.contiguousOrderedWeight + " " + orderedQuery + " )"; 
		else if( w.termWeight != 0.0 && w.contiguousOrderedWeight == 0.0 && w.unorderedWeight != 0.0 )
			return "#weight( " + w.termWeight + " " + termQuery + " " + w.unorderedWeight + " " + unorderedQuery + " )"; 
		else if( w.termWeight == 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight != 0.0 )
			return "#weight( " + w.contiguousOrderedWeight + " " + orderedQuery + " " + w.unorderedWeight + " " + unorderedQuery + " )"; 
		else if( w.termWeight != 0.0 && w.contiguousOrderedWeight == 0.0 && w.unorderedWeight == 0.0 )
			return termQuery; 
		else if( w.termWeight == 0.0 && w.contiguousOrderedWeight != 0.0 && w.unorderedWeight == 0.0 )
			return orderedQuery; 
		else if( w.termWeight == 0.0 && w.contiguousOrderedWeight == 0.0 && w.unorderedWeight != 0.0 )
			return unorderedQuery; 
				
		return null;
	}
	
	public class Weighting {
		public double termWeight = 0.0;
		public double contiguousOrderedWeight = 0.0;
		public double noncontigOrderedWeight = 0.0;
		public double unorderedWeight = 0.0;
		
		public Weighting( double term, double contig, double noncontig, double unordered ) {
			this.termWeight = term;
			this.contiguousOrderedWeight = contig;
			this.noncontigOrderedWeight = noncontig;
			this.unorderedWeight = unordered; 
		}
	}	
}
