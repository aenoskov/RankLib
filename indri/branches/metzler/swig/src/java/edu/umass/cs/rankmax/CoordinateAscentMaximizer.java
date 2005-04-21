/*
 * Created on Apr 17, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public class CoordinateAscentMaximizer extends Maximizer {

	// maximum number of iterations
	protected int MAX_ITERS = 100;

	// width of line search bracket
	protected double BRACKET_WIDTH = 1.0;
	
	// determines whether or not to keep the optimization constrainted to a simplex
	protected boolean onSimplex = false;
	
	public CoordinateAscentMaximizer( Ranker r, Evaluator e, Parameters parameters, boolean onSimplex ) {
		super( r, e );
		this.param = parameters;
		this.onSimplex = onSimplex;
	}
	
	public void maximize() {
		double curVal = eval( param );
		Parameters direction = new Parameters( param.size(), 0.0 );
		Bracket bracket = null;

		for( int iter = 0; iter < MAX_ITERS; iter++ ) {
			if( verbose ) {
				System.out.println( "ITERATION = " + iter );
				//System.out.println( "CURRENT PARAMETER = " + param );
				System.out.println( "FXN VALUE = " + curVal );
			}

			// perform one optimization step
			for( int i = 0; i < param.size(); i++ ) {
				System.out.println( "coordinate: " + i );
				if( i == 0 )
					direction.setParam( param.size() - 1, 0.0 );
				else
					direction.setParam( i - 1, 0.0 );
				direction.setParam( i, 1.0 );
				bracket = bracket( direction, 10E-16, -10.0, 10.0 );
				if( bracket.isBracket() ) {
					System.out.println( bracket );
					curVal = lineSearch( direction, bracket );
					System.out.println( "[BRACKET] curVal: " + curVal );
				}
				else { // if we don't have a bracket to search then step in the direction
					   // that maximizes our objective function as much as possible
					System.out.println( bracket );
					if( curVal == bracket.fa && bracket.fc > curVal ) {
						param = param.add( direction, 1.0, bracket.c );
						double tmp1 = eval( param );
						param.simplexNormalize();
						double tmp2 = eval( param );
						if( tmp1 != tmp2 ) {
							System.out.println( "[NON-BRACKET-0] non-normalized: " + tmp1 );
							System.out.println( "[NON-BRACKET-0] normalized: " + tmp2 );
							//System.exit(-1);
						}
						curVal = bracket.fc;
					}
					else if( curVal == bracket.fc && bracket.fa > curVal ) {
						param = param.add( direction, 1.0, bracket.a );
						double tmp1 = eval( param );
						param.simplexNormalize();
						double tmp2 = eval( param );
						if( tmp1 != tmp2 ) {
							System.out.println( "[NON-BRACKET-1] non-normalized: " + tmp1 );
							System.out.println( "[NON-BRACKET-1] normalized: " + tmp2 );
							//System.exit(-1);
						}
						curVal = bracket.fa;
					}
					else {
						if( bracket.fa > curVal && bracket.fa > bracket.fc ) {
							param = param.add( direction, 1.0, bracket.a );
							double tmp1 = eval( param );
							param.simplexNormalize();
							double tmp2 = eval( param );
							if( tmp1 != tmp2 ) {
								System.out.println( "[NON-BRACKET-2] non-normalized: " + tmp1 );
								System.out.println( "[NON-BRACKET-2] normalized: " + tmp2 );
								//System.exit(-1);
							}
							curVal = bracket.fa;
						}
						else if( bracket.fc > curVal && bracket.fc > bracket.fa ) {
							param = param.add( direction, 1.0, bracket.c );
							double tmp1 = eval( param );
							param.simplexNormalize();
							double tmp2 = eval( param );
							if( tmp1 != tmp2 ) {
								System.out.println( "[NON-BRACKET-3] non-normalized: " + tmp1 );
								System.out.println( "[NON-BRACKET-3] normalized: " + tmp2 );
								//System.exit(-1);
							}
							curVal = bracket.fc;
						}
					}
					System.out.println( "[NON-BRACKET] curVal: " + curVal );
				}
				if( onSimplex )
					param.simplexNormalize();
			}
		}
		
		if( verbose )
			System.out.println( "Total function evaluations = " + fxnEvaluations );
	}
	
}
