package edu.umass.cs.recap;
import java.awt.Choice;
import java.awt.GridLayout;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
/*
 * Created on Nov 2, 2004
 *
 */

/**
 * @author Don Metzler
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class QueryPanel extends JPanel {

	private JLabel queryLabel = null;
	private JScrollPane queryScrollPane = null;
	private JTextArea queryTextArea = null;
	private JButton runQueryButton = null;
	private JButton clearQueryButton = null;
	private Choice numResultsChoice = null;
	private Choice simMeasureChoice = null;
	private Choice fieldChoice = null;
	
	public QueryPanel( String [] fields ) {
		queryLabel = new JLabel( "Query:" );
		queryScrollPane = new JScrollPane();
		runQueryButton = new JButton( "Run query" );
		clearQueryButton = new JButton( "Clear query" );
		queryTextArea = new JTextArea( 5, 40 );
		queryTextArea.setLineWrap( true );

		numResultsChoice = new Choice();
		numResultsChoice.add("5");
		numResultsChoice.add("25");
		numResultsChoice.add("50");
		numResultsChoice.add("100");
		
		simMeasureChoice = new Choice();
		simMeasureChoice.add("overlap"); 
		simMeasureChoice.add("identsim");
		simMeasureChoice.add("tf.idf"); 
		simMeasureChoice.add("ql");
		simMeasureChoice.add("mt0"); 

		fieldChoice = new Choice();
		fieldChoice.add( "document" ); // document retrieval
		for( int i = 0; i < fields.length; i++ )
			fieldChoice.add( fields[i] );
		
		// subpanel
		JPanel subPanel = new JPanel();
		subPanel.setLayout( new GridLayout( 4,2 ) );
		subPanel.add( new JLabel( "Method:" ) );
		subPanel.add( simMeasureChoice );
		subPanel.add( new JLabel( "Results:" ) );
		subPanel.add( numResultsChoice );
		subPanel.add( new JLabel( "Field:" ) );
		subPanel.add( fieldChoice );
		subPanel.add( runQueryButton );
		subPanel.add( clearQueryButton );
		
		queryScrollPane.getViewport().setView( queryTextArea );
		
		add( queryLabel );
		add( queryScrollPane );
		//add( runQueryButton );
		add( subPanel );
	}
	
	public String getQueryText() {
		return queryTextArea.getText();
	}
	
	public void setQueryText( String query ) {
		queryTextArea.setText( query );
	}

	public JButton getRunQueryButton() {
		return runQueryButton;
	}

	public JButton getClearQueryButton() {
		return clearQueryButton;
	}

	public Choice getNumResultsChoice() {
		return numResultsChoice;
	}
	
	public String getSimMeasure() {
		String choice = simMeasureChoice.getSelectedItem();
		if( choice.equals( "overlap" ) )
			return "#identsim0";
		else if( choice.equals( "identsim" ) )
			return "#identsim8";
		else if( choice.equals( "tf.idf") )
			return "#identsim10";
		else if( choice.equals( "mt0" ) )
			return "#identsim50";
		return "#combine";
	}
	
	public String getExtent() {
		String choice = fieldChoice.getSelectedItem();
		if( choice.equals( "document") )
			return "";
		return "[" + choice + "]";
	}
	
	public int getNumResults() {
		return Integer.parseInt( numResultsChoice.getSelectedItem() );
	}
}
