/*
 * Created on Apr 26, 2005
 *
 */
package edu.umass.cs.rankmax;

import java.lang.reflect.Constructor;
import java.util.Collections;

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
		System.out.println( "Two main modes of operation:"     );
		System.out.println( "   -train"                        );
		System.out.println( "   -test"                         );
		System.out.println( "---------------------------------");
		System.out.println( "Train mode required arguments:"   );
		System.out.println( "   -maximizer name"               );
		System.out.println( "   -ranker name input_file"       );
		System.out.println( "   -evaluator name input_file"    );
		System.out.println( "---------------------------------");
		System.out.println( "Train mode optional arguments:"   );
		System.out.println( "   -maxiters num"                 );
		System.out.println( "   -onsimplex"                    );
		System.out.println( "   -saveparam param_file"         );
		System.out.println( "   -loadparam param_file"         );
		System.out.println( "   -randomstart"                  );
		System.out.println( "   -randomrestart num"            );
		System.out.println( "   -verbose"                      );
		System.out.println( "---------------------------------");
		System.out.println( "Test mode required arguments:"    );
		System.out.println( "   -ranker name input_file"       );
		System.out.println( "   -evaluator name input_file"    );
		System.out.println( "   -loadparam param_file"         );
		System.out.println( "---------------------------------");
		System.out.println( "Test mode optional arguments:"    );
		System.out.println( "   -printrankings num run_id"     );
		System.out.println( "   -printevaluation"              );
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

		// two main modes of operation
		boolean training = false;
		boolean testing = false;
		
		Maximizer maximizer = null;
		Constructor maximizerConstructor = null;
		Ranker ranker = null;
		Evaluator evaluator = null;
		Parameters p0 = null;
		String saveParamFile = null;
		String loadParamFile = null;
		int maxIters = -1;
		int numRandomRestarts = 1;
		boolean randomStart = false;
		boolean onSimplex = false;
		boolean printRankings = false;
		int rankingNum = 0;
		String rankingID = null;
		boolean printEvaluation = false;
		boolean verbose = false;
		
		double bestVal = Double.MIN_VALUE;
		
		// parse the command line options
		for( int i = 0; i < args.length; i++ ) {
			String curArg = args[ i ];
			try {
				if( curArg.toLowerCase().equals( "-train" ) ) {
					training = true;					
				}
				else if( curArg.toLowerCase().equals( "-test" ) ) {
					testing = true;
				}
				else if( curArg.toLowerCase().equals( "-maximizer" ) ) {
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
					loadParamFile = args[ ++i ];
				}
				else if( curArg.toLowerCase().equals( "-randomstart" ) ) {
					randomStart = true;
				}
				else if( curArg.toLowerCase().equals( "-randomrestart" ) ) {
					numRandomRestarts = Integer.parseInt( args[ ++i ] );
				}
				else if( curArg.toLowerCase().equals( "-printrankings" ) ) {
					printRankings = true;
					rankingNum = Integer.parseInt( args[ ++i ] );
					rankingID = args[ ++i ];
				}
				else if( curArg.toLowerCase().equals( "-printevaluation" ) ) {
					printEvaluation = true;
				}
				else if( curArg.toLowerCase().equals( "-verbose" ) ) {
					verbose = true;					
				}
				else {
					System.err.println( "Unrecognized command: " + curArg + " -- ignoring" );
				}
			}
			catch( Exception e ) {
				e.printStackTrace();
				System.err.println( "Encountered an error while parsing: " + curArg );
				System.exit( -1 );
			}
		}
		
		if( !training && !testing ) {
			System.err.println( "No mode selected! Must specify -train and/or -test");
			System.exit( -1 );
		}
		
		// perform the optimization
		try {
			if( training ) {
				if( loadParamFile != null )
					p0 = new Parameters( loadParamFile );
				else if( !randomStart )
					p0 = ranker.getDefaultStartParam();
				else
					p0 = ranker.getRandomStartParam();
				if( onSimplex )
					p0.simplexNormalize();
				for( int runNum = 0; runNum < numRandomRestarts; runNum++ ) {
					maximizer = (Maximizer)maximizerConstructor.newInstance( new Object [] { ranker, evaluator, p0 } );
					maximizer.setOnSimplex( onSimplex );
					maximizer.setVerbose( verbose );
					if( maxIters != -1 )
						maximizer.setMaxNumIters( maxIters );
					double val = maximizer.maximize();
					if( val > bestVal ) {
						if( saveParamFile != null )
							maximizer.writeParamToFile( saveParamFile );
						bestVal = val;
					}
					p0 = ranker.getRandomStartParam();
				}
				System.out.println("Final evaluation value: " + bestVal );
			}
			
			if( testing ) {
				p0 = new Parameters( loadParamFile );
				Ranking [] rankings = ranker.getRankings( p0 );
				if( printRankings ) {
					for( int i = 0; i < rankings.length; i++ ) {
						Collections.sort( rankings[i] );
						rankings[i].print( rankingNum, rankingID );
					}
				}
				if( printEvaluation ) {
					System.out.println( "Evaluation value: " + evaluator.evaluate( rankings ) );
					if( rankingNum > 0 )
						System.out.println( "NOTE: evaluation value is based on ENTIRE ranking, not just top " + rankingNum + "!" );
				}
			}
		}
		catch( Exception e ) {
			e.printStackTrace();
			System.err.println( "Caught an exception -- make sure proper command line arguments were specified!" );
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
