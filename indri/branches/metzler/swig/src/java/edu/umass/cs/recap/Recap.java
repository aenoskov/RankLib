package edu.umass.cs.recap;
import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.ImageIcon;
import javax.swing.JFrame;

import edu.umass.cs.indri.QueryEnvironment;
/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
public class Recap extends JFrame {
	
	private static final String VERSION = "0.5";
		
	public Recap() {
		setTitle( "RECAP v" + VERSION );
		setIconImage( (new ImageIcon("edu/umass/cs/recap/images/recap-small.png")).getImage() );
		setDefaultCloseOperation( EXIT_ON_CLOSE );
	}
	
	public void init( String [] args ) {
		// create an Indri interface
		QueryEnvironment indri = new QueryEnvironment();
		
		// command line mode object, if required
		CommandLineMode clm = null;
		
		// create the retrieval engine
		RetrievalEngine retEngine = new RetrievalEngine( indri );
		
		// parse the command line arguments
		boolean interactive = true;

		for( int i = 0; i < args.length; i++ ) {
			String curArg = args[i].toLowerCase();
			try {
				if( curArg.equals( "-index" ) ) {
					String nextArg= args[++i];
					retEngine.addIndex( nextArg );
					System.out.println( "Added index " + nextArg + "...");
				}
				else if( curArg.equals( "-server" ) ) {
					String nextArg= args[++i];
					retEngine.addServer( nextArg );
					System.out.println( "Added server " + nextArg + "...");
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
		
		if( interactive ) { // interactive mode
			// get the screen size
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();

			// add menu bar
			MainMenuBar mainMenuBar = new MainMenuBar();
			setJMenuBar( mainMenuBar );
			
			// setup and display the application window
			//mainPane = new MainPane( retEngine, mainMenuBar, screenSize );			
			InfoPane infoPane = new InfoPane( retEngine, mainMenuBar, screenSize );
			setContentPane( infoPane );
			
			// register action listener for the menu
			mainMenuBar.addActionListeners( infoPane );
			
			setSize( getMaximumSize() );						
			setVisible( true );
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
