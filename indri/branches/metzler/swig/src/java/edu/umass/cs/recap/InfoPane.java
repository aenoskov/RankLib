package edu.umass.cs.recap;
import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTextPane;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
public class InfoPane extends JSplitPane implements ActionListener, ChangeListener, MouseListener {
	
	private RetrievalEngine retEngine = null;
	
	private DocViewPane dvPane = null;
	private TimelinePanel tlPanel = null;
	private QueryPanel queryPanel = null;
	
	private MainPaneUpdater updater = null;

	// default settings
	private String simMeasure = "#identsim1";
	private String queryExtent = "sentence";
	private int numResults = 5;
	
	public InfoPane( RetrievalEngine retEngine, Dimension screenSize, MainPaneUpdater updater ) {
		super( JSplitPane.VERTICAL_SPLIT );
		this.retEngine = retEngine;
		this.updater = updater;
		
		tlPanel = new TimelinePanel();
		dvPane = new DocViewPane( retEngine, screenSize );
		queryPanel = new QueryPanel();
		JPanel topPanel = new JPanel();
		
		topPanel.setLayout( new BorderLayout() );		
		topPanel.add( dvPane, BorderLayout.CENTER );
		topPanel.add( queryPanel, BorderLayout.SOUTH );
		
		tlPanel.setMinimumSize( new Dimension( 0, 100 ) );
		setTopComponent( topPanel );
		setBottomComponent( tlPanel );
		
		dvPane.getDocTextPane().addMouseListener( this );
		dvPane.getMatchPane().addChangeListener( this );

		queryPanel.getRunQueryButton().addActionListener( this );
		queryPanel.getClearQueryButton().addActionListener( this );
		queryPanel.getUpdateTimelineButton().addActionListener( this );
		
		tlPanel.addMouseListener( this );
	}
	
	public void displayDoc( DocInfo doc ) {
		dvPane.displayDoc( doc );
	}

	// runs an "explore" query, scores documents, and updates appropriate on-screen info
	private void runExploreQuery( String query ) {
		dvPane.clearTabs();		
		setCursor( new Cursor( Cursor.WAIT_CURSOR ) );

		Vector viewableResults = retEngine.runQuery( query, numResults );
				
		// update DocViewPane
		dvPane.addMatches( viewableResults );
		
		// update TimelinePanel
		tlPanel.setResults( viewableResults );
		String curDocName = ( (ScoredDocInfo)viewableResults.elementAt( 0 ) ).docName;
		tlPanel.setCurrent( curDocName );

		// update QueryPanel
		queryPanel.setTimelineStartDate( tlPanel.getStartDate() );
		queryPanel.setTimelineEndDate( tlPanel.getEndDate() );
		
		setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
	}	
		
	// runs an "analyze" query, scores documents, and updates appropriate on-screen info
	private void runAnalyzeQuery( String query ) {
		dvPane.clearTabs();
		setCursor( new Cursor( Cursor.WAIT_CURSOR ) );

		Vector viewableResults = retEngine.runQuery( query, simMeasure, queryExtent, numResults );
				
		// update DocViewPane
		dvPane.addMatches( viewableResults );
		
		// update TimelinePanel
		tlPanel.setResults( viewableResults );
		String curDocName = ( (ScoredDocInfo)viewableResults.elementAt( 0 ) ).docName;
		tlPanel.setCurrent( curDocName );

		// update QueryPanel
		queryPanel.setTimelineStartDate( tlPanel.getStartDate() );
		queryPanel.setTimelineEndDate( tlPanel.getEndDate() );
		
		setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
	}	
	
	public void setSimMeasure( String simType ) {
		simMeasure = simType;
	}
	
	public void setExtent( String extent ) {
		queryExtent = extent;
	}
	
	public void setNumResults( int results ) {
		numResults = results;
	}
	
	public void stateChanged( ChangeEvent e ) {		
		if( dvPane != null && dvPane.getMatchPane().getSelectedIndex() != -1 ) {
			String curDoc = dvPane.getMatchPane().getTitleAt( dvPane.getMatchPane().getSelectedIndex() );
			tlPanel.setCurrent( curDoc );
			repaint();
		}
	}

	public void mouseClicked( MouseEvent e ) {
		Object src = e.getSource();
		if( src == tlPanel ) {
			ScoredDocInfo info = tlPanel.getDocAt( e.getPoint() );
			int clickCount = e.getClickCount();
			if( info != null ) {
				DocInfo doc = new DocInfo( info.docName, info.docID );
				if( clickCount == 1 ) { // 1 click => set tab to doc
					tlPanel.setCurrent( info.docName );
					dvPane.setSelectedDoc( doc );
				}
				else { // 2+ clicks => analyze document
					//updater.setSelectedDoc( doc );
					displayDoc( doc );
				}
			}
		}
	}
	
	// required for MouseListener, but not implemented
	public void mouseEntered( MouseEvent e ) {}
	public void mouseExited( MouseEvent e ) {}
	public void mousePressed( MouseEvent e ) {}
	public void mouseReleased( MouseEvent e ) {
		Object src = e.getSource();
		if( src == dvPane.getDocTextPane() ) {
			JTextPane pane = dvPane.getDocTextPane();
			queryPanel.setQueryText( pane.getSelectedText() );
		}
	}

	// required for ActionListener
	public void actionPerformed( ActionEvent e ) {
		Object src = e.getSource();
		if( src == queryPanel.getRunQueryButton() ) {
			String mode = queryPanel.getMode();
			if( mode.equals( "explore" ) )
				runExploreQuery( queryPanel.getQueryText() );
			else if( mode.equals( "analyze" ) )
				runAnalyzeQuery( queryPanel.getQueryText() );
			else
				System.err.println( "ERROR -- invalid search mode!" );
		}
		else if( src == queryPanel.getClearQueryButton() ) {
			queryPanel.setQueryText("");
		}
		else if( src == queryPanel.getUpdateTimelineButton() ) {
			String startDate = queryPanel.getTimelineStartDate();
			String endDate = queryPanel.getTimelineEndDate();
			
			StringTokenizer tok = new StringTokenizer( startDate, "/" );
			int minMonth = Integer.parseInt( tok.nextToken() );
			int minYear = Integer.parseInt( tok.nextToken() );
			tlPanel.setStartDate( minMonth, minYear );

			tok = new StringTokenizer( endDate, "/" );
			int maxMonth = Integer.parseInt( tok.nextToken() );
			int maxYear = Integer.parseInt( tok.nextToken() );
			tlPanel.setEndDate( maxMonth, maxYear );
			
			repaint();
		}
	}
}
