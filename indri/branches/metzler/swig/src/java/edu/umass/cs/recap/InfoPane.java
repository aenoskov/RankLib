package edu.umass.cs.recap;
import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JTextPane;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import edu.umass.cs.indri.QueryEnvironment;
import edu.umass.cs.indri.ScoredExtentResult;

/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
public class InfoPane extends JSplitPane implements ActionListener, ChangeListener, MouseListener {

	private final double EPSILON = 1E-100;
	
	private QueryEnvironment indri = null;
	
	private DocViewPane dvPane = null;
	private TimelinePanel tlPanel = null;
	private QueryPanel queryPanel = null;
	
	private MainPaneUpdater updater = null;
	
	public InfoPane( QueryEnvironment indri, MainPaneUpdater updater ) {
		super( JSplitPane.VERTICAL_SPLIT );
		this.indri = indri;
		this.updater = updater;
		
		tlPanel = new TimelinePanel();
		dvPane = new DocViewPane( indri );
		// TODO: call indri.fieldList() to populate this list
		queryPanel = new QueryPanel( indri.fieldList() );
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
		
		Vector queries = getQuerySentences( query );
		Vector allScores = new Vector();		

		this.setCursor( new Cursor( Cursor.WAIT_CURSOR ) );
		
		// run a query for each sentence
		for( int queryNum = 0; queryNum < queries.size(); queryNum++ ) {
			String q = (String)queries.elementAt( queryNum );
			ScoredExtentResult [] scores = indri.runQuery( queryOp + queryExtent + "(" + q + ")", 10 );
			allScores.add( scores );
		}

		Vector results = scoreResults( allScores );
		
		for( int i = 0; i < results.size(); i++ )
			System.out.println( results.elementAt(i) );
		
		Collections.sort( results );
		setMetadata( results );

		Vector viewableResults = new Vector();
		for( int i = results.size() - queryPanel.getNumResults(); i < results.size(); i++ )
			viewableResults.add( results.elementAt( i ) );
		
		// update DocViewPane
		dvPane.addMatches( viewableResults );
		
		// update TimelinePanel
		tlPanel.setResults( viewableResults );
		String curDocName = ( (ScoredDocInfo)viewableResults.elementAt( viewableResults.size() - 1 ) ).docName;
		tlPanel.setCurrent( curDocName );

		// update QueryPanel
		queryPanel.setTimelineStartDate( tlPanel.getStartDate() );
		queryPanel.setTimelineEndDate( tlPanel.getEndDate() );
		
		this.setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
	}

	// TODO: put this somewhere more appropriate
	// adds document metadata to the ScoredDocumentInfo
	// objects contained in the docs Vector
	private void setMetadata( Vector docs ) {
		int [] ids = new int[docs.size()];
		
		for( int i = 0; i < docs.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)docs.elementAt( i );
			ids[i] = info.docID;
		}
		
		String [] docNames = indri.documentMetadata( ids, "docno" );
		
