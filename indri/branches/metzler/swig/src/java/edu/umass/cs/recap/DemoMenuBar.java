/*
 * Created on Apr 26, 2005
 *
 */
package edu.umass.cs.recap;

import java.awt.event.ActionListener;
import java.io.RandomAccessFile;
import java.util.ArrayList;

import javax.swing.JMenu;
import javax.swing.JMenuItem;

/**
 * @author Don Metzler
 *
 */
public class DemoMenuBar extends MainMenuBar {

	protected ArrayList queryItems = null;
	
	public DemoMenuBar( String queryFile ) {
		super();		
		removeAll();
		queryItems = new ArrayList();
		
		JMenu queryMenu = new JMenu( "Query" );
		
		RandomAccessFile in = null;
		try { in = new RandomAccessFile( queryFile, "r" ); }
		catch( Exception e ) {
			System.err.println( "Error reading demo query file!" );
		}
		
		String str = null;
		try {
			while( ( str = in.readLine() ) != null ) {
				String [] data = str.split( "\t" );
				QueryMenuItem item = new QueryMenuItem( data[0], data[1] );
				queryMenu.add( item );
				queryItems.add( item );
			}
		}
		catch( Exception e ) {
			System.err.println( "Encountered an error while reading demo query file!" );			
		}
		
		add( queryMenu );
	}
	
	public void addActionListeners( ActionListener listener ) {
		// add listeners for each demo query
		for( int i = 0; i < queryItems.size(); i++ )
			( (QueryMenuItem)queryItems.get( i ) ).addActionListener( listener );
	}
	
	public class QueryMenuItem extends JMenuItem {
		public String query;
		
		public QueryMenuItem( String description, String query ) {
			super( description );
			this.query = query;
		}
	}
}
