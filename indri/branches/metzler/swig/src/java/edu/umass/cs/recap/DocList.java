package edu.umass.cs.recap;
import java.util.Collections;
import java.util.Vector;

import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
/*
 * Created on Sep 28, 2004
 *
 * DocList Class
 *
 */

/**
 * @author Don Metzler
 *
 * Presents a list of documents that the user may choose
 * choose to analyze in further detail. 
 * 
 */
public class DocList extends JScrollPane implements ListSelectionListener {

	private MainPaneUpdater updater = null;
	private JList docList = null;
	private Vector docIDs = null;
	
	public DocList( Vector docIDs, MainPaneUpdater updater ) {
		this.docIDs = docIDs;
		this.updater = updater;
				
		// sort the docs by id to
		// allow fast lookups
		Collections.sort( docIDs ); // sort the docs by id
		docList = new JList( docIDs );
		docList.addListSelectionListener( this );
		getViewport().setView( docList );		
	}
	
	public void setSelectedDoc( DocInfo doc ) {
		int newIndex = Collections.binarySearch( docIDs, doc );
		if( newIndex >= 0 ) {
			docList.ensureIndexIsVisible( newIndex );
			docList.setSelectedIndex( newIndex );
		}
	}
	
	public void setSelectedIndex( int ind ) {
		docList.setSelectedIndex( ind );
	}

	public void valueChanged( ListSelectionEvent e ) {
		updater.update( (DocInfo)docList.getSelectedValue() );
	}
}
