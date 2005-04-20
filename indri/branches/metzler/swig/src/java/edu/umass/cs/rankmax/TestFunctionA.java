/*
 * Created on Mar 13, 2005
 *
 */
package edu.umass.cs.rankmax;

/**
 * @author Don Metzler
 *
 */
public class TestFunctionA implements Ranker, Evaluator {

	private Parameters curParams = null;
	
	public Ranking [] getRankings( Parameters p ) {
		curParams = p;
		return null;
	}

	public double evaluate( Ranking [] results ) {
		//return -curParams.getParam(0)*curParams.getParam(0);
		return -Math.sin(curParams.getParam(1)) + Math.sin( curParams.getParam(0) );
	}

	public static void main( String [] args ) {
		TestFunctionA fxn = new TestFunctionA();
		Parameters a = new Parameters( new double [] { -5, -1 } );
		Parameters b = new Parameters( new double [] { 20, 10 } );
		Parameters c = new Parameters( new double [] { 7, 2 } );
		Parameters [] params = new Parameters [] { a, b, c };
		//Parameters a = new Parameters( new double [] { -5 } );
		//Parameters b = new Parameters( new double [] { 7 } );
		//Parameters c = new Parameters( new double [] { 7, 2 } );
		//Parameters [] params = new Parameters [] { a, b };
		Maximizer m;
		
		m = new NelderMeadMaximizer( fxn, fxn, params );
		m.setVerbose( true );
		m.maximize();
		
		m = new CoordinateAscentMaximizer( fxn, fxn, a, false );
		m.setVerbose( true );
		m.maximize();
	}
}
