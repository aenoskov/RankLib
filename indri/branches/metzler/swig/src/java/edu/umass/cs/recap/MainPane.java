package edu.umass.cs.recap;
import java.util.Vector;

import javax.swing.JSplitPane;

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

	private RetrievalEngine retEngine = null;
	
	private InfoPane infoPane = null;
	private DocList docList = null;

	private Vector docIDs = null;
	
	public MainPane( RetrievalEngine retEngine ) {
		super( JSplitPane.HORIZONTAL_SPLIT );
		
		this.retEngine = retEngine;
		
		docIDs = retEngine.getDocIDs();
		
		infoPane = new InfoPane( retEngine, this );
		docList = new DocList( docIDs, this );
		
		setLeftComponent( docList );
		setRightComponent( infoPane );
	}

	public void init() {
		docList.setSelectedIndex( 0 );
	}
			
	public void setSelectedDoc( DocInfo doc ) {
		docList.setSelectedDoc( doc );
	}
	
	public void update( DocInfo doc ) {
		infoPane.displayDoc( doc );
		infoPane.setDividerLocation( 0.8 );
	}	
}
