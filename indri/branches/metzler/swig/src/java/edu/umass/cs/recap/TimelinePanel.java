package edu.umass.cs.recap;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Shape;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.geom.Ellipse2D;
import java.util.EventListener;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
/*
 * Created on Nov 9, 2004
 *
 */

/**
 * @author Don Metzler
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class TimelinePanel extends JPanel {

	// display formatting params
	private final int HORIZONTAL_INSET = 25;
	private final int YEAR_TICK_SIZE = 20;
	private final int MONTH_TICK_SIZE = 10;
	private final int MIN_SIZE = 3;
	private final int MAX_SIZE = 8;
	
	// Vector of ScoredDocInfo objects to display
	private Vector results = null;

	// Vector of oval's corresponding to a
	// document's on-screen position
	private Vector ovals = null;
	
	// Currently selected document
	private ScoredDocInfo currentInfo = null;
	
	private int minYear = Integer.MAX_VALUE;
	private int minDate = 31;
	private int minMonth = 12;
	
	private int maxYear = Integer.MIN_VALUE;
	private int maxDate = 1;
	private int maxMonth = 1;
	
	private double minScore = Double.POSITIVE_INFINITY;
	private double maxScore = Double.NEGATIVE_INFINITY;
	
	// previous, next and analyze buttons
	private JButton previousDocButton = null;
	private JButton nextDocButton = null;
	private JButton analyzeButton = null;

	// which documents are actually viewable
	// if this variable is null then all documents are viewable
	private boolean [] viewable = null;
		
	public TimelinePanel () {
		ovals = new Vector();
		
		ImageIcon nextButtonIcon = new ImageIcon("edu/umass/cs/recap/images/next.gif");
		ImageIcon previousButtonIcon = new ImageIcon("edu/umass/cs/recap/images/previous.gif");
		ImageIcon analyzeButtonIcon = new ImageIcon("edu/umass/cs/recap/images/magnify.jpg");
		previousDocButton = new JButton( previousButtonIcon );		
		previousDocButton.setMargin( new Insets(0,0,0,0) );
		previousDocButton.setName("tlPrev");
		nextDocButton = new JButton( nextButtonIcon );
		nextDocButton.setMargin( new Insets(0,0,0,0) );
		nextDocButton.setName("tlNext");
		analyzeButton = new JButton( analyzeButtonIcon );
		analyzeButton.setMargin( new Insets(0,0,0,0) );
		analyzeButton.setSize( 10,10 );
		analyzeButton.setName("tlAnalyze");
		add( previousDocButton );
		add( analyzeButton );
		add( nextDocButton );
		
		// set tool tip text
		previousDocButton.setToolTipText( "Move to previous document");
		analyzeButton.setToolTipText( "Analyze current document");
		nextDocButton.setToolTipText( "Move to next document");
	}
		
	public void setResults( Vector res ) {
		this.viewable = null;
		this.results = res;
		init();
	}

	public void setViewableResults( boolean [] b ) {
		this.viewable = b;
	}
	
	public void setCurrent( String docName ) {
		if( results == null )
			return;
		for( int i = 0; i < results.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			if( info.docName.equals(docName)) {
				currentInfo = info;
				break;
			}
		}
	}

	public ScoredDocInfo getCurrentDoc() {
		return currentInfo;
	}
	
	public ScoredDocInfo getNextDoc() {
		int best = Integer.MAX_VALUE;
		ScoredDocInfo bestInfo = null;
		if( currentInfo == null )
			return null;
		int curDays = dateToInt( currentInfo.year, currentInfo.month, currentInfo.date );
		for( int i = 0; i < results.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			if( info.docID == currentInfo.docID || ( viewable != null && !viewable[i] ) )
				continue;
			int tmpDays = dateToInt( info.year, info.month, info.date );
			
			if( ( tmpDays >= curDays ) && 
				( tmpDays != curDays || info.docID > currentInfo.docID ) &&
			    ( tmpDays - curDays < best || ( tmpDays - curDays == best && info.docID < bestInfo.docID ) ) ) {
				best = tmpDays - curDays;
				bestInfo = info;
			}
		}
		
		return bestInfo;
	}

	public ScoredDocInfo getPreviousDoc() {
		int best = Integer.MAX_VALUE;
		ScoredDocInfo bestInfo = null;
		if( currentInfo == null ) 
			return null;
		int curDays = dateToInt( currentInfo.year, currentInfo.month, currentInfo.date );
		for( int i = 0; i < results.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			if( info.docID == currentInfo.docID || ( viewable != null && !viewable[i] ) )
				continue;
			int tmpDays = dateToInt( info.year, info.month, info.date );
			if( ( curDays >= tmpDays ) && 
				( tmpDays != curDays || info.docID < currentInfo.docID ) &&
			    ( curDays - tmpDays < best || ( curDays - tmpDays == best && info.docID > bestInfo.docID ) ) ) {
				best = curDays - tmpDays;
				bestInfo = info;
			}
		}
		
		return bestInfo;
	}

		
	private int dateToInt( int year, int month, int date ) {
		return year*12*31 + month*31 + date;
	}
		
	private void init() {
		minYear = Integer.MAX_VALUE;
		minDate = 32;
		minMonth = 13;
		
		maxYear = Integer.MIN_VALUE;
		maxDate = 0;
		maxMonth = 0;
		
		minScore = Double.POSITIVE_INFINITY;
		maxScore = Double.NEGATIVE_INFINITY;
		
		int minDays = Integer.MAX_VALUE;
		int maxDays = Integer.MIN_VALUE;
		for( int i = 0; i < results.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			int curDays = dateToInt( info.year, info.month, info.date );
			if( curDays < minDays ) {
				minMonth = info.month;
				minDate = info.date;
				minYear = info.year;
				minDays = curDays;
			}
			if( curDays > maxDays ) {
				maxMonth = info.month;
				maxDate = info.date;
				maxYear = info.year;
				maxDays = curDays;
			}
			if( info.score > maxScore )
				maxScore = info.score;
			if( info.score < minScore )
				minScore = info.score;
		}
		
		maxMonth++;
	}
	
	// actually draw the timeline
	protected void paintComponent(Graphics g) {
		super.paintComponent( g );
		if( results == null )
			return;

		// clear out the ovals
		ovals.clear();
		
		int width = getWidth();
		int height = getHeight();
		int midY = (int)(1.0*height/2.0);
	
		// draw main horizontal axis
		g.drawLine( HORIZONTAL_INSET, midY,
				    width-HORIZONTAL_INSET, midY);
		
		// width that each type of segment takes up
		int numMonths = 12 * ( maxYear - minYear ) - minMonth + maxMonth;
		double monthWidth = ( width - 2.0*HORIZONTAL_INSET ) / ( 1.0*numMonths );
		double dayWidth = monthWidth / 31.0 ;
		
		// draw documents on axis
		for( int i = 0; i < results.size(); i++ ) {			
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			
			// find where we should put this document
			int monthOffset = 12 * ( info.year - minYear ) - minMonth + info.month;
			int xPos = HORIZONTAL_INSET + (int)( monthOffset*monthWidth ) + (int)( (info.date - 1)*dayWidth );
		
			//int size = MIN_SIZE + (int)(( MAX_SIZE - MIN_SIZE )* ( Math.exp( info.score ) / Math.exp( maxScore ) ) );
			int size = MAX_SIZE;

			// ensure that size(minScore) = MIN_SIZE and size(maxScore) = MAX_SIZE
			if( maxScore != minScore ) {
				double b = ( maxScore*MIN_SIZE - minScore*MAX_SIZE ) / ( maxScore - minScore );
				double m = ( MAX_SIZE - MIN_SIZE ) / ( maxScore - minScore );
				size = (int)Math.ceil( m*info.score + b );
			}			

			// add oval for this document
			ovals.add( new Ellipse2D.Double( xPos-size, midY-size, 2*size, 2*size ) );

			// make sure this document is viewable
			if( viewable != null && !viewable[i] )
				continue;

			// make sure we're actually on the screen
			if( xPos < HORIZONTAL_INSET || xPos > width - HORIZONTAL_INSET )
				continue;
			
			if( info == currentInfo ) {
				// TODO: make this into a "drawDoc" function
				g.setColor( new Color( 0.0f, 1.0f, 0.0f, 1.0f ) );
				g.fillOval(xPos-size, midY-size, 2*size, 2*size);
				g.setColor( new Color( 0.0f, 1.0f, 0.0f, 1.0f ) );
				g.drawOval(xPos-size, midY-size, 2*size, 2*size);
			}
			else {
				g.setColor( new Color( 1.0f, 0.0f, 0.0f, 0.50f ) );
				g.fillOval(xPos-size, midY-size, 2*size, 2*size);
				g.setColor( new Color( 1.0f, 0.0f, 0.0f, 1.0f ) );
				g.drawOval(xPos-size, midY-size, 2*size, 2*size);
			}
		}
		
		// draw tickmarks for every year / month
		int curYear = minYear + 1;
		if( minMonth == 1 ) // the first tick we draw will be a year tick
			curYear = minYear;
		for( int curMonth = 0; curMonth <= numMonths; curMonth++ ) {
			g.setColor( Color.black );
			int xPos = HORIZONTAL_INSET + (int)( curMonth*monthWidth ); 
			if( ( minMonth + curMonth - 1 ) % 12 == 0 ) {
				g.drawLine( xPos, midY-YEAR_TICK_SIZE, xPos, midY+YEAR_TICK_SIZE );
				g.setColor( Color.blue );
				g.drawString(""+curYear, xPos-15, midY-YEAR_TICK_SIZE);
				curYear++;
			}
			else
				g.drawLine( xPos, midY-MONTH_TICK_SIZE, xPos, midY+MONTH_TICK_SIZE );
		}
 	}
	
	public ScoredDocInfo getDocAt( Point p ) {
		ScoredDocInfo ret = null;
		for( int i = 0; i < ovals.size(); i++ ) {
			Shape s = (Ellipse2D.Double)ovals.elementAt( i );
			if( s.contains( p ) )
				return (ScoredDocInfo)results.elementAt( i );
		}
		return ret;
	}
	
	public String getStartDate() {
		return minMonth + "/" + minYear;  
	}
	
	public void setStartDate( int month, int year ) {
		minMonth = month;		
		minYear = year;
	}
	
	public String getEndDate() {
		return maxMonth + "/" + maxYear;  
	}
	
	public void setEndDate( int month, int year ) {
		maxMonth = month;
		maxYear = year;
	}
	
	// register EventListeners for this class
	public void addListeners( EventListener listener ) {
		previousDocButton.addActionListener( (ActionListener)listener );
		nextDocButton.addActionListener( (ActionListener)listener );
		analyzeButton.addActionListener( (ActionListener)listener );
		
		addMouseListener( (MouseListener)listener );
	}

}
