package edu.umass.cs.recap;

import java.awt.event.MouseListener;
import java.util.EventListener;
import java.util.HashMap;
import java.util.Vector;

import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.event.ChangeListener;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Document;
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
public class DocViewPane extends JSplitPane {

	private RetrievalEngine retEngine = null;
	
	private JTabbedPane matchPane = null;
	private JTextPane docTextPane = null;
	private JScrollPane docPane = null;
	
	private HashMap curExploreResults = null;
	private Vector curAnalyzeResults = null;
	
	public DocViewPane( RetrievalEngine retEngine ) {
		super( JSplitPane.HORIZONTAL_SPLIT );
		this.retEngine = retEngine;
		this.curExploreResults = new HashMap();
		this.curAnalyzeResults = new Vector();

		docTextPane = new JTextPane();
		matchPane = new JTabbedPane( JTabbedPane.RIGHT, JTabbedPane.SCROLL_TAB_LAYOUT );
		docPane = new JScrollPane();
		docPane.getViewport().setView( docTextPane );

		setLeftComponent( docPane );
		setRightComponent( matchPane );
		
		setResizeWeight( 0.5 );
	}
	
	public void displayDoc( Document doc ) {
		docTextPane.setDocument( doc );
	}
	
	public void displayDoc( ScoredDocInfo info ) {
		DefaultStyledDocument doc = retEngine.getDocument( info );
		docTextPane.setDocument( doc );		
	}
	
	public void displayHighlightedDoc( int docID ) {
		docTextPane.setDocument( (DefaultStyledDocument)curExploreResults.get( new Integer( docID ) ) );
	}

	public void setAnalyzeResults( Vector docs ) {
		curAnalyzeResults.clear();
		for( int i = 0; i < docs.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)docs.elementAt(i);
			addNewTab( info );
		}		
	}
	
	public void setViewableAnalyzeResults( boolean [] b ) {
		for( int i = 0; i < b.length; i++ ) {
			if( b[i] )
				matchPane.setEnabledAt( i, true );
			else
				matchPane.setEnabledAt( i, false );
		}
	}
	
	// TODO: clean this up a bit
	private void addNewTab( ScoredDocInfo info ) {
		RecapStyledDocument doc = retEngine.getMarkedDocument( info );		
		curAnalyzeResults.add( doc );
		JScrollPane scrollPane = new QuickFindScrollPane( doc.getViewableSentenceMatches(), doc.getQueryPositions(), doc.getLength() );
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
		
		for( int i = 0; i < results.length; i++ )
			curExploreResults.put( new Integer( results[i].document ), retEngine.getAnnotatedDocument( results[i].document, annotation ) );
		
		addResultTab( annotation, viewableResults ); //results, viewableResults );
	}
	
	private void addResultTab( QueryAnnotation annotation, Vector viewableResults) {
		JScrollPane resultPane = new JScrollPane();
		JTextPane textPane = new JTextPane();
		textPane.setContentType("text/html");
		HTMLEditorKit kit = new HTMLEditorKit();
		HTMLDocument doc = (HTMLDocument)kit.createDefaultDocument();
		setResults( doc, annotation, viewableResults );
		textPane.setDocument( doc );
		textPane.setEditable( false );
		resultPane.getViewport().setView( textPane );
		matchPane.addTab( "Results", resultPane );
	}

	private void setResults( HTMLDocument doc, QueryAnnotation annotation, Vector viewableResults ) {
		HTMLEditorKit htmlKit = new HTMLEditorKit();		

		//Map matches = annotation.getAnnotations();		
		QueryAnnotationNode root = annotation.getQueryTree();
		String queryText = "<h2>Query:</h2><pre>" + root.queryText + "</pre><br><hr>";
		
		try {
			htmlKit.insertHTML( doc, doc.getLength(), queryText, 0, 0, null );
		}
		catch( Exception e ) { e.printStackTrace(); }
				
		ScoredExtentResult [] results = annotation.getResults();
		for( int i = 0; i < results.length; i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)viewableResults.elementAt(i);
			String url = info.docName + ":document:" + info.docID;
			String title = "<h2><b>" + (i+1) + ". <a href=\"" + url + "\">" + info.docName + "</a></h2>";
			String docinfo = "<i>Extent</i>: <font color=#ff0000><b><a href=\"" + info.docName + ":extent:" + info.docID + ":" + results[i].begin + ":" + results[i].end + "\">[" + results[i].begin + "," + results[i].end + "]</a></b></font><br>";
					
			try {
				htmlKit.insertHTML( doc, doc.getLength(), title, 0, 0, null );
				htmlKit.insertHTML( doc, doc.getLength(), generateHTMLSnippet( results[i].document ), 0, 0, null );
				htmlKit.insertHTML( doc, doc.getLength(), docinfo, 0, 0, null );
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
	
	public Vector getCurAnalyzeResults() {
		return curAnalyzeResults;
	}
	
	public JTabbedPane getMatchPane() {
		return matchPane;
	}

	public JTextPane getResultPane() {
		if( matchPane != null && matchPane.getSelectedComponent() != null )
			return (JTextPane)((JScrollPane)matchPane.getSelectedComponent()).getViewport().getView();
		return null;
	}

	public QuickFindScrollPane getQuickFindScrollPane() {
		return (QuickFindScrollPane)matchPane.getSelectedComponent();		
	}
	
	public JTextPane getDocTextPane() {
		return docTextPane;
	}	
	
	public void setSelectedDoc( ScoredDocInfo doc ) {
		int numTabs = matchPane.getTabCount();
		for( int i = 0; i < numTabs; i++ ) {
			if( doc.docName.equals( matchPane.getTitleAt( i ) ) ) {
				matchPane.setSelectedIndex( i );
				return; // set to FIRST tab that matches the docID
			}
		}
	}
	
	public void clearTabs() {
		matchPane.removeAll();
	}

	// registers EventListeners for this class
	public void addListeners( EventListener pane) {
		docTextPane.addMouseListener( (MouseListener)pane );
		matchPane.addChangeListener( (ChangeListener)pane );
	}
}
