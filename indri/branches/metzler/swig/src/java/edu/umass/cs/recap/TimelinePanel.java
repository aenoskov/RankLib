package edu.umass.cs.recap;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Shape;
import java.awt.geom.Ellipse2D;
import java.util.Vector;

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
	
	private double maxScore = Double.MIN_VALUE;
	
	public TimelinePanel () {
		ovals = new Vector();
	}
	
	public void setResults( Vector res ) {
		this.results = res;
		init();
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
	
	private void init() {
		minYear = Integer.MAX_VALUE;
		minDate = 32;
		minMonth = 13;
		
		maxYear = Integer.MIN_VALUE;
		maxDate = 0;
		maxMonth = 0;
		
		maxScore = -100000.0;
		
		for( int i = 0; i < results.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			int curMonth = info.month;
			int curDate = info.date;
			int curYear = info.year;
			if( curYear < minYear ||
				( curYear == minYear &&
				( curMonth < minMonth ||
				( curMonth == minMonth && curDate < minDate ) ) ) ) { 
				minMonth = curMonth;
				minDate = curDate;
				minYear = curYear;
			}
			if( curYear > maxYear ||
				( curYear == maxYear &&
				( curMonth > maxMonth ||
				( curMonth == maxMonth && curDate > maxDate ) ) ) ) { 
				maxMonth = curMonth;
				maxDate = curDate;
				maxYear = curYear;
			}
			//System.out.println("score="+info.score+",maxScore="+maxScore);
			if( info.score > maxScore )
				maxScore = info.score;
		}
				
		//System.out.println("minYear="+minYear+",maxYear="+maxYear+",maxScore="+maxScore);
	}
	
	// actually draw the timeline
	protected void paintComponent(Graphics g) {
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
		int numMonths = 12 * ( maxYear - minYear ) - minMonth + maxMonth + 1;
		int monthWidth = (int)( ( width - 2.0*HORIZONTAL_INSET ) / ( 1.0*numMonths ) );
		int dayWidth = (int)Math.ceil( monthWidth / 31.0 );
		
		// draw documents on axis
		for( int i = 0; i < results.size(); i++ ) {
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			
			// find where we should put this document
			int monthOffset = 12 * ( info.year - minYear ) - minMonth + info.month;
			int xPos = HORIZONTAL_INSET + monthOffset*monthWidth + (info.date - 1)*dayWidth;
		
			int size = MIN_SIZE + (int)(( MAX_SIZE - MIN_SIZE )* ( Math.exp( info.score ) / Math.exp( maxScore ) ) );

			// add oval for this document
			ovals.add( new Ellipse2D.Double( xPos-size, midY-size, 2*size, 2*size ) );

			// make sure we're actually on the screen
			if( xPos-size < HORIZONTAL_INSET || xPos+size > width - HORIZONTAL_INSET )
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
		int xPos = HORIZONTAL_INSET;
		for( int curMonth = 0; curMonth <= numMonths; curMonth++ ) {
			g.setColor( Color.black );
			if( ( minMonth + curMonth - 1 ) % 12 == 0 ) {
				g.drawLine( xPos, midY-YEAR_TICK_SIZE, xPos, midY+YEAR_TICK_SIZE );
				g.setColor( Color.blue );
				g.drawString(""+curYear, xPos-15, midY-YEAR_TICK_SIZE);
				curYear++;
			}
			else
				g.drawLine( xPos, midY-MONTH_TICK_SIZE, xPos, midY+MONTH_TICK_SIZE );
			xPos += monthWidth;
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
		return minMonth + "/" + minDate + "/" + minYear;  
	}
	
	public void setStartDate( int month, int year ) {
		minMonth = month;		
		minYear = year;
	}
	
	public String getEndDate() {
		return maxMonth + "/" + maxDate + "/" + maxYear;  
	}
	
	public void setEndDate( int month, int year ) {
		maxMonth = month;
		maxYear = year;
	}
}
