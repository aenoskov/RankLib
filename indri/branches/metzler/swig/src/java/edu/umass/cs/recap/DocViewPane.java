package edu.umass.cs.recap;
import java.awt.Dimension;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.html.HTMLDocument;
import javax.swing.text.html.HTMLEditorKit;

import edu.umass.cs.indri.QueryAnnotation;
import edu.umass.cs.indri.QueryAnnotationNode;
import edu.umass.cs.indri.ScoredExtentResult;

/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
public class DocViewPane extends JSplitPane implements HyperlinkListener {

	private RetrievalEngine retEngine = null;
	
	private JTabbedPane matchPane = null;
	private JTextPane docTextPane = null;
	private JScrollPane docPane = null;
	
	private HashMap curExploreResults = null;
	
	public DocViewPane( RetrievalEngine retEngine, Dimension screenSize ) {
		super( JSplitPane.HORIZONTAL_SPLIT );
		this.retEngine = retEngine;
		this.curExploreResults = new HashMap();

		docTextPane = new JTextPane();		
		matchPane = new JTabbedPane( JTabbedPane.TOP, JTabbedPane.SCROLL_TAB_LAYOUT );
		docPane = new JScrollPane();
		docPane.getViewport().setView( docTextPane );

		setLeftComponent( docPane );
		setRightComponent( matchPane );
		
		setResizeWeight( 0.5 );
	}
	
	public void displayDoc( DocInfo info ) {
		DefaultStyledDocument doc = retEngine.getDocument( info );
		docTextPane.setDocument( doc );		
	}
	
	public void displayHighlightedDoc( int docID ) {
		docTextPane.setDocument( (DefaultStyledDocument)curExploreResults.get( new Integer( docID ) ) );
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
	
	// displays the results of an "Explore" query
	public void displayExplorationResults( QueryAnnotation annotation, Vector viewableResults ) {
		curExploreResults.clear();
		ScoredExtentResult [] results = annotation.getResults();
		QueryAnnotationNode root = annotation.getQueryTree();
		Map matches = annotation.getAnnotations();
		
		for( int i = 0; i < results.length; i++ )
			curExploreResults.put( new Integer( results[i].document ), retEngine.getAnnotatedDocument( results[i].document, annotation ) );
		
		addResultTab( results, viewableResults );
	}
	
	private void addResultTab( ScoredExtentResult [] results, Vector viewableResults ) {
		JScrollPane resultPane = new JScrollPane();
		JTextPane textPane = new JTextPane();
		textPane.setContentType("text/html");
		HTMLEditorKit kit = new HTMLEditorKit();
		HTMLDocument doc = (HTMLDocument)kit.createDefaultDocument();
		setResults( doc, results, viewableResults );
		textPane.setDocument( doc );
		textPane.setEditable( false );
		textPane.addHyperlinkListener( this ); // listen for hyperlink clicks
		resultPane.getViewport().setView( textPane );
		matchPane.addTab( "Results", resultPane );
	}

	private void setResults( HTMLDocument doc, ScoredExtentResult [] results, Vector viewableResults ) {
		HTMLEditorKit htmlKit = new HTMLEditorKit();
		
		for( int i = 0; i < results.length; i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)viewableResults.elementAt(i);
			String url = info.docName + ":" + info.docID;
			String title = "<h2>" + (i+1) + ". <a href=\"" + url + "\">" + info.docName + "</a></h2>";
					
			try {
				htmlKit.insertHTML( doc, doc.getLength(), title, 0, 0, null );
				htmlKit.insertHTML( doc, doc.getLength(), generateHTMLSnippet( results[i].document ), 0, 0, null );
			}
			catch( Exception e ) { e.printStackTrace(); }
		}		
	}
	
	private String generateHTMLSnippet( int docID ) {
		DefaultStyledDocument doc = (DefaultStyledDocument)curExploreResults.get( new Integer( docID) );
		String snippet = new String("Snippet not available.");
		
		// TODO: make this better!
		try {
			String docText = doc.getText( 0, doc.getLength() );
			int textStart = docText.indexOf("<TEXT>");
			if( textStart != -1 )
				snippet = doc.getText(textStart+6, 250);
			else
				snippet = doc.getText(0, 250 );
		}
		catch(Exception e) {
			snippet = "Snippet not available.";
		}			
		
		return snippet + "...<br>";
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
	
	// process a hyperlink click
	public void hyperlinkUpdate(HyperlinkEvent e) {
		if( e.getEventType() == HyperlinkEvent.EventType.ACTIVATED ) {
			StringTokenizer tok = new StringTokenizer( e.getDescription(), ":" );
			String docName = tok.nextToken();
			int docID = Integer.parseInt( tok.nextToken() );
			displayHighlightedDoc( docID );
			setDividerLocation( 0.5 );
		}
	}
}
