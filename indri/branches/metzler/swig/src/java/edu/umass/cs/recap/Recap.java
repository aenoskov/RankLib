package edu.umass.cs.recap;
import javax.swing.JFrame;

import edu.umass.cs.indri.QueryEnvironment;
/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Recap extends JFrame {
	
	private final String VERSION = "0.1";
	
	private QueryEnvironment indri = null;
	private MainPane mainPane = null;
	
	public Recap() {
		setTitle( "RECAP v" + VERSION );
		setDefaultCloseOperation( EXIT_ON_CLOSE );
	}
	
	public void init( String [] args ) {
		// create an Indri interface
		indri = new QueryEnvironment();
		
		// parse the command line arguments
		boolean addedIndex = false;

		if( args.length == 0 ) {
			
		}
		
		for( int i = 0; i < args.length; i++ ) {
			String curArg = args[i].toLowerCase();
			try {
				if( curArg.equals( "-index" ) ) {
					String nextArg= args[++i];
					indri.addIndex( nextArg );
					System.out.println( "Added index " + nextArg + "...");
					addedIndex = true;
				}
				else if( curArg.equals( "-server" ) ) {
					String nextArg= args[++i];
					indri.addServer( nextArg );
					System.out.println( "Added server " + nextArg + "...");
					addedIndex = true;
				}
				else {
					System.err.println( "Unrecognized argument: " + curArg );
				}
			}
			catch( Exception e ) {
				System.err.println( "Malformed argument:" + curArg );
			}
		}
		
		if( !addedIndex ) {
			System.err.println( "Must specify at least one index or server!" );
			return;
		}
		
		// display the program window
		mainPane = new MainPane( indri );
		setContentPane( mainPane );

		setSize( getMaximumSize() );
		setVisible( true );
		
		mainPane.init();		
	}

	public static void main(String [] args) {
		Recap d = new Recap();
		d.init( args );
	}
}
