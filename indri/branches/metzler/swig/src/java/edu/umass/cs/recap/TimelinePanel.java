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
		int yearWidth = (int)( ( width - 2.0*HORIZONTAL_INSET ) / ( maxYear - minYear + 1.0 ) );
		//int monthWidth = (int)Math.ceil( yearWidth / 12.0 );
		//int dayWidth = (int)Math.ceil( monthWidth / 31.0 );
		
		// draw documents on axis
		for( int i = 0; i < results.size(); i++ ) {
			int xPos = HORIZONTAL_INSET;
			ScoredDocInfo info = (ScoredDocInfo)results.elementAt( i );
			
			// find where we should put this document
			xPos += (info.year - minYear)*yearWidth;
			xPos += (int)(info.month-1)*( yearWidth / 12.0 );
			xPos += (int)(info.date-1)*( yearWidth / (12.0 * 31.0 ) );
			
			int size = MIN_SIZE + (int)(( MAX_SIZE - MIN_SIZE )* ( Math.exp( info.score ) / Math.exp( maxScore ) ) );

			// add oval for this document
			ovals.add( new Ellipse2D.Double( xPos-size, midY-size, 2*size, 2*size ) );
			
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
		for( int i = minYear; i <= maxYear+1; i++ ) {
			int xPos = HORIZONTAL_INSET;
			int yearPos = xPos + (i - minYear)*yearWidth;
			g.setColor( Color.black );
			g.drawLine( yearPos, midY-YEAR_TICK_SIZE, yearPos, midY+YEAR_TICK_SIZE );
			g.setColor( Color.blue );
			g.drawString(""+i, yearPos-5, midY-YEAR_TICK_SIZE);
			for( int j = 0; j < 12; j++ ) {
				int monthPos = (int)( yearPos + j*( yearWidth / 12.0 ) );
				g.setColor( Color.black );
				g.drawLine( monthPos, midY-MONTH_TICK_SIZE, monthPos, midY+MONTH_TICK_SIZE );
			}
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
	
	public void setStartDate( String start ) {
		
	}
	
	public String getEndDate() {
		return maxMonth + "/" + maxDate + "/" + maxYear;  
	}
	
	public void setEndDate( String end ) {
		
	}
}
