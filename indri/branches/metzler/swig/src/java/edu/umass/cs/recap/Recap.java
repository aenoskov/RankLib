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
	
	private MainPane mainPane = null;
	
	public Recap() {
		setTitle( "RECAP v" + VERSION );
		setDefaultCloseOperation( EXIT_ON_CLOSE );
	}
	
	public void init( String [] args ) {
		// create an Indri interface
		QueryEnvironment indri = new QueryEnvironment();
		
		// command line mode object, if required
		CommandLineMode clm = null;
		
		// parse the command line arguments
		boolean addedIndex = false;
		boolean interactive = true;

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
				else if( curArg.equals( "-commandline") ) {
					interactive = false;
					String nextArg = args[++i];
					clm = new CommandLineMode( nextArg );
				}
				else {
					System.err.println( "Unrecognized argument: " + curArg );
				}
			}
			catch( Exception e ) {
				System.err.println( "An error occurred while parsing argument: " + curArg + " [" + e.getMessage() + "]" );
				return;
			}
		}
		
		if( !addedIndex ) {
			System.err.println( "Must specify at least one index or server!" );
			return;
		}

		// create the retrieval engine
		RetrievalEngine retEngine = new RetrievalEngine( indri );

		if( interactive ) { // interactive mode
			// setup and display the application window
			mainPane = new MainPane( retEngine );
			setContentPane( mainPane );

			setSize( getMaximumSize() );
			setVisible( true );
		
			mainPane.init();
		}
		else { // command line mode
			clm.run( retEngine );
		}
	}

	public static void main(String [] args) {
		Recap d = new Recap();
		d.init( args ); // parse command line options
	}
}
