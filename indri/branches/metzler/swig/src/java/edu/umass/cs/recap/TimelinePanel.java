package edu.umass.cs.recap;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Shape;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Ellipse2D;
import java.util.ArrayList;
import java.util.EventListener;
import java.util.HashMap;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
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

	private boolean sourcesCombined = false;
	
	private boolean monthTicksVisible = true;
	private boolean yearTicksVisible = true;
	
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

	// drag-selection information
	private Point startDragPoint = null;
	private Point endDragPoint = null;
	private int dragStartMonth = 0;
	private int dragStartYear = 0;
	private int dragEndMonth = 0;
	private int dragEndYear = 0;
	
	// which documents are actually viewable
	// if this variable is null then all documents are viewable
	private boolean [] viewable = null;

	// popup menu associated with timeline
	private JPopupMenu popup = null;
	private JMenuItem sourcePopupItem = null;
	private JMenuItem monthTicksPopupItem = null;
	private JMenuItem yearTicksPopupItem = null;
	private JMenuItem resetPopupItem = null;
	
	// hashmap of 'sources' contained in results
	private HashMap sources = null;
	private ArrayList sourceNames = null;
	
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
		
		// set up popup menu
		popup = new JPopupMenu();
		sourcePopupItem = new JMenuItem( "Combine sources" );
		monthTicksPopupItem = new JMenuItem( "Hide month ticks" );
		yearTicksPopupItem = new JMenuItem( "Hide year ticks" );
		resetPopupItem = new JMenuItem( "Reset" );
		popup.add( sourcePopupItem );
		popup.add( new JPopupMenu.Separator() );
		popup.add( monthTicksPopupItem );
		popup.add( yearTicksPopupItem );
		popup.add( new JPopupMenu.Separator() );
		popup.add( resetPopupItem );
		
		// set tool tip text
		previousDocButton.setToolTipText( "Move to previous document");
		analyzeButton.setToolTipText( "Analyze current document");
		nextDocButton.setToolTipText( "Move to next document");
		
		// set up source hashmap
		sources = new HashMap();
		sourceNames = new ArrayList();
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

		sources.clear();
		sourceNames.clear();
		int sourceNum = 0;
		
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
			
			String source = info.docName.toUpperCase().substring( 0, 2 );
			Object o = sources.get( source );
			if( o == null ) {
				sourceNames.add( source );
				sources.put( source, new Integer( sourceNum++ ) );
			}
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
	
		int sourceOffset = 0;
				
		// width that each type of segment takes up
		int numMonths = 12 * ( maxYear - minYear ) - minMonth + maxMonth;
		double monthWidth = ( width - 2.0*HORIZONTAL_INSET ) / ( 1.0*numMonths );
		double dayWidth = monthWidth / 31.0 ;

		// draw tickmarks for every year / month
		if( sourcesCombined )
			drawTickedLine( g, width, midY, numMonths, monthWidth, true, null );
		else {
			for( int i = 0; i < sources.size(); i++ )
				drawTickedLine( g, width, midY + i*YEAR_TICK_SIZE - YEAR_TICK_SIZE, numMonths, monthWidth, i == 0, (String)sourceNames.get( i ) );
		}
		
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

			// if we're showing the sources then we must compute the source offset
			if( !sourcesCombined ) {
				int sourceNum = ((Integer)sources.get( info.docName.toUpperCase().substring( 0, 2 ) )).intValue();
				sourceOffset = sourceNum*YEAR_TICK_SIZE - YEAR_TICK_SIZE;
			}
			
			// add oval for this document
			ovals.add( new Ellipse2D.Double( xPos-size, midY-size + sourceOffset, 2*size, 2*size ) );

			// make sure this document is viewable
			if( viewable != null && !viewable[i] )
				continue;

			// make sure we're actually on the screen
			if( xPos < HORIZONTAL_INSET || xPos > width - HORIZONTAL_INSET )
				continue;
						
			if( info == currentInfo ) {
				// TODO: make this into a "drawDoc" function
				g.setColor( new Color( 0.0f, 1.0f, 0.0f, 1.0f ) );
				g.fillOval(xPos-size, midY-size + sourceOffset, 2*size, 2*size);
				g.setColor( new Color( 0.0f, 1.0f, 0.0f, 1.0f ) );
				g.drawOval(xPos-size, midY-size + sourceOffset, 2*size, 2*size);
			}
			else {
				g.setColor( new Color( 1.0f, 0.0f, 0.0f, 0.50f ) );
				g.fillOval(xPos-size, midY-size + sourceOffset, 2*size, 2*size);
				g.setColor( new Color( 1.0f, 0.0f, 0.0f, 1.0f ) );
				g.drawOval(xPos-size, midY-size + sourceOffset, 2*size, 2*size);
			}
		}
				
		// draw click-dragged region, if any
		if( startDragPoint != null && endDragPoint != null ) {
			g.setColor( new Color( 0.0f, 0.0f, 1.0f, 0.50f ) );
			int w = endDragPoint.x - startDragPoint.x;
			int sz = sourcesCombined ? 0 : sources.size() - 1;
			int offset = sourcesCombined ? 0 : -YEAR_TICK_SIZE;
			if( w < 0 ) {
				g.fillRect( endDragPoint.x, midY-YEAR_TICK_SIZE+offset, -w, (2 + sz)*YEAR_TICK_SIZE );
				dragStartMonth = (int)Math.floor( ( endDragPoint.x - HORIZONTAL_INSET ) / monthWidth );
				dragEndMonth = (int)Math.ceil( ( startDragPoint.x - HORIZONTAL_INSET ) / monthWidth );
			}
			else {
				g.fillRect( startDragPoint.x, midY-YEAR_TICK_SIZE+offset, w, (2 + sz)*YEAR_TICK_SIZE );
				dragStartMonth = (int)Math.floor( ( startDragPoint.x - HORIZONTAL_INSET ) / monthWidth );
				dragEndMonth = (int)Math.ceil( ( endDragPoint.x - HORIZONTAL_INSET ) / monthWidth );
			}
			dragStartYear = minYear + (int)( ( dragStartMonth + minMonth - 1 ) / 12.0 );
			dragEndYear = minYear + (int)( ( dragEndMonth + minMonth - 1 ) / 12.0 );
			dragStartMonth = ( dragStartMonth + minMonth - 1) % 12 + 1;
			dragEndMonth = ( dragEndMonth + minMonth - 1) % 12 + 1;
		}
		else {
			dragStartMonth = 0;
			dragStartYear = 0;
			dragEndMonth = 0;
			dragEndYear = 0;
		}
 	}

	private void drawTickedLine( Graphics g, int width, int midY, int numMonths, double monthWidth, boolean drawDates, String sourceName ) {
		int curYear = minYear + 1;
		if( minMonth == 1 ) // the first tick we draw will be a year tick
			curYear = minYear;
		
		// draw main axis
		g.setColor( Color.black );
		g.drawLine( HORIZONTAL_INSET, midY, width-HORIZONTAL_INSET, midY);

		for( int curMonth = 0; curMonth <= numMonths; curMonth++ ) {
			g.setColor( Color.black );
			int xPos = HORIZONTAL_INSET + (int)( curMonth*monthWidth ); 
			if( ( minMonth + curMonth - 1 ) % 12 == 0 ) {
				if( yearTicksVisible )
					g.drawLine( xPos, midY-YEAR_TICK_SIZE, xPos, midY+YEAR_TICK_SIZE );
				if( drawDates ) {
					g.setColor( Color.blue );
					g.drawString(""+curYear, xPos-15, midY-YEAR_TICK_SIZE);
				}
				curYear++;
			}
			else {
				if( monthTicksVisible )
					g.drawLine( xPos, midY-MONTH_TICK_SIZE, xPos, midY+MONTH_TICK_SIZE );
			}
		}
		if( sourceName != null ) {
			g.setColor( Color.blue );
			g.drawString( sourceName, 0, midY + 5 );
		}
	}
	
	public ScoredDocInfo getDocAt( Point p ) {
		ScoredDocInfo ret = null;
		for( int i = 0; i < ovals.size(); i++ ) {
			if( viewable != null && !viewable[i] ) // can't select invisible documents
				continue;
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
	
	public void setStartDragPoint( Point p ) {
		startDragPoint = p;
	}
	
	public void setEndDragPoint( Point p ) {
		endDragPoint = p;
	}
	
	public void clearDragPoints() {
		startDragPoint = null;
		endDragPoint = null;
	}

	// TODO: make a "Date" class to return this with instead
	public String getDragStartDate() {
		return dragStartMonth + "/" + dragStartYear;
	}

	public String getDragEndDate() {
		return dragEndMonth + "/" + dragEndYear;
	}

	// returns the popup menu associated with the timeline
	public JPopupMenu getPopup() {
		return popup;
	}
	
	// resets the timeline to it's original configuration
	public void reset() {
		init();
		repaint();
	}
	
	// sets the visibility of the month tick marks
	public void setMonthTicksVisible( boolean b ) {
		if( b )
			monthTicksPopupItem.setText( "Hide month ticks" );
		else	
			monthTicksPopupItem.setText( "Show month ticks" );
		monthTicksVisible = b;
		repaint();
	}
	
	// sets the visibility of the year tick marks
	public void setYearTicksVisible( boolean b ) {
		if( b )
			yearTicksPopupItem.setText( "Hide year ticks" );
		else
			yearTicksPopupItem.setText( "Show year ticks" );
		yearTicksVisible = b;
		repaint();
	}

	public void	setSourcesCombined( boolean b ) {
		if( b )
			sourcePopupItem.setText( "Separate sources" );
		else
			sourcePopupItem.setText( "Combine sources" );
		sourcesCombined = b;
		repaint();
	}
	
	// register EventListeners for this class
	public void addListeners( EventListener listener ) {
		previousDocButton.addActionListener( (ActionListener)listener );
		nextDocButton.addActionListener( (ActionListener)listener );
		analyzeButton.addActionListener( (ActionListener)listener );

		sourcePopupItem.addActionListener( (ActionListener)listener );
		monthTicksPopupItem.addActionListener( (ActionListener)listener );
		yearTicksPopupItem.addActionListener( (ActionListener)listener );
		resetPopupItem.addActionListener( (ActionListener)listener );
		
		addMouseListener( (MouseListener)listener );
		addMouseMotionListener( (MouseMotionListener)listener );
	}

}
