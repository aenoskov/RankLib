package edu.umass.cs.recap;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JMenuItem;
import javax.swing.JPanel;

/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
//public class MainPane extends JSplitPane implements ActionListener, MainPaneUpdater {
public class MainPane extends JPanel implements ActionListener, MainPaneUpdater {

	private RetrievalEngine retEngine = null;
	private MainMenuBar menu = null;
	
	private InfoPane infoPane = null;
	//private DocList docList = null;

	//private Vector docIDs = null;
	
	public MainPane( RetrievalEngine retEngine, MainMenuBar menu, Dimension screenSize ) {
		//super( JSplitPane.HORIZONTAL_SPLIT );
		super();
		
		this.retEngine = retEngine;
		this.menu = menu;
		
		menu.addActionListeners( this );
		
		//docIDs = retEngine.getDocIDs();
		setLayout( new BorderLayout() );
		infoPane = new InfoPane( retEngine, screenSize, this );
		infoPane.setPreferredSize( screenSize );
		//infoPane.setSize( getWidth(), getHeight() );
		//docList = new DocList( docIDs, this );
		
		//setLeftComponent( null );
		//setRightComponent( infoPane );
		//infoPane.setSize( getWidth(), (int)(getHeight()*0.5) );
		add( infoPane );
	}

	/*public void init() {
		docList.setSelectedIndex( 0 );
	}
			
	public void setSelectedDoc( DocInfo doc ) {
		docList.setSelectedDoc( doc );
	}*/
	
	public void update( DocInfo doc ) {
		infoPane.displayDoc( doc );
		infoPane.setDividerLocation( 0.8 );
	}

	public void actionPerformed(ActionEvent e) {
		JMenuItem item = (JMenuItem)e.getSource();
		String label = item.getLabel();
		// TODO: make this more robust (i.e. don't rely on menu labels)
		if( label.equals( "Increase font size" ) ) {
			retEngine.increaseDocFontSize();
		}
		else if( label.equals( "Decrease font size" ) ) {
			retEngine.decreaseDocFontSize();
		}
		else if( label.equals( "IDF-based Word Overlap" ) ) {
			infoPane.setSimMeasure( "#identsim1" );
		}
		else if( label.equals( "Word Overlap" ) ) {
			infoPane.setSimMeasure( "#identsim0" );
		}
		else if( label.equals( "Machine Translation (Model 0)" ) ) {
			infoPane.setSimMeasure( "#identsim50" );
		}
		else if( label.equals( "Query likelihood" ) ) {
			infoPane.setSimMeasure( "#combine" );
		}
		else if( label.equals( "5" ) ) {
			infoPane.setNumResults( 5 );
		}
		else if( label.equals( "10" ) ) {
			infoPane.setNumResults( 10 );
		}
		else if( label.equals( "25" ) ) {
			infoPane.setNumResults( 25 );
		}
		else if( label.equals( "50" ) ) {
			infoPane.setNumResults( 50 );
		}
		else if( label.equals( "100" ) ) {
			infoPane.setNumResults( 100 );
		}
		else if( label.equals( "Exit" ) ) {
			System.exit(0);
		}
		else {
			System.err.println( "UNKNOWN MENU ITEM SELECTED: " + item.getLabel() );
		}
	}	
}
