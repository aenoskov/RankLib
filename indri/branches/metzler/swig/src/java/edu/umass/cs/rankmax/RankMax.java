/*
 * Created on Apr 26, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.lang.reflect.Constructor;

/**
 * @author Don Metzler
 *
 */
public class RankMax {

	private final static String [] maximizerNames = new String [] { "grid", "neldermead", "coordinate", "steepest" }; 
	private final static String [] maximizerClasses = new String [] { "edu.umass.cs.rankmax.GridMaximizer", "edu.umass.cs.rankmax.NelderMeadMaximizer", "edu.umass.cs.rankmax.CoordinateAscentMaximizer", "edu.umass.cs.rankmax.SteepestAscentMaximizer" };
	
	private final static String [] rankerNames = new String [] { "classification", "indrismoothing", "oneclass" }; 
	private final static String [] rankerClasses = new String [] { "edu.umass.cs.rankmax.ClassificationSearch", "edu.umass.cs.rankmax.IndriSmoothingSearch", "edu.umass.cs.rankmax.OneClassSearch" };

	private final static String [] evaluatorNames = new String [] { "map", "p1", "p5", "p10" }; 
	private final static String [] evaluatorClasses = new String [] { "edu.umass.cs.rankmax.AveragePrecisionEvaluator", "edu.umass.cs.rankmax.PrecisionAtNEvaluator", "edu.umass.cs.rankmax.PrecisionAtNEvaluator", "edu.umass.cs.rankmax.PrecisionAtNEvaluator" };
	private final static Object [] evaluatorArgs = new Object [] { null, new Integer(1), new Integer(5), new Integer(10) };
	
	private static void printUsage() {
		System.out.println( "---------------------------------");
		System.out.println( "Required arguments:"              );
		System.out.println( "   -maximizer name"               );
		System.out.println( "   -ranker name input_file"       );
		System.out.println( "   -evaluator name input_file"    );
		System.out.println( "---------------------------------");
		System.out.println( "Optional arguments:"              );
		System.out.println( "   -maxiters num"                 );
		System.out.println( "   -onsimplex"                    );
		System.out.println( "   -saveparam param_file"         );
		System.out.println( "   -loadparam param_file"         );
		System.out.println( "   -randomstart"                  );
		System.out.println( "   -printrankings"                );
		System.out.println( "   -verbose"                      );
		System.out.println( "---------------------------------");
		System.out.println( "Maximizer types:"                 );
		printNames( maximizerNames );
		System.out.println( "---------------------------------");
		System.out.println( "Ranker types:"                    );
		printNames( rankerNames );
		System.out.println( "---------------------------------");
		System.out.println( "Evaluator types:"                 );
		printNames( evaluatorNames );
		System.out.println( "---------------------------------");
	}

	private static void printNames( String [] names ) {
		for( int i = 0; i < names.length; i++ ) {
			System.out.println( "   " + names[i] );
		}
	}
	
	public static void main( String [] args ) {
		if( args.length == 0 ) {
			printUsage();
			System.exit( 0 );
		}

		Maximizer maximizer = null;
		Constructor maximizerConstructor = null;
		Ranker ranker = null;
		Evaluator evaluator = null;
		Parameters p0 = null;
		String saveParamFile = null;
		int maxIters = -1;
		boolean randomStart = false;
		boolean onSimplex = false;
		boolean verbose = false;
		
		// parse the command line options
		for( int i = 0; i < args.length; i++ ) {
			String curArg = args[ i ];
			try {
				if( curArg.toLowerCase().equals( "-maximizer" ) ) {
					String name = args[ ++i ].toLowerCase();					
					int idx = lookup( name, maximizerNames );
					if( idx == -1 ) {
						System.err.println( "Unrecognized maximizer name: " + name );
						return;
					}
					Class c = Class.forName( maximizerClasses[ idx ] );
					maximizerConstructor = c.getConstructor( new Class [] { Ranker.class, Evaluator.class, Parameters.class } );
				}
				else if( curArg.toLowerCase().equals( "-ranker" ) ) {
					String name = args[ ++i ].toLowerCase();					
					String filename = args[ ++i ];
					int idx = lookup( name, rankerNames );
					if( idx == -1 ) {
						System.err.println( "Unrecognized ranker name: " + name );
						return;
					}
					Class c = Class.forName( rankerClasses[ idx ] );
					ranker = (Ranker)( c.getConstructor( new Class [] { String.class } ) ).newInstance( new Object [] { filename } );
				}
				else if( curArg.toLowerCase().equals( "-evaluator" ) ) {
					String name = args[ ++i ].toLowerCase();
					String filename = args[ ++i ];
					int idx = lookup( name, evaluatorNames );
					if( idx == -1 ) {
						System.err.println( "Unrecognized evaluator name: " + name );
						return;
					}
					Class c = Class.forName( evaluatorClasses[ idx ] );
					if( evaluatorArgs[ idx ] == null )
						evaluator = (Evaluator)( c.getConstructor( new Class [] { String.class } ) ).newInstance( new Object [] { filename } );
					else // TODO: generalize this if necessary 
						evaluator = (Evaluator)( c.getConstructor( new Class [] { String.class, int.class } ) ).newInstance( new Object [] { filename, evaluatorArgs[ idx ] } );
				}
				else if( curArg.toLowerCase().equals( "-maxiters" ) ) {
					maxIters = Integer.parseInt( args[ ++i ] );					
				}
				else if( curArg.toLowerCase().equals( "-onsimplex" ) ) {
					onSimplex = true;
				}
				else if( curArg.toLowerCase().equals( "-saveparam" ) ) {
					saveParamFile = args[ ++i ];
				}
				else if( curArg.toLowerCase().equals( "-loadparam" ) ) {
					
				}
				else if( curArg.toLowerCase().equals( "-randomstart" ) ) {
					randomStart = true;
				}
				else if( curArg.toLowerCase().equals( "-printrankings" ) ) {
					
				}
				else if( curArg.toLowerCase().equals( "-verbose" ) ) {
					verbose = true;					
				}
			}
			catch( Exception e ) {
				e.printStackTrace();
				System.err.println( "Encountered an error while parsing: " + curArg );
				System.exit( -1 );
			}
		}
		
		// perform the optimization
		try {
			if( !randomStart )
				p0 = ranker.getDefaultStartParam();
			else
				p0 = ranker.getRandomStartParam();
			if( onSimplex )
				p0.simplexNormalize();
			maximizer = (Maximizer)maximizerConstructor.newInstance( new Object [] { ranker, evaluator, p0 } );
			maximizer.setOnSimplex( onSimplex );
			maximizer.setVerbose( verbose );
			if( maxIters != -1 )
				maximizer.setMaxNumIters( maxIters );
			maximizer.maximize();
			if( saveParamFile != null )
				maximizer.writeParamToFile( saveParamFile );
		}
		catch( Exception e ) {
			e.printStackTrace();
			System.err.println( "Caught an exception during optimization!" );
		}
	}
	
	private static int lookup( String key, String [] values ) {
		int idx = -1;
		for( int i = 0; i < values.length; i++ ) {
			if( key.equals(values[i]) ) {
				idx = i;
				break;
			}
		}		
		return idx;
	}
	
}
