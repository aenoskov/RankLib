package edu.umass.cs.recap;
import java.awt.GridLayout;
import java.awt.event.ActionListener;
import java.util.EventListener;
import java.util.Hashtable;

import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JSlider;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.event.ChangeListener;
/*
 * Created on Nov 2, 2004
 *
 */

/**
 * @author Don Metzler
 *
 */
public class QueryPanel extends JPanel {

	private JLabel queryLabel = null;
	private JScrollPane queryScrollPane = null;
	private JTextArea queryTextArea = null;
	private JButton runQueryButton = null;
	private JButton clearQueryButton = null;
	private JRadioButton exploreButton = null;
	private JRadioButton analyzeButton = null;
	
	private JTextField startDate = null;
	private JTextField endDate = null;
	private JButton updateTimelineButton = null;

	private JSlider slider = null;
	
	public QueryPanel() {
		queryLabel = new JLabel( "Query:" );
		queryScrollPane = new JScrollPane();
		runQueryButton = new JButton( "Run query" );
		clearQueryButton = new JButton( "Clear query" );
		queryTextArea = new JTextArea( 5, 40 );
		queryTextArea.setLineWrap( true );
		
		startDate = new JTextField("1/1980");
		endDate = new JTextField("12/2010");
		updateTimelineButton = new JButton( "Update timeline");
		
		// "explore" & "analyze" radio button group
		ButtonGroup group = new ButtonGroup();
		exploreButton = new JRadioButton( "Explore" );
		exploreButton.setSelected( true );
		analyzeButton = new JRadioButton( "Analyze" );
		group.add( exploreButton );
		group.add( analyzeButton );
		
		// subpanel
		JPanel subPanel = new JPanel();
		subPanel.setLayout( new GridLayout( 3, 3) );
		subPanel.add( exploreButton );
		subPanel.add( new JLabel( "Timeline start: ") );
		subPanel.add( startDate );
		subPanel.add( analyzeButton );
		subPanel.add( new JLabel( "Timeline end: ") );
		subPanel.add( endDate );
		subPanel.add( runQueryButton );
		subPanel.add( clearQueryButton );
		subPanel.add( updateTimelineButton );
		
		queryScrollPane.getViewport().setView( queryTextArea );
		
		// slider
		slider = new JSlider( JSlider.HORIZONTAL, 0, 100, 0 );
		
		Hashtable labelTable = new Hashtable();
		labelTable.put( new Integer( 0 ), new JLabel("All") );
		labelTable.put( new Integer( 50 ), new JLabel("Partial") );
		labelTable.put( new Integer( 100 ), new JLabel("Exact") );
		slider.setLabelTable( labelTable );
		
		//slider.setMajorTickSpacing(10);
		//slider.setMinorTickSpacing(1);
		//slider.setPaintTicks(true);
		slider.setPaintLabels(true);
		slider.setVisible( true );
		
		add( queryLabel );
		add( queryScrollPane );
		//add( runQueryButton );
		add( subPanel );
		add( slider );
	}
	
	public String getQueryText() {
		return queryTextArea.getText();
	}
	
	public void setQueryText( String query ) {
		queryTextArea.setText( query );
	}

	public double getThreshold() {
		return ( 1.0*slider.getValue() / slider.getMaximum() );
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
	
	public String getMode() {
		if( exploreButton.isSelected() )
			return "explore";
		else if( analyzeButton.isSelected() )
			return "analyze";
		return null; // if, somehow, neither radio button is selected
	}

	// registers all EventListeners used in this class 
	public void addListeners( EventListener listener ) {
		// action listeners
		runQueryButton.addActionListener( (ActionListener)listener );
		clearQueryButton.addActionListener( (ActionListener)listener );
		updateTimelineButton.addActionListener( (ActionListener)listener );
		
		// change listeners
		slider.addChangeListener( (ChangeListener)listener );
	}

	public void setSliderEnabled( boolean b ) {
		slider.setEnabled( b );
	}
	
}
