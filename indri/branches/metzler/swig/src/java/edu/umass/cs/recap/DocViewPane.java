package edu.umass.cs.recap;
import java.awt.Dimension;
import java.util.Vector;

import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.text.DefaultStyledDocument;

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
public class DocViewPane extends JSplitPane {

	private RetrievalEngine retEngine = null;
	
	private JTabbedPane matchPane = null;
	private JTextPane docTextPane = null;
	private JScrollPane docPane = null;
	
	public DocViewPane( RetrievalEngine retEngine, Dimension screenSize ) {
		super( JSplitPane.HORIZONTAL_SPLIT );
		this.retEngine = retEngine;

		docTextPane = new JTextPane();		
		matchPane = new JTabbedPane( JTabbedPane.TOP, JTabbedPane.SCROLL_TAB_LAYOUT );
		docPane = new JScrollPane();
		docPane.getViewport().setView( docTextPane );
		docPane.setPreferredSize( new Dimension( (int)(0.5 * screenSize.width ),
				                                 (int)(0.65 * screenSize.height ) ) );

		setLeftComponent( docPane );
		setRightComponent( matchPane );
	}
	
	public void displayDoc( DocInfo info ) {
		DefaultStyledDocument doc = retEngine.getDocument( info );
		docTextPane.setDocument( doc );
		
		setDividerLocation( 0.5 );
	}

	public void addMatches( Vector docs ) {		
		for( int i = 0; i < docs.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)docs.elementAt(i);
			addNewTab( (ScoredDocInfo)docs.elementAt(i) );
		}		
	}
	
	// TODO: clean this up a bit
	private void addNewTab( ScoredDocInfo info ) {
		RecapStyledDocument doc = retEngine.getMarkedDocument( info );
		//JScrollPane scrollPane = new JScrollPane();
		JScrollPane scrollPane = new QuickFindScrollPane( doc.getMatches(), doc.getByteLength() );
		JTextPane textPane = new JTextPane();
		textPane.setDocument( doc );
		textPane.setEditable( false );
		scrollPane.getViewport().setView( textPane );
		matchPane.addTab( info.docName, scrollPane );
	}
		
	public JTabbedPane getMatchPane() {
		return matchPane;
	}

	public JTextPane getDocTextPane() {
		return docTextPane;
	}
	
	public void setSelectedDoc( DocInfo doc ) {
		int numTabs = matchPane.getTabCount();
		for( int i = 0; i < numTabs; i++ ) {
			if( doc.getDocID().equals( matchPane.getTitleAt( i ) ) ) {
				matchPane.setSelectedIndex( i );
				return; // set to FIRST tab that matches the docID
			}
		}
	}
	
	public void clearTabs() {
		matchPane.removeAll();
	}
}
