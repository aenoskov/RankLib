package edu.umass.cs.recap;
import java.util.Vector;

import javax.swing.JSplitPane;

import edu.umass.cs.indri.QueryEnvironment;

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
public class MainPane extends JSplitPane implements MainPaneUpdater {

	private QueryEnvironment indri = null;
	
	private InfoPane infoPane = null;
	private DocList docList = null;

	private Vector docIDs = null;
	
	public MainPane( QueryEnvironment indri ) {
		super( JSplitPane.HORIZONTAL_SPLIT );
		
		this.indri = indri;
		
		docIDs = getDocIDs();
		
		infoPane = new InfoPane( indri, this );
		docList = new DocList( docIDs, this );
		
		setLeftComponent( docList );
		setRightComponent( infoPane );
	}

	public void init() {
		docList.setSelectedIndex( 0 );
	}
	
	private Vector getDocIDs() {
		Vector ret = new Vector();

		// TODO: make this work for collections
		//       with numDocs > MAX_INT
		int numDocs = (int)indri.documentCount();
		
		int [] ids = new int[ numDocs ];
		for( int i = 0; i < numDocs; i++ )
			ids[i] = i+1;
		
		String [] docIDs = indri.documentMetadata( ids, "docno" );
		
		for( int i = 0; i < numDocs; i++ )
			ret.add( new DocInfo( docIDs[i], i+1 ) );
		
		return ret;
	}
		
	public void setSelectedDoc( DocInfo doc ) {
		docList.setSelectedDoc( doc );
	}
	
	public void update( DocInfo doc ) {
		infoPane.displayDoc( doc );
		infoPane.setDividerLocation( 0.8 );
	}	
}
