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

	// explore settings menu
	private JMenu exploreSubmenu = null;
	private JMenu numExploreResultsSubmenu = null;
	private JMenuItem fiveExploreResultsItem = null;
	private JMenuItem tenExploreResultsItem = null;
	private JMenuItem twentyfiveExploreResultsItem = null;
	private JMenuItem fiftyExploreResultsItem = null;
	private JMenuItem onehundredExploreResultsItem = null;
	
	// analyze settings menu
	private JMenu analyzeSubmenu = null;
	private JMenu similaritySubmenu = null;
	private JMenuItem idfoverlapSimItem = null;
	private JMenuItem overlapSimItem = null;
	private JMenuItem mt0SimItem = null;
	private JMenuItem qlSimItem = null;
	private JMenu numAnalyzeResultsSubmenu = null;
	private JMenuItem fiveAnalyzeResultsItem = null;
	private JMenuItem tenAnalyzeResultsItem = null;
	private JMenuItem twentyfiveAnalyzeResultsItem = null;
	private JMenuItem fiftyAnalyzeResultsItem = null;
	private JMenuItem onehundredAnalyzeResultsItem = null;

	// formatting menu
	private JMenu formattingMenu = null;
	private JMenuItem increaseFontSizeItem = null;
	private JMenuItem decreaseFontSizeItem = null;
	
	// help menu
	private JMenu helpMenu = null;
	private JMenuItem aboutItem = null;
	
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
		
		// explore submenu
		exploreSubmenu = new JMenu( "Explore Mode" );
		
		// number of explore results submenu
		numExploreResultsSubmenu = new JMenu( "Number of results" );
		ButtonGroup numExploreResultsGroup = new ButtonGroup();
		fiveExploreResultsItem = new JRadioButtonMenuItem( "5" );
		tenExploreResultsItem = new JRadioButtonMenuItem( "10" );
		twentyfiveExploreResultsItem = new JRadioButtonMenuItem( "25" );
		fiftyExploreResultsItem = new JRadioButtonMenuItem( "50" );
		onehundredExploreResultsItem = new JRadioButtonMenuItem( "100" );
		
		numExploreResultsGroup.add( fiveExploreResultsItem );
		numExploreResultsGroup.add( tenExploreResultsItem );
		numExploreResultsGroup.add( twentyfiveExploreResultsItem );
		numExploreResultsGroup.add( fiftyExploreResultsItem );
		numExploreResultsGroup.add( onehundredExploreResultsItem );

		numExploreResultsSubmenu.add( fiveExploreResultsItem );
		fiveExploreResultsItem.setSelected( true );
		numExploreResultsSubmenu.add( tenExploreResultsItem );
		numExploreResultsSubmenu.add( twentyfiveExploreResultsItem );
		numExploreResultsSubmenu.add( fiftyExploreResultsItem );
		numExploreResultsSubmenu.add( onehundredExploreResultsItem );

		exploreSubmenu.add( numExploreResultsSubmenu );
		
		// analyze submenu
		analyzeSubmenu = new JMenu( "Analyze Mode" );
		
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
		
		// number of analyze results submenu
		numAnalyzeResultsSubmenu = new JMenu( "Number of results" );
		ButtonGroup numAnalyzeResultsGroup = new ButtonGroup();
		fiveAnalyzeResultsItem = new JRadioButtonMenuItem( "5" );
		tenAnalyzeResultsItem = new JRadioButtonMenuItem( "10" );
		twentyfiveAnalyzeResultsItem = new JRadioButtonMenuItem( "25" );
		fiftyAnalyzeResultsItem = new JRadioButtonMenuItem( "50" );
		onehundredAnalyzeResultsItem = new JRadioButtonMenuItem( "100" );
		
		numAnalyzeResultsGroup.add( fiveAnalyzeResultsItem );
		numAnalyzeResultsGroup.add( tenAnalyzeResultsItem );
		numAnalyzeResultsGroup.add( twentyfiveAnalyzeResultsItem );
		numAnalyzeResultsGroup.add( fiftyAnalyzeResultsItem );
		numAnalyzeResultsGroup.add( onehundredAnalyzeResultsItem );
		
		numAnalyzeResultsSubmenu.add( fiveAnalyzeResultsItem );
		fiveAnalyzeResultsItem.setSelected( true );
		numAnalyzeResultsSubmenu.add( tenAnalyzeResultsItem );
		numAnalyzeResultsSubmenu.add( twentyfiveAnalyzeResultsItem );
		numAnalyzeResultsSubmenu.add( fiftyAnalyzeResultsItem );
		numAnalyzeResultsSubmenu.add( onehundredAnalyzeResultsItem );
		
		analyzeSubmenu.add( similaritySubmenu );
		analyzeSubmenu.add( numAnalyzeResultsSubmenu );

		settingsMenu.add( exploreSubmenu );
		settingsMenu.add( analyzeSubmenu );
		
		// formatting menu
		formattingMenu = new JMenu( "Formatting" );
		increaseFontSizeItem = new JMenuItem( "Increase font size", new ImageIcon( "edu/umass/cs/recap/images/incfont.gif" ) );
		decreaseFontSizeItem = new JMenuItem( "Decrease font size", new ImageIcon( "edu/umass/cs/recap/images/decfont.gif" ) );
		formattingMenu.add( increaseFontSizeItem );
		formattingMenu.add( decreaseFontSizeItem );

		// help menu
		helpMenu = new JMenu( "Help" );
		aboutItem = new JMenuItem( "About..." );
		helpMenu.add( aboutItem );
		
		// construct the menu
		add( fileMenu );
		add( settingsMenu );
		add( formattingMenu );
		add( helpMenu );
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
		
		fiveExploreResultsItem.addActionListener( listener );
		tenExploreResultsItem.addActionListener( listener );
		twentyfiveExploreResultsItem.addActionListener( listener );
		fiftyExploreResultsItem.addActionListener( listener );
		onehundredExploreResultsItem.addActionListener( listener );

		fiveAnalyzeResultsItem.addActionListener( listener );
		tenAnalyzeResultsItem.addActionListener( listener );
		twentyfiveAnalyzeResultsItem.addActionListener( listener );
		fiftyAnalyzeResultsItem.addActionListener( listener );
		onehundredAnalyzeResultsItem.addActionListener( listener );
		
		// formatting menu listeners
		increaseFontSizeItem.addActionListener( listener );
		decreaseFontSizeItem.addActionListener( listener );
		
		// help menu listeners
		aboutItem.addActionListener( listener );
	}

	// TODO: do this more intelligently
	public int getNumExploreResults() {
		if( fiveExploreResultsItem.isSelected() ) {
			return 5; 
		}
		else if( tenExploreResultsItem.isSelected() ) {
			return 10;
		}
		else if( twentyfiveExploreResultsItem.isSelected() ) {
			return 25;
		}
		else if( fiftyExploreResultsItem.isSelected() ) {
			return 50;
		}
		else if( onehundredExploreResultsItem.isSelected() ) {
			return 100;
		}
		return -1;
	}
	
	// TODO: do this more intelligently
	public int getNumAnalyzeResults() {
		if( fiveAnalyzeResultsItem.isSelected() ) {
			return 5; 
		}
		else if( tenAnalyzeResultsItem.isSelected() ) {
			return 10;
		}
		else if( twentyfiveAnalyzeResultsItem.isSelected() ) {
			return 25;
		}
		else if( fiftyAnalyzeResultsItem.isSelected() ) {
			return 50;
		}
		else if( onehundredAnalyzeResultsItem.isSelected() ) {
			return 100;
		}
		return -1;
	}

}
