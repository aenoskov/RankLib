package edu.umass.cs.recap;
import java.awt.Color;
import java.util.Vector;

import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;

import edu.umass.cs.indri.ParsedDocument;
import edu.umass.cs.indri.QueryEnvironment;
import edu.umass.cs.indri.ScoredExtentResult;

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

	private QueryEnvironment indri = null;
	
	private JTabbedPane matchPane = null;
	private JTextPane docTextPane = null;
	private JScrollPane docPane = null;
	
	public DocViewPane( QueryEnvironment indri ) {
		super( JSplitPane.HORIZONTAL_SPLIT );
		this.indri = indri;

		docTextPane = new JTextPane();
		matchPane = new JTabbedPane( JTabbedPane.TOP, JTabbedPane.SCROLL_TAB_LAYOUT );
		docPane = new JScrollPane();
		docPane.getViewport().setView( docTextPane );

		setLeftComponent( docPane );
		setRightComponent( matchPane );
	}
	
	public void displayDoc( DocInfo info ) {
		// clear the tabs and relationship graph
		//matchPane.removeAll();
		//relPaneUpdater.clear();

		DefaultStyledDocument doc = getDocument( info );
		docTextPane.setDocument( doc );
		
		// add main document tab
		//addNewTab( info );
		this.setDividerLocation( 0.5 );
	}

	public void addMatches( Vector docs ) {		
		for( int i = docs.size()-1; i >= 0; i-- ) {
			ScoredDocInfo info = (ScoredDocInfo)docs.elementAt(i);
			addNewTab( (ScoredDocInfo)docs.elementAt(i) );
		}		
	}
	
	// TODO: clean this up a bit
	private void addNewTab( ScoredDocInfo info ) {
		RecapStyledDocument doc = getMarkedDocument( info );
		//JScrollPane scrollPane = new JScrollPane();
		JScrollPane scrollPane = new QuickFindScrollPane( doc.getMatches(), doc.getByteLength() );
		JTextPane textPane = new JTextPane();
		textPane.setDocument( doc );
		textPane.setEditable( false );
		scrollPane.getViewport().setView( textPane );
		matchPane.addTab( info.docName, scrollPane );
	}
	
	private DefaultStyledDocument getDocument( DocInfo info ) {
		DefaultStyledDocument doc = new DefaultStyledDocument();
		
		// TODO: clean this up a bit
		int [] id = new int[1];
		id[0] = info.getDocNum();
		ParsedDocument [] docs = indri.documents( id );
		try {
			doc.insertString( 0, docs[0].text , null );
		}
		catch( Exception e ) { /* do nothing */ }
				
		return doc;
	}
	
	private RecapStyledDocument getMarkedDocument( ScoredDocInfo info ) {
		RecapStyledDocument doc = new RecapStyledDocument();
		
		// TODO: clean this up a bit
		int [] id = new int[1];
		id[0] = info.docID;
		ParsedDocument [] docs = indri.documents( id );
		try {
			doc.insertString( 0, docs[0].text , null );
		}
		catch( Exception e ) { /* do nothing */ }

		Style defaultStyle = StyleContext.getDefaultStyleContext().getStyle(StyleContext.DEFAULT_STYLE);
		Style s = doc.addStyle( "match", defaultStyle );
		StyleConstants.setForeground( s, Color.blue );
		StyleConstants.setItalic( s, true );
        
		for( int i = 0; i < info.extents.size(); i++ ) {
			ScoredExtentResult extent = (ScoredExtentResult)info.extents.elementAt( i );
			int extentBegin = docs[0].positions[extent.begin].begin;
			int extentEnd = docs[0].positions[extent.end - 1].end;

			doc.addMatch( extentBegin, extentEnd );
			
			try {
				doc.replace( extentBegin, extentEnd - extentBegin,
					     	 doc.getText(extentBegin, extentEnd-extentBegin),
							 doc.getStyle("match") );
			}
			catch(Exception e) { /* do nothing */ }
		}
		
		doc.setByteLength( docs[0].text.length() );
		
		return doc;
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
