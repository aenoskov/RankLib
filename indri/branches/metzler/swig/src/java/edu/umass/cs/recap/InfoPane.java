package edu.umass.cs.recap;
import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
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
	
	public InfoPane( RetrievalEngine retEngine, MainPaneUpdater updater ) {
		super( JSplitPane.VERTICAL_SPLIT );
		this.retEngine = retEngine;
		this.updater = updater;
		
		tlPanel = new TimelinePanel();
		dvPane = new DocViewPane( retEngine );
		queryPanel = new QueryPanel( retEngine.getFieldList() );
		JPanel topPanel = new JPanel();
		
		topPanel.setLayout( new BorderLayout() );
		topPanel.add( dvPane, BorderLayout.CENTER );
		topPanel.add( queryPanel, BorderLayout.SOUTH );
		
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

	// runs a query, scores documents, and updates appropriate on-screen info
	private void runQuery( String query ) {
		dvPane.clearTabs();
		String queryOp = queryPanel.getSimMeasure();
		String queryExtent = queryPanel.getExtent();
		String queryCombiner = queryPanel.getCombiner();
		int numResults = queryPanel.getNumResults();
		
		this.setCursor( new Cursor( Cursor.WAIT_CURSOR ) );

		Vector viewableResults = retEngine.runQuery( query, queryOp, queryExtent, queryCombiner, numResults );
				
		// update DocViewPane
		dvPane.addMatches( viewableResults );
		
		// update TimelinePanel
		tlPanel.setResults( viewableResults );
		String curDocName = ( (ScoredDocInfo)viewableResults.elementAt( 0 ) ).docName;
		tlPanel.setCurrent( curDocName );

		// update QueryPanel
		queryPanel.setTimelineStartDate( tlPanel.getStartDate() );
		queryPanel.setTimelineEndDate( tlPanel.getEndDate() );
		
		this.setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
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
					updater.setSelectedDoc( doc );
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
			runQuery( queryPanel.getQueryText() );
		}
		else if( src == queryPanel.getClearQueryButton() ) {
			queryPanel.setQueryText("");
		}
		else if( src == queryPanel.getUpdateTimelineButton() ) {
			//tlPanel.
		}
	}
}
