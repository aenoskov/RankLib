package edu.umass.cs.recap;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.JMenuItem;
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
public class MainPane extends JSplitPane implements ActionListener, MainPaneUpdater {

	private RetrievalEngine retEngine = null;
	private MainMenuBar menu = null;
	
	private InfoPane infoPane = null;
	private DocList docList = null;

	private Vector docIDs = null;
	
	public MainPane( RetrievalEngine retEngine, MainMenuBar menu ) {
		super( JSplitPane.HORIZONTAL_SPLIT );
		
		this.retEngine = retEngine;
		this.menu = menu;
		
		menu.addActionListeners( this );
		
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

	public void actionPerformed(ActionEvent e) {
		JMenuItem item = (JMenuItem)e.getSource();
		String label = item.getLabel();
		if( label.equals( "Increase font size" ) ) {
			retEngine.increaseDocFontSize();
		}
		else if( label.equals( "Decrease font size" ) ) {
			retEngine.decreaseDocFontSize();
		}
		else {
			System.err.println( "UNKNOWN MENU ITEM SELECTED: " + item.getLabel() );
		}
	}	
}
