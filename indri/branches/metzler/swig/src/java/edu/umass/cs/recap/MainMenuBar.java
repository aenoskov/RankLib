/*
 * Created on Feb 16, 2005
 *
 */
package edu.umass.cs.recap;

import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;

/**
 * @author Don Metzler
 *
 */
public class MainMenuBar extends JMenuBar {

	// file menu
	private JMenu fileMenu = null;
	private JMenuItem addIndexItem = null;
	private JMenuItem addServerItem = null;
	private JMenuItem exitItem = null;
	
	// settings menu
	private JMenu settingsMenu = null;
	private JMenu similaritySubmenu = null;
	private JMenuItem idfoverlapSimItem = null;
	private JMenuItem overlapSimItem = null;
	private JMenuItem mt0SimItem = null;
	private JMenuItem qlSimItem = null;
	private JMenu numResultsSubmenu = null;
	private JMenuItem fiveResultsItem = null;
	private JMenuItem tenResultsItem = null;
	private JMenuItem twentyfiveResultsItem = null;
	private JMenuItem fiftyResultsItem = null;
	private JMenuItem onehundredResultsItem = null;

	// formatting menu
	private JMenu formattingMenu = null;
	private JMenuItem increaseFontSizeItem = null;
	private JMenuItem decreaseFontSizeItem = null;
	
	public MainMenuBar() {
		super();
		
		// file menu
		fileMenu = new JMenu( "File" );
		addIndexItem = new JMenuItem( "Add index..." );
		addServerItem = new JMenuItem( "Add server..." );
		exitItem = new JMenuItem( "Exit" );
		fileMenu.add( addIndexItem );
		fileMenu.add( addServerItem );
		fileMenu.addSeparator();
		fileMenu.add( exitItem );
		
		// settings menu
		settingsMenu = new JMenu( "Settings" );
		
		// sentence similarity method submenu
		similaritySubmenu = new JMenu( "Sentence similarity method" );
		ButtonGroup similarityGroup = new ButtonGroup();
		idfoverlapSimItem = new JRadioButtonMenuItem( "IDF-based Word Overlap" );
		overlapSimItem = new JRadioButtonMenuItem( "Word Overlap" );
		mt0SimItem = new JRadioButtonMenuItem( "Machine Translation (Model 0)" );
		qlSimItem = new JRadioButtonMenuItem( "Query likelihood" );
		
		similarityGroup.add( idfoverlapSimItem );
		similarityGroup.add( overlapSimItem );
		similarityGroup.add( mt0SimItem );
		similarityGroup.add( qlSimItem );
		
		similaritySubmenu.add( idfoverlapSimItem );
		idfoverlapSimItem.setSelected( true );
		similaritySubmenu.add( overlapSimItem );
		similaritySubmenu.add( mt0SimItem );
		similaritySubmenu.add( qlSimItem );
		
		// number of results submenu
		numResultsSubmenu = new JMenu( "Number of results" );
		ButtonGroup numResultsGroup = new ButtonGroup();
		fiveResultsItem = new JRadioButtonMenuItem( "5" );
		tenResultsItem = new JRadioButtonMenuItem( "10" );
		twentyfiveResultsItem = new JRadioButtonMenuItem( "25" );
		fiftyResultsItem = new JRadioButtonMenuItem( "50" );
		onehundredResultsItem = new JRadioButtonMenuItem( "100" );
		
		numResultsGroup.add( fiveResultsItem );
		numResultsGroup.add( tenResultsItem );
		numResultsGroup.add( twentyfiveResultsItem );
		numResultsGroup.add( fiftyResultsItem );
		numResultsGroup.add( onehundredResultsItem );
		
		numResultsSubmenu.add( fiveResultsItem );
		fiveResultsItem.setSelected( true );
		numResultsSubmenu.add( tenResultsItem );
		numResultsSubmenu.add( twentyfiveResultsItem );
		numResultsSubmenu.add( fiftyResultsItem );
		numResultsSubmenu.add( onehundredResultsItem );
		
		settingsMenu.add( similaritySubmenu );
		settingsMenu.add( numResultsSubmenu );

		// formatting menu
		formattingMenu = new JMenu( "Formatting" );
		increaseFontSizeItem = new JMenuItem( "Increase font size", new ImageIcon( "edu/umass/cs/recap/images/incfont.gif" ) );
		decreaseFontSizeItem = new JMenuItem( "Decrease font size", new ImageIcon( "edu/umass/cs/recap/images/decfont.gif" ) );
		formattingMenu.add( increaseFontSizeItem );
		formattingMenu.add( decreaseFontSizeItem );
				
		// construct the menu
		add( fileMenu );
		add( settingsMenu );
		add( formattingMenu );
	}
	
	// register listeners for each menu item
	public void addActionListeners( ActionListener listener ) {
		// file menu listeners
		addIndexItem.addActionListener( listener );
		addServerItem.addActionListener( listener );
		exitItem.addActionListener( listener );

		// settings menu listeners
		idfoverlapSimItem.addActionListener( listener );
		overlapSimItem.addActionListener( listener );
		mt0SimItem.addActionListener( listener );
		qlSimItem.addActionListener( listener );
		
		fiveResultsItem.addActionListener( listener );
		tenResultsItem.addActionListener( listener );
		twentyfiveResultsItem.addActionListener( listener );
		fiftyResultsItem.addActionListener( listener );
		onehundredResultsItem.addActionListener( listener );

		// formatting menu listeners
		increaseFontSizeItem.addActionListener( listener );
		decreaseFontSizeItem.addActionListener( listener );		
	}
	
}
