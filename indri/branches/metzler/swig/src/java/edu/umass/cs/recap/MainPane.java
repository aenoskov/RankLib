package edu.umass.cs.recap;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

/*
 * Created on Sep 28, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
public class MainPane extends JPanel implements ActionListener, MainPaneUpdater {

	private RetrievalEngine retEngine = null;
	private MainMenuBar menu = null;
	
	private InfoPane infoPane = null;
	
	public MainPane( RetrievalEngine retEngine, MainMenuBar menu, Dimension screenSize ) {
		//super( JSplitPane.HORIZONTAL_SPLIT );
		super();
		
		this.retEngine = retEngine;
		this.menu = menu;
		
		menu.addActionListeners( this );
		
		setLayout( new BorderLayout() );
		infoPane = new InfoPane( retEngine, screenSize, this );
		infoPane.setPreferredSize( screenSize );

		add( infoPane );
	}
	
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
		else if( label.equals( "About...") ) {			
			JOptionPane.showMessageDialog( this,
				    "RECAP is the result of a collaborative effort involving:\n\nYaniv Bernstein (RMIT)\nW. Bruce Croft (UMass)\nDonald Metzler (UMass)\nAllistair Moffat (U. of Melbourne)\nJustin Zobel (RMIT)\n\nCode developed by Donald Metzler",
				    "About...",
				    JOptionPane.INFORMATION_MESSAGE,
				    new ImageIcon("edu/umass/cs/recap/images/recap.png"));		
		}
		// update these values each time a menu event occurs for simplicity
		infoPane.setNumExploreResults( menu.getNumExploreResults() );
		infoPane.setNumAnalyzeResults( menu.getNumAnalyzeResults() );
	}	
}
