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

import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTextPane;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import edu.umass.cs.indri.ParsedDocument;
import edu.umass.cs.indri.QueryAnnotation;
import edu.umass.cs.indri.ScoredExtentResult;

/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
public class InfoPane extends JSplitPane implements ActionListener, ChangeListener, MouseListener, HyperlinkListener {
	
	private RetrievalEngine retEngine = null;
	
	private DocViewPane dvPane = null;
	private TimelinePanel tlPanel = null;
	private QueryPanel queryPanel = null;
	
	// default settings
	private String simMeasure = "#identsim1";
	private String queryExtent = "sentence";
	private int numExploreResults = 5;
	private int numAnalyzeResults = 5;
	
	public InfoPane( RetrievalEngine retEngine, Dimension screenSize ) {
		super( JSplitPane.VERTICAL_SPLIT );
		this.retEngine = retEngine;
		
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
		tlPanel.addActionListeners( this );
		setResizeWeight( 0.8 );
	}
	
	public void displayDoc( DocInfo doc ) {
		dvPane.displayDoc( doc );
	}

	// runs an "explore" query, scores documents, and updates appropriate on-screen info
	private void runExploreQuery( String query ) {		
		// error checking
		if( query == null || query.equals("") ) {
			this.showErrorDialog( "Unable to evaluate empty or null query!" );
			return;
		}
		if( !retEngine.isQueryable() ) { // make sure at least one index or server is open  
			showErrorDialog( "No server or index open to query against!" );
			return;
		}
		
		setCursor( new Cursor( Cursor.WAIT_CURSOR ) );
		
		Pair p = retEngine.runQuery( query, numExploreResults );
		
		QueryAnnotation annotation = (QueryAnnotation)p.left;
		Vector viewableResults = (Vector)p.right;

		ScoredExtentResult [] results = annotation.getResults();
		if( results == null || results.length == 0 ) {
			setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
			showErrorDialog( "Query returned no results!" );
			return;
		}
		
		// update DocViewPane
		dvPane.clearTabs();
		dvPane.displayExplorationResults( annotation, viewableResults );
		dvPane.getResultPane().addHyperlinkListener( this );
		dvPane.displayHighlightedDoc( results[0].document );
		
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
		// error checking
		if( query == null || query.equals("") ) {
			showErrorDialog( "Unable to evaluate empty or null query!");
			return;
		}
		if( !retEngine.isQueryable() ) { // make sure at least one index or server is open  
			showErrorDialog( "No server or index open to query against!" );
			return;
		}

		dvPane.clearTabs();
		setCursor( new Cursor( Cursor.WAIT_CURSOR ) );

		Vector viewableResults = retEngine.runQuery( query, simMeasure, queryExtent, numAnalyzeResults );
				
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
	
	public void setNumExploreResults( int results ) {
		numExploreResults = results;
	}
	
	public void setNumAnalyzeResults( int results ) {
		numAnalyzeResults = results;
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
					if( getMode().equals( "explore" ) )
						dvPane.displayHighlightedDoc( info.docID );
					repaint();
				}
				else { // 2+ clicks => analyze document
					//updater.setSelectedDoc( doc );
					if( getMode().equals( "analyze" ) )
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
			String queryText = pane.getSelectedText();
			if( queryText != null && !queryText.trim().equals("") )
				queryPanel.setQueryText( pane.getSelectedText() );
		}
	}

	// required for ActionListener
	public void actionPerformed( ActionEvent e ) {
		Object src = e.getSource();
		if( src == queryPanel.getRunQueryButton() ) {
			String mode = getMode();
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
			
			try {
				StringTokenizer tok = new StringTokenizer( startDate, "/" );
				int minMonth = Integer.parseInt( tok.nextToken() );
				int minYear = Integer.parseInt( tok.nextToken() );
				
				tok = new StringTokenizer( endDate, "/" );
				int maxMonth = Integer.parseInt( tok.nextToken() );
				int maxYear = Integer.parseInt( tok.nextToken() );
				
				if( minYear > maxYear || ( minYear == maxYear && minMonth >= maxMonth ) ) {
					showErrorDialog( "Start date must be before end date!" );
					return;
				}
				
				tlPanel.setStartDate( minMonth, minYear );
				tlPanel.setEndDate( maxMonth, maxYear );
			
				repaint();
			}
			catch( Exception a ) {
				showErrorDialog( "Dates must be in MM/YYYY format!" );
			}
		}
		else if( src == tlPanel.getPreviousDocButton() ) {
			ScoredDocInfo info = tlPanel.getPreviousDoc();
			DocInfo doc = new DocInfo( info.docName, info.docID );
			tlPanel.setCurrent( info.docName );
			dvPane.setSelectedDoc( doc );
			if( getMode().equals( "explore" ) )
				dvPane.displayHighlightedDoc( info.docID );
			repaint();
		}
		else if( src == tlPanel.getNextDocButton() ) {
			ScoredDocInfo info = tlPanel.getNextDoc();
			DocInfo doc = new DocInfo( info.docName, info.docID );
			tlPanel.setCurrent( info.docName );
			dvPane.setSelectedDoc( doc );
			if( getMode().equals( "explore" ) )
				dvPane.displayHighlightedDoc( info.docID );
			repaint();
		}
	}

	// process a hyperlink click
	public void hyperlinkUpdate(HyperlinkEvent e) {		
		if( e.getEventType() == HyperlinkEvent.EventType.ACTIVATED ) {
			StringTokenizer tok = new StringTokenizer( e.getDescription(), ":" );
			String docName = tok.nextToken();
			String linkType = tok.nextToken();
			int docID = Integer.parseInt( tok.nextToken() );
			dvPane.displayHighlightedDoc( docID );
			if( linkType.equals( "extent" ) ) {				
				int begin = Integer.parseInt( tok.nextToken() );
				int end = Integer.parseInt( tok.nextToken() );
				highlightExtent( docID, begin, end );
			}
			tlPanel.setCurrent( docName );
			repaint();
		}
	}
	
	private void highlightExtent( int docID, int begin, int end ) {
		JTextPane docTextPane = dvPane.getDocTextPane();
		ParsedDocument doc = retEngine.getParsedDocument( docID );				
		int startPos = doc.positions[ begin ].begin;
		int endPos = doc.positions[ end - 1 ].end;
		int curPos = docTextPane.getCaretPosition();
		docTextPane.grabFocus();
		if( curPos > endPos )
			docTextPane.setCaretPosition( startPos );
		else
			docTextPane.setCaretPosition( endPos );
		docTextPane.setCaretPosition( startPos );
		docTextPane.moveCaretPosition( endPos );
	}
	
	public String getMode() {
		return queryPanel.getMode();
	}
	
	protected void showErrorDialog( String msg ) {		
		JOptionPane.showMessageDialog( this, msg, "Error", JOptionPane.ERROR_MESSAGE );
	}
}
