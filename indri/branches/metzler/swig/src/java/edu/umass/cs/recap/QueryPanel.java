package edu.umass.cs.recap;
import java.awt.Choice;
import java.awt.GridLayout;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
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
	private Choice combineMethodChoice = null;
	private Choice fieldChoice = null;
	
	private JTextField startDate = null;
	private JTextField endDate = null;
	private JButton updateTimelineButton = null;
	
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

		combineMethodChoice = new Choice();
		combineMethodChoice.add("none");
		combineMethodChoice.add("prob");
		combineMethodChoice.add("sequence");

		fieldChoice = new Choice();
		fieldChoice.add( "document" ); // document retrieval
		for( int i = 0; i < fields.length; i++ )
			fieldChoice.add( fields[i] );
		
		startDate = new JTextField("1/1980");
		endDate = new JTextField("12/2010");
		updateTimelineButton = new JButton( "Update timeline");
		
		// subpanel
		JPanel subPanel = new JPanel();
		subPanel.setLayout( new GridLayout( 5,3 ) );
		subPanel.add( new JLabel( "Similarity:" ) );
		subPanel.add( simMeasureChoice );
		subPanel.add( new JLabel( "Timeline start: ") );
		subPanel.add( new JLabel( "Combiner:" ) );
		subPanel.add( combineMethodChoice );
		subPanel.add( startDate );
		subPanel.add( new JLabel( "Results:" ) );
		subPanel.add( numResultsChoice );
		subPanel.add( new JLabel( "Timeline end: ") );
		subPanel.add( new JLabel( "Field:" ) );
		subPanel.add( fieldChoice );
		subPanel.add( endDate );
		subPanel.add( runQueryButton );
		subPanel.add( clearQueryButton );
		subPanel.add( updateTimelineButton );
		
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

	public JButton getUpdateTimelineButton() {
		return updateTimelineButton;
	}
	
	public String getSimMeasure() {
		String choice = simMeasureChoice.getSelectedItem();
		if( choice.equals( "overlap" ) )
			return "#identsim0";
		else if( choice.equals( "identsim" ) )
			return "#identsim1";
		else if( choice.equals( "tf.idf") )
			return "#identsim10";
		else if( choice.equals( "mt0" ) )
			return "#identsim50";
		return "#combine";
	}
	
	public String getExtent() {
		return fieldChoice.getSelectedItem();
	}
	
	public String getCombiner() {
		return combineMethodChoice.getSelectedItem();
	}
	
	public int getNumResults() {
		return Integer.parseInt( numResultsChoice.getSelectedItem() );
	}

	public String getTimelineStartDate() {
		return startDate.getText();
	}
	
	public String getTimelineEndDate() {
		return endDate.getText();
	}
	
	public void setTimelineStartDate( String start ) {
		startDate.setText( start );
	}
	
	public void setTimelineEndDate( String end ) {
		endDate.setText( end );
	}
}