		for( int i = 0; i < docs.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)docs.elementAt( i );
			info.docName = docNames[ i ];
			// TODO: is there a better way to do this??
			if( docNames[i].startsWith("WSJ") ) {
				info.date = Integer.parseInt(docNames[i].substring(7,9));
				info.month = Integer.parseInt(docNames[i].substring(5,7));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(3,5));
			}
			else if( docNames[i].startsWith("LA") ) {
				info.date = Integer.parseInt(docNames[i].substring(2,4));
				info.month = Integer.parseInt(docNames[i].substring(4,6));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(6,8));				
			}
			else if( docNames[i].startsWith("AP") ) {
				info.date = Integer.parseInt(docNames[i].substring(6,8));
				info.month = Integer.parseInt(docNames[i].substring(4,6));
				info.year = 1900 + Integer.parseInt(docNames[i].substring(2,4));
			}
			// TODO: fix this to handle FBIS, FT, and SJMN
			else {
				info.date = 1;
				info.month = 1;
				info.year = 1989;
			}
			System.out.println(info);
		}
	}
	
	// tokenizes query string
	// TODO: does this belong here or somewhere else?
	//       maybe make an IndriWrapper / Retrieval class and stick it there
	private Vector getQuerySentences( String in ) {
		Vector ret = new Vector();
		SentenceTokenizer tok = new SentenceTokenizer( in );
		
		while( tok.hasMoreTokens() ) {
			String sentence = parse( tok.nextSentence() );
			System.out.println( "SENTENCE = " + sentence );
			ret.add( sentence );
		}
		
		return ret;
	}
	
	// TODO: this doesn't belong here either
	private String parse( String in ) {
		String ret = new String();
	
		StringTokenizer tok = new StringTokenizer( in, 	" .?!\b\t\n\f\r\'\"\\~`@#$%^&*()-=_+{}|[]:;,/<>" );
		while( tok.hasMoreTokens() )
			ret += tok.nextToken() + " ";
		
		return ret;
	}
	
	// returns a vector of ScoredDocInfo objects
	private Vector scoreResults( Vector scores ) {
		Vector ret = new Vector();

		HashMap candidateDocs = new HashMap();

		// create a hashmap to keep track of candidate documents
		for( int queryNum = 0; queryNum < scores.size(); queryNum++ ) {
			ScoredExtentResult [] queryScores = (ScoredExtentResult [])scores.elementAt( queryNum );
			for( int i = 0; i < queryScores.length; i++ ) {
				ScoredExtentResult r = (ScoredExtentResult)queryScores[i];
				Integer docNum = new Integer( r.document );
				candidateDocs.put( docNum, new ScoredDocInfo( docNum.intValue(), 0.0, new Vector() ) );
			}			
		}
		
		// score the documents
		for( int queryNum = 0; queryNum < scores.size(); queryNum++ ) {
			HashMap results = new HashMap();
			// collect results into a form we can use for scoring
			ScoredExtentResult [] queryScores = (ScoredExtentResult [])scores.elementAt( queryNum );
			for( int i = 0; i < queryScores.length; i++ ) {
				ScoredExtentResult r = (ScoredExtentResult)queryScores[i];
				System.out.println( r.score );
				Integer docNum = new Integer( r.document );
				Vector v = (Vector)results.get( docNum );
				if( v == null )
					v = new Vector();
				v.add( r );
				results.put( docNum, v );
			}
		
			// update each document's score
			Iterator iter = candidateDocs.keySet().iterator();
			while( iter.hasNext() ) {
				Integer i = (Integer)iter.next();
				double score = getDocScore( i , results );
				ScoredDocInfo info = (ScoredDocInfo)candidateDocs.get( i );
				Vector v = (Vector)results.get( i );
				info.score += Math.log( score );
				if( v != null && v.size() > 0 )
					info.extents.addAll( v ); // add all matches
					//info.extents.add( v.elementAt( 0 ) ); // only add the best match
				
			}
		}

		// construct the final ranked list
		Iterator iter = candidateDocs.keySet().iterator();
		while( iter.hasNext() ) {
			Integer i = (Integer)iter.next();
			ScoredDocInfo info = (ScoredDocInfo)candidateDocs.get( i );
			ret.add( info );
		}
		
		return ret;
	}
	
	// scores a single document
	private double getDocScore( Integer i, HashMap results ) {
		double score = 0.0;
		
		// TODO: make this more sophisticated
		// for now, just count the number of matches
		Vector v = (Vector)results.get( i );
		if( v == null || v.size() == 0 )
			score = EPSILON;
		else {
			for( int j = 0; j < v.size(); j++ ) {
				ScoredExtentResult s = (ScoredExtentResult)v.elementAt( j );
				score += Math.exp( 1.0*s.score ) / v.size();
			}
		}
		
		return score;
	}
	
	public void stateChanged( ChangeEvent e ) {		
		if( dvPane != null && dvPane.getMatchPane().getSelectedIndex() != -1 ) {
			String curDoc = dvPane.getMatchPane().getTitleAt( dvPane.getMatchPane().getSelectedIndex() );
			tlPanel.setCurrent( curDoc );
			//rgPanel.getGraph().setSelectedDocID( curDoc );
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
