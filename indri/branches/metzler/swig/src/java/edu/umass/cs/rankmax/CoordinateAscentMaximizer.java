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

	public CoordinateAscentMaximizer( Ranker r, Evaluator e, Parameters parameters ) {
		super( r, e, parameters );
		this.param = parameters;
	}
	
	public void maximize() {
		double curVal = eval( param );
		Parameters direction = new Parameters( param.size(), 0.0 );
		Bracket bracket = null;
		int lastCoordinate = 0;

		int [] permutation = new int[ param.size() ];
		for( int i = 0; i < permutation.length; i++ )
			permutation[ i ] = i;
		
		for( int iter = 0; iter < maxNumIters; iter++ ) {
			verbosePrint( "[CoordinateAscentMaximizer.maximize] ITERATION = " + iter );
			//verbosePrint( "CURRENT PARAMETER = " + param );
			verbosePrint( "[CoordinateAscentMaximizer.maximize] FXN VALUE = " + curVal );

			// permute the ordering of the coordinates
			for( int i = 0; i < param.size(); i++ ) {
				int pos = (int)( Math.random() * param.size() );
				int tmp = permutation[ i ];
				permutation[ i ] = permutation[ pos ];
				permutation[ pos ] = tmp;
			}
							
			// perform one optimization step
			for( int i = 0; i < param.size(); i++ ) {
				verbosePrint( "[CoordinateAscentMaximizer.maximize] coordinate: " + permutation[ i ] );
				
				// set up direction vector
				direction.setParam( lastCoordinate, 0.0 );
				direction.setParam( permutation[ i ], 1.0 );
				lastCoordinate = permutation[ i ];

				//bracket = bracket( direction, 10E-16, -10.0, 10.0 );
				double curParam = param.getParam( permutation[ i ] );
				if( curParam >= 1 )
					bracket = bracket( direction, Math.sqrt( curParam ), -curParam, 10.0 );
				else
					bracket = bracket( direction, curParam * curParam, -curParam, 10.0 );

				verbosePrint( "bracket: " + bracket );
				
				if( bracket.isBracket() ) {
					curVal = lineSearch( direction, bracket );
					verbosePrint( "[CoordinateAscentMaximizer.maximize] [BRACKET] curVal: " + curVal );
				}
				else { // if we don't have a bracket to search then step in the direction
					   // that maximizes our objective function as much as possible
					if( curVal == bracket.fa && bracket.fc > curVal ) {
						param = param.add( direction, 1.0, bracket.c );
						curVal = bracket.fc;
					}
					else if( curVal == bracket.fc && bracket.fa > curVal ) {
						param = param.add( direction, 1.0, bracket.a );
						curVal = bracket.fa;
					}
					else {
						if( bracket.fa > curVal && bracket.fa > bracket.fc ) {
							param = param.add( direction, 1.0, bracket.a );
							curVal = bracket.fa;
						}
						else if( bracket.fc > curVal && bracket.fc > bracket.fa ) {
							param = param.add( direction, 1.0, bracket.c );
							curVal = bracket.fc;
						}
					}
					verbosePrint( "[CoordinateAscentMaximizer.maximize] [NON-BRACKET] curVal: " + curVal );
				}
				if( onSimplex )
					param.simplexNormalize();
			}
		}
		
		verbosePrint( "[CoordinateAscentMaximizer.maximize] Total function evaluations = " + fxnEvaluations );
	}
	
}
