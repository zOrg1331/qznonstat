// Time-stamp: <zchart.cpp - 12:19:24 05.07.2006>
/******************************************************************
 *** ������ ������������ ����� ����������� �������� � ��������  ***
 ***         ������������� � ������� ��������� ������           ***
 ******************************************************************/

/* code: ������ ������� <knzsoft@mail.ru> */
/* some changes & improvements: ���������� ����� <zorg1331@mail.ru> */

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QTextCodec>
#include <QToolTip>

#include <QtGui>

#include <math.h>

#include "zchart.h"

//===================================================================//
//                                                                   //
//  ������� ����� ��� ������ ��������, ������������ � ������ ZChart  //
//                                                                   //
//===================================================================//

ZChartSeries::ZChartSeries( const QString &aName ) {
     name = aName;
}

ZChartSeries::ZChartSeries( const QString &aName, const QColor &aColor ) {
     name = aName;
     color = aColor;
}

void ZChartSeries::setColor( const QColor &c ) {
     color = c;
}

void ZChartSeries::setGraphArea( const ZGraphArea &g ) {
     ga = g;

     // ������� ����� ��� �������� ���������
     xMin = ga.xMin;
     xMax = ga.xMax;
     yMin = ga.yMin;
     yMax = ga.yMax;
     xsMin = ga.xsMin;
     xsMax = ga.xsMax;
     ysMin = ga.ysMin;
     ysMax = ga.ysMax;
     painter = ga.painter;
     xPenPos = xsMin; // ��������� ������� x-���������� ����
     yPenPos = ysMin; // ��������� ������� y-���������� ����

     // �������� ������������ ��������
     xScreen = ( xsMax - xsMin ) / ( xMax - xMin );
     yScreen = ( ysMax - ysMin ) / ( yMax - yMin );

     isLastPointVisible = true;
}

int ZChartSeries::xToScreen( long double x ) {
     return ( int )(( x - xMin ) * xScreen + xsMin );
}

int ZChartSeries::yToScreen( long double y ) {
     return ysMax - ( int )(( y - yMin ) * yScreen );
}

void ZChartSeries::draw( void ) {}

void ZChartSeries::moveTo( int x, int y ) {
     xPenPos = x;
     yPenPos = y;
     isLastPointVisible = true;
}

void ZChartSeries::lineTo( int x, int y ) {
     if (( x >= xsMin ) && ( x <= xsMax ) && ( y >= ysMin ) && ( y <= ysMax ) ) {
	  painter->drawLine( xPenPos, yPenPos, x, y );
	  isLastPointVisible = true;
     } else {
	  if ( isLastPointVisible )
	       painter->drawLine( xPenPos, yPenPos, x, y );

	  //���� ������� ������ ������ ����������� ��������, ����� ��� �����
	  //�� ��������� �������, �� ����� ������� ����������� �������
	  // ������� ���������� ������� ;) (c) zOrg
	  else {
	       // y=(y1-y2)*(x-x1)/(x1-x2)+y1
	       // x=(y-y1)*(x1-x2)/(y1-y2)+x1
	       // ysMax = (yPenPos-y)*(xN-xPenPos)/(xPenPos-x)+yPenPos

	       // ����� ��������� �������, ������� ������ ���� ������������ ������� ��
	       // ���������� (��������, ��� ������������� �������).
	       if ((
			( yPenPos <= ysMin && y >= ysMin )
			|| ( yPenPos >= ysMax && y <= ysMax )
			|| ( yPenPos <= ysMax && yPenPos >= ysMin )
			)
		   && (
			( xPenPos <= xsMin && x >= xsMin )
			|| ( xPenPos >= xsMax && x <= xsMax )
			|| ( xPenPos >= xsMin && xPenPos <= xsMax )
			)
		    ) {

		    // ���� ���� ����� ������� ���� ��� ��
		    if ( yPenPos <= ysMin ) {
			 // ������� ����� ����������� ������, ���������� ����� ����� �������
			 // � ������ y=ysMax
			 if ( yPenPos != y ) {
			      double xN = ( ysMax - yPenPos ) * ( xPenPos - x ) / ( yPenPos - y ) + xPenPos;
			      // ���� ����������� ����� ������� �� �������� �������, �� �������
			      // ����������� � ������ x=xsMax ��� x=xsMin
			      if ( xN > xsMax && xPenPos != x ) {
				   double yN = ( yPenPos - y ) * ( xsMax - xPenPos ) / ( xPenPos - x ) + yPenPos;
				   painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMax, ( int )yN );

			      } else
				   if ( xN < xsMin && xPenPos != x ) {
					double yN = ( yPenPos - y ) * ( xsMin - xPenPos ) / ( xPenPos - x ) + yPenPos;
					painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMin, ( int )yN );

				   } else
					painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xN, ( int )ysMax );
			 }
			 // ���� ���� ����� ������� ���� ������ y=ysMax
		    } else
			 if ( yPenPos >= ysMax ) {
			      // ������� ����� ����������� ������, ���������� ����� ����� �������
			      // � ������ y=ysMin
			      if ( yPenPos != y ) {
				   double xN = ( ysMin - yPenPos ) * ( xPenPos - x ) / ( yPenPos - y ) + xPenPos;
				   // ���� ����������� ����� ������� �� �������� �������, �� �������
				   // ����������� � ������ x=xsMax ��� x=xsMin
				   if ( xN > xsMax && xPenPos != x ) {
					double yN = ( yPenPos - y ) * ( xsMax - xPenPos ) / ( xPenPos - x ) + yPenPos;
					painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMax, ( int )yN );

				   } else
					if ( xN < xsMin && xPenPos != x ) {
					     double yN = ( yPenPos - y ) * ( xsMin - xPenPos ) / ( xPenPos - x ) + yPenPos;
					     painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMin, ( int )yN );

					} else
					     painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xN, ( int )ysMin );
			      }
			      // ���� ���� ����� ������� ����� � ���������
			      // ����� ������� � ������ ���������
			 } else
			      if ( yPenPos <= ysMax && yPenPos >= ysMin ) {
				   // ���� ������� ������� �����
				   if ( y >= yPenPos ) {
					// ������� ����� ����������� ������, ���������� ����� ����� �������
					// � ������ y=ysMax
					if ( yPenPos != y ) {
					     double xN = ( ysMax - yPenPos ) * ( xPenPos - x ) / ( yPenPos - y ) + xPenPos;
					     // ���� ����������� ����� ������� �� �������� �������, �� �������
					     // ����������� � ������ x=xsMax ��� x=xsMin
					     if ( xN > xsMax && xPenPos != x ) {
						  double yN = ( yPenPos - y ) * ( xsMax - xPenPos ) / ( xPenPos - x ) + yPenPos;
						  painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMax, ( int )yN );

					     } else
						  if ( xN < xsMin && xPenPos != x ) {
						       double yN = ( yPenPos - y ) * ( xsMin - xPenPos ) / ( xPenPos - x ) + yPenPos;
						       painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMin, ( int )yN );

						  } else
						       painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xN, ( int )ysMax );
					}
					// ���� ������� ������� ����
				   } else
					if ( y <= yPenPos ) {
					     // ������� ����� ����������� ������, ���������� ����� ����� �������
					     // � ������ y=ysMin
					     if ( yPenPos != y ) {
						  double xN = ( ysMin - yPenPos ) * ( xPenPos - x ) / ( yPenPos - y ) + xPenPos;
						  // ���� ����������� ����� ������� �� �������� �������, �� �������
						  // ����������� � ������ x=xsMax ��� x=xsMin
						  if ( xN > xsMax && xPenPos != x ) {
						       double yN = ( yPenPos - y ) * ( xsMax - xPenPos ) / ( xPenPos - x ) + yPenPos;
						       painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMax, ( int )yN );

						  } else
						       if ( xN < xsMin && xPenPos != x ) {
							    double yN = ( yPenPos - y ) * ( xsMin - xPenPos ) / ( xPenPos - x ) + yPenPos;
							    painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xsMin, ( int )yN );

						       } else
							    painter->drawLine(( int )xPenPos, ( int )yPenPos, ( int )xN, ( int )ysMin );
					     }
					}
			      }
	       }
	  };
	  isLastPointVisible = FALSE;
     };
     xPenPos = x;
     yPenPos = y;
}

double ZChartSeries::getMaxX( void ) {
     return 0;
}

double ZChartSeries::getMaxY( void ) {
     return 0;
}

double ZChartSeries::getMinX( void ) {
     return 0;
}

double ZChartSeries::getMinY( void ) {
     return 0;
}

double ZChartSeries::getMaxYy( double /*xl*/,double /*xr*/ ) {
     return 0;
}

double ZChartSeries::getMinYy( double /*xl*/,double /*xr*/ ) {
     return 0;
}

//==================================================================
//
//  "��������" ������� ������ ��� ����������� ������ ������
//
//==================================================================

ZSimpleSeries::ZSimpleSeries( const QString &aName )
     : ZChartSeries( aName ) {
     point.resize( 100 );
     LongDoublePoint p = {0, 0};
     point.fill( p );
}

ZSimpleSeries::ZSimpleSeries( const QString &aName, const QColor &aColor )
     : ZChartSeries( aName, aColor ) {
     point.resize( 1 );
     LongDoublePoint p = {0, 0};
     point.fill( p );
     color = aColor;
}

void ZSimpleSeries::draw( void ) {
     LongDoublePoint pt;
     int x2, y2;

     QPen pen = painter->pen();
     //�������� ����
     pen.setColor( color );
     pen.setStyle( Qt::SolidLine );
     pen.setWidth( 1 );
     painter->setPen( pen );

     for ( int i = 0; i < point.size(); ++i ) {
	  pt = point.at( i );
	  x2 = xToScreen( pt.x );
	  y2 = yToScreen( pt.y );
	  //if (i>0) painter->drawLine(x1,y1,x2,y2);
	  //x1 = x2; y1 = y2;
	  if ( i > 0 )
	       lineTo( x2, y2 );
	  else
	       moveTo( x2, y2 );
     };
}

void ZSimpleSeries::add( long double x, long double y ) {
     LongDoublePoint p = {x, y};
     point.append( p );
}

void ZSimpleSeries::clear( void ) {
     point.clear();
}

double ZSimpleSeries::getYValue( double x ) {
     // ��������� �� ���� �����, ���� ���������� �� ��, ��� ������� ������� ������,
     // ��������������� � ������� ���������� ��������.
     // ��� ���������� ������ ������-��� ������ ����� ������� � �����
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).x >= x ) {
	       return point.at( i ).y;
	  }
     }
     return 0;
}

double ZSimpleSeries::getMaxX() {
     double xMax = -10000000;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).x > xMax )
	       xMax = point.at( i ).x;
     }
     return xMax;
}

double ZSimpleSeries::getMaxY() {
     double yMax = -10000000;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).y > yMax )
	       yMax = point.at( i ).y;
     }
     return yMax;
}

double ZSimpleSeries::getMaxYy(double xl,double xr) {
     double yMax = -10000000;
     for ( int i = 0; i < point.size(); i++ ) {
          if ((point.at( i ).x >= xl) && (point.at( i ).x <= xr)) {
	    if ( point.at( i ).y > yMax )
	         yMax = point.at( i ).y;
          };
     }
     return yMax;
}

double ZSimpleSeries::getMinX() {
     double xMin = 1000000;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).x < xMin )
	       xMin = point.at( i ).x;
     }
     return xMin;
}

double ZSimpleSeries::getMinY() {
     double yMin = 1000000;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).y < yMin )
	       yMin = point.at( i ).y;
     }
     return yMin;
}

double ZSimpleSeries::getMinYy(double xl,double xr) {
     double yMin = 1000000;
     for ( int i = 0; i < point.size(); i++ ) {
          if ((point.at( i ).x >= xl) && (point.at( i ).x <= xr)) {
	   if ( point.at( i ).y < yMin )
	       yMin = point.at( i ).y;
        }
     }
     return yMin;
}
//==================================================================
//
//  ���� �����, ��� � ����, �� ���������� ������������ ��������
//
//==================================================================

ZLimitSeries::ZLimitSeries( const QString &aName )
     : ZChartSeries( aName ) {
     point.resize( 100 );
     LongDoublePoint p = {0, 0};
     point.fill( p );
     baseLine = 32767.0;
     isFilled = true;
}

ZLimitSeries::ZLimitSeries( const QString &aName, const QColor &aColor,
                            double baseline, bool isfilled )
     : ZChartSeries( aName, aColor ) {
     point.resize( 1 );
     LongDoublePoint p = {0, 0};
     point.fill( p );
     baseLine = baseline;
     isFilled = isfilled;
     color = aColor;
}

void ZLimitSeries::draw( void ) {
     LongDoublePoint pt;
     int x2, y2;

     QPen pen = painter->pen();
     pen.setStyle( Qt::SolidLine );
     pen.setWidth( 1 );

     for ( int i = 0; i < point.size(); ++i ) {
	  pt = point.at( i );
	  x2 = xToScreen( pt.x );
	  y2 = yToScreen( pt.y );
	  int R = 100 + 1000 * ( double )( pt.y - baseLine ) / ( double )( getMaxY() - getMinY() );
	  if ( R > 255 ) {
	       R = 255;
	  }
	  if ( pt.y >= baseLine ) {
	       pen.setColor( QColor( R, 20, 20 ) );
	  } else {
	       pen.setColor( color );
	  }
	  painter->setPen( pen );
	  //if (i>0) painter->drawLine(x1,y1,x2,y2);
	  //x1 = x2; y1 = y2;
	  if ( i > 0 )
	       lineTo( x2, y2 );
	  else
	       moveTo( x2, y2 );
	  if (( isFilled ) && ( pt.y >= baseLine ) ) {
	       QBrush brush = painter->brush();
	       QBrush brush_old = painter->brush();
	       brush.setStyle( Qt::SolidPattern );
	       brush.setColor( QColor( R, 20, 20 ) );
	       painter->setBrush( brush );
	       QPointF points[4] = {
		    QPointF( x_old, y_old ),
		    QPointF( x2, y2 ),
		    QPointF( x2, yToScreen( baseLine ) ),
		    QPointF( x_old, yToScreen( baseLine ) ),
	       };
	       painter->drawPolygon( points, 4 );
	       painter->setBrush( brush_old );
	  }
	  x_old = x2;
	  y_old = y2;
     };
     pen.setStyle( Qt::SolidLine );
     pen.setWidth( 2 );
     pen.setColor( Qt::red );
     painter->setPen( pen );
     moveTo( 0, yToScreen( baseLine ) );
     lineTo( 32768.0, yToScreen( baseLine ) );
}

void ZLimitSeries::add( long double x, long double y ) {
     LongDoublePoint p = {x, y};
     point.append( p );
}

void ZLimitSeries::clear( void ) {
     point.clear();
}

double ZLimitSeries::getYValue( double x ) {
     // ��������� �� ���� �����, ���� ���������� �� ��, ��� ������� ������� ������,
     // ��������������� � ������� ���������� ��������.
     // ��� ���������� ������ ������-��� ������ ����� ������� � �����
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).x >= x ) {
	       return point.at( i ).y;
	  }
     }
     return 0;
}

double ZLimitSeries::getMaxX() {
     double xMax = 0;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).x > xMax )
	       xMax = point.at( i ).x;
     }
     return xMax;
}

double ZLimitSeries::getMaxY() {
     double yMax = 0;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).y > yMax )
	       yMax = point.at( i ).y;
     }
     return yMax;
}

double ZLimitSeries::getMaxYy(double xl,double xr) {
     double yMax = -10000000;
     for ( int i = 0; i < point.size(); i++ ) {
          if ((point.at( i ).x >= xl) && (point.at( i ).x <= xr)) {
	    if ( point.at( i ).y > yMax )
	         yMax = point.at( i ).y;
          };
     }
     return yMax;
}

double ZLimitSeries::getMinX() {
     double xMin = 10000;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).x < xMin )
	       xMin = point.at( i ).x;
     }
     return xMin;
}

double ZLimitSeries::getMinY() {
     double yMin = 10000;
     for ( int i = 0; i < point.size(); i++ ) {
	  if ( point.at( i ).y < yMin )
	       yMin = point.at( i ).y;
     }
     return yMin;
}

double ZLimitSeries::getMinYy(double xl,double xr) {
     double yMin = 1000000;
     for ( int i = 0; i < point.size(); i++ ) {
          if ((point.at( i ).x >= xl) && (point.at( i ).x <= xr)) {
	   if ( point.at( i ).y < yMin )
	       yMin = point.at( i ).y;
        }
     }
     return yMin;
}

//====================================================================
//
//  ����� �������� ��� ��������, ���������� � ����������� ZChartSeries
//
//====================================================================

ZChart::ZChart( QWidget *parent )
     : QWidget( parent ) {
     frames = new ZFrames( this );
     setMinimumSize( 500, 150 );
     setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
     setMouseTracking( true );

     // ��������� ��������� ���� �� ���������
     setXRange( 0.0, 1.0 );
     setYRange( 0.0, 1.0 );
     setFakeXRange(0, 0);

     borderType = btRaisedNone;
     titleBorderType = btRaisedNone;
     graphPanelBorderType = btRaisedLowered;

     // ������ ���� �� ������
     leftMouseDownActive  = false;
     rightMouseDownActive = false;
     // ����������� � ������ ������� �� �������
     baffleMovedActive = false;
     // ����� ��� �� ������
     isTitleTruncated = false;

     // �� ����������� ������
     isLightGraph = false;

     sbBaffles[0] = 100;
     margin  = 2; // ������ ������� �����
     spacing = 2; // ������ ���������� �����
     titleText = trUtf8( "Здесь могла быть ваша реклама!" );
     sbFirstText = "";
     sbSecondText = "";
     xAxName = "";
     yAxName = "";
     zeroLabel = "";
     showLegendFlag = false;
     autoscaleFlag = false;
     fixSize();

     // ������ ����� ������� ����� �, �������������,
     // ������������� ������� �� ���������� � �����
     setFocusPolicy( Qt::StrongFocus );

     // ������� ������� - ������� �� ���������
     curZoom = 0;

     // ����������� ��������� �������� ����
     mainFont.setFamily( "Arial" );
     mainFont.setPointSize( 10 );
     boldFont.setFamily( "Arial" );
     boldFont.setPointSize( 10 );
     boldFont.setBold( true );
     axesPen.setStyle( Qt::SolidLine );
     axesPen.setWidth( 2 );
     axesPen.setColor( Qt::black );
     gridPen.setStyle( Qt::DotLine );
     gridPen.setWidth( 1 );
     gridPen.setColor( Qt::gray );
     legendFramePen.setStyle( Qt::DotLine );
     legendFramePen.setWidth( 1 );
     legendFramePen.setColor( Qt::black );
     rubberPen.setStyle( Qt::DotLine );
     rubberPen.setWidth( 1 );
     rubberPen.setColor( Qt::black );
     zeroPen.setStyle( Qt::SolidLine );
     zeroPen.setWidth( 0 );
     zeroPen.setColor( Qt::black );
     zoomButtonsPen.setStyle( Qt::SolidLine );
     zoomButtonsPen.setWidth( 1 );
     zoomButtonsPen.setColor( Qt::gray );

     this->setFont( mainFont );
}

// ���������� ������ ������ ������ - ���������� ������ ZChartSeries
void ZChart::addSeries( ZChartSeries *s ) {
     series.append( s );
}

void ZChart::removeSeries() {
     series.clear();
}

// ��������� ������ �������������� �����
int ZChart::fixXScaleHeight( void ) {
     return fontSize;
}

// ��������� ������ ������������ �����
int ZChart::fixYScaleWidth( void ) {
     long double y = yMinTick;
     int count = 0;
     int p;
     QString s;
     int res = 8;
     int w;

     while ( y <= yMax + yStepTick / 10.0 ) {
	  if ( yPrecision < 0 ) {
	       p = -yPrecision;
	       s = QString::number( y, 'f', p );
	  } else {
	       s = QString::number( y, 'f', 1 );
	  };
	  w = fontMetrics().width( s );
	  if ( res < w )
	       res = w;
	  y += yStepTick;
	  count++;
	  if ( count > 500 )
	       break; // ������������ ������ ������������ ���� long double ��� y
     };
     return res + 4;
}

// ��������� ��������� �������������� �����
void ZChart::setXRange( long double aXMin, long double aXMax ) {
     long double foo;

     if ( aXMin > aXMax ) {
	  foo = aXMin;
	  aXMin = aXMax;
	  aXMax = foo;
     };

     xMin = xMinOrig = aXMin;
     xMax = xMaxOrig = aXMax;

     if ( zoomStack.size() != 0 ) {
	  zoomStack[curZoom].xMin = xMin;
	  zoomStack[curZoom].xMax = xMax;
     }

     adjustXScale();
}

void ZChart::setFakeXRange( long double aFXMin, long double aFXMax ) {
     long double foo;

     if ( aFXMin > aFXMax ) {
	  foo = aFXMin;
	  aFXMin = aFXMax;
	  aFXMax = foo;
     };

     xFMin = aFXMin;
     xFMax = aFXMax;

     adjustXScale();
}

// ��������� ��������� ������������ �����
void ZChart::setYRange( long double aYMin, long double aYMax ) {
     long double foo;

     if ( aYMin > aYMax ) {
	  foo = aYMin;
	  aYMin = aYMax;
	  aYMax = foo;
     };

     yMin = yMinOrig = aYMin;
     yMax = yMaxOrig = aYMax;

     if ( zoomStack.size() != 0 ) {
	  zoomStack[curZoom].yMin = yMin;
	  zoomStack[curZoom].yMax = yMax;
     }

     adjustYScale();
}

// ��������/��������� �������������������
void ZChart::setAutoscale( bool _autoscale ) {
     autoscaleFlag = _autoscale;
}

void ZChart::autoscale() {
     double xMax = -1000000, yMax = -1000000;
     double xMin = 1000000, yMin = 1000000;
     ZChartSeries *s;
     for ( int i = 0; i < series.count(); i++ ) {
	  s = series.at( i );
	  if ( s ) {
	       if ( xMax < s->getMaxX() )
		    xMax = s->getMaxX();
	       if ( yMax < s->getMaxY() )
		    yMax = s->getMaxY();
	       if ( xMin > s->getMinX() )
		    xMin = s->getMinX();
	       if ( yMin > s->getMinY() )
		    yMin = s->getMinY();
	  };
     };
     // ������������� �� ������ ��������, � � ������������ � 5% (�� ������ ������� ;) )
     double margin = 0.05;
     setXRange( -margin + xMin - margin*(fabs(xMax) + fabs(xMin)), margin + xMax + margin*(fabs(xMax) + fabs(xMin)));
     setYRange( -margin + yMin - margin*(fabs(yMax) + fabs(yMin)), margin + yMax + margin*(fabs(yMax) + fabs(yMin)));
     update();
}
// �������������������
void ZChart::autoscale1(double xl, double xr) {
     yMax = -1000000;
     yMin = 1000000;
     ZChartSeries *s;
     for ( int i = 0; i < series.count(); i++ ) {
	  s = series.at( i );
	  if ( s ) {
	       if ( yMax < s->getMaxYy(xl, xr) )
		    yMax = s->getMaxYy(xl, xr);
	       if ( yMin > s->getMinYy(xl, xr) )
		    yMin = s->getMinYy(xl, xr);
	  };
     };
     // ������������� �� ������ ��������, � � ������������ � 5% (�� ������ ������� ;) )
     double margin = 0.01;

     setYRange( -0.0001 + yMin - margin*(fabs(yMax) + fabs(yMin)),
		0.0001 + yMax + margin*(fabs(yMax) + fabs(yMin)));
     update();
}

// ��������� �������� ��������� �������
void ZChart::setSettings( QFont _mainFont, QFont _boldFont, QPen _axesPen, QPen _gridPen,
                          QPen _legendFramePen, QPen _rubberPen, QPen _zeroPen ) {
     mainFont = _mainFont;
     boldFont = _boldFont;
     axesPen = _axesPen;
     gridPen = _gridPen;
     legendFramePen = _legendFramePen;
     rubberPen = _rubberPen;
     zeroPen = _zeroPen;
}

// ���������� �� �������
void ZChart::showLegend( bool show ) {
     showLegendFlag = show;
}

// ��������� �������� ��������� �������
void ZChart::setTitle( const QString &s ) {
     titleText = s;
     update();
}

void ZChart::setFirstStatusPanelWidth( int w ) {
     sbBaffles[0] = w;
     xSecondText = sbLeft + sbBaffles[0] + 4;
}

// ��������� �������� ������������� � ������ ������ ������ �������
void ZChart::setFirstText( const QString &s ) {
     sbFirstText = s;
     update();
}

// ��������� �������� ������������� �� ������ ������ ������ �������
void ZChart::setSecondText( const QString &s ) {
     sbSecondText = s;
     update();
}

// ������ ����� ������ ������ ������ �������
void ZChart::drawFirstText( void ) {
     QPainter p( this );
     QString a;
     int len;
     int w = sbBaffles[0] - 7;

     a = sbFirstText;
     QFontMetrics fm( mainFont );
     if ( fm.width( a ) > w ) {
	  while ( fm.width( a ) > w ) {
	       len = a.length();
	       if ( len < 2 )
		    break;
	       a.truncate( len - 2 );
	       a += "~";
	  };
     };
     p.eraseRect( sbLeft + 2, sbTop + 2, sbBaffles[0] - 3, sbHeight - 4 );
     if ( isLightGraph )
	  p.fillRect( sbLeft + 2, sbTop + 2, sbBaffles[0] - 3, sbHeight - 4, Qt::red );
     p.setFont( mainFont );
     p.drawText( xFirstText, yStatusBarText, a );
     p.end();
}

// ������ ����� ������ ������ ������ �������
void ZChart::drawSecondText( void ) {
     QPainter p( this );
     p.setFont( mainFont );
     QString a = sbSecondText;
     int len;
     int w = sbWidth - sbBaffles[0] - 7;

     p.eraseRect( sbLeft + sbBaffles[0] + 2, sbTop + 2,
		  sbWidth - sbBaffles[0] - 4, sbHeight - 4 );
     if ( isLightGraph )
	  p.fillRect( sbLeft + sbBaffles[0] + 2, sbTop + 2,
		      sbWidth - sbBaffles[0] - 4, sbHeight - 4, Qt::red );

     QFontMetrics fm( mainFont );
     if ( !a.isEmpty() ) {
	  if ( p.fontMetrics().width( a ) > w ) {
	       while ( p.fontMetrics().width( a ) > w ) {
		    len = a.length();
		    if ( len < 2 )
			 break;
		    a.truncate( len - 2 );
		    a += "~";
	       };
	  };
	  // ���� � �������� ������ ���� ���� <b></b>,
	  // �.�. ������� �������� ������ ����� ������.
	  int lastX = 0, i = 0;
	  while ( i < a.length() ) {
	       // ���� ��� ����� �����������
	       if ( a[i] != QChar( '<' ) ) {
		    QString text;
		    text[0] = a[i];
		    p.drawText( xSecondText + lastX, yStatusBarText, text );
		    lastX += p.fontMetrics().width( text );
		    i++;
	       } else {
		    // ���� ��� �������� ������ ��������� ������
		    if ( a[i+1] == QChar( 'b' ) ) {
			 i += 3;
			 p.setFont( boldFont );

			 // ���� ��� �����������
		    } else
			 if ( a[i+1] == QChar( '/' ) ) {
			      i += 4;
			      p.setFont( mainFont );
			 }
	       }
	  }
     } else {
	  int x = sbLeft + sbBaffles[0] + 3;
	  int y = sbTop + 3;
	  int dy = ( fontSize / 2 ) * 2;
	  int ym = y + dy / 2 - 1;

	  QPolygon pa;
	  p.setBrush( Qt::darkGreen );
	  pa.putPoints( 0, 7, x, ym, x + 5, y,
			x + 5, ym - 2, x + 16, ym - 2, x + 16, ym + 2,
			x + 5, ym + 2, x + 5, y + dy - 2 );
	  p.drawPolygon( pa );
	  a = trUtf8( "Используйте мышку, чтобы двигать разделитель" );
	  w -= 20;
	  if ( p.fontMetrics().width( a ) > w ) {
	       while ( p.fontMetrics().width( a ) > w ) {
		    len = a.length();
		    if ( len < 2 )
			 break;
		    a.truncate( len - 2 );
		    a += "~";
	       };
	  };
	  p.drawText( xSecondText + 20, yStatusBarText, a );
     };
     p.end();
}

// ������ ����������� �������� ����� � ���� ����
void ZChart::adjustXScale( void ) {
     int count = 5;
     double foo, p;

     foo = ( xMax - xMin ) / count;
     p = floor( log10( foo ) );
     xPrecision = ( int )p;
     xStepTick = pow( 10.0, p );

     if ( xStepTick * 5 < foo )
	  xStepTick *= 5;
     else
	  if ( xStepTick * 2 < foo )
	       xStepTick *= 2;
     xMinTick = ceil( xMin / xStepTick ) * xStepTick;
}

void ZChart::adjustYScale( void ) {
     int count = 4;
     double foo, p;

     foo = ( yMax - yMin ) / count;
     p = floor( log10( foo ) );
     yPrecision = ( int )p;
     yStepTick = pow( 10.0, p );
     if ( yStepTick * 5 < foo )
	  yStepTick *= 5;
     else
	  if ( yStepTick * 2 < foo )
	       yStepTick *= 2;
     yMinTick = ceil( yMin / yStepTick ) * yStepTick;
}

// �������������� �� �������� ���������� "x" � ��������
int ZChart::xToScreen( long double x ) {
     return ( int )(( x - xMin ) / ( xMax - xMin ) * ( xsMax - xsMin ) + xsMin );
}

// �������������� �� �������� ���������� "y" � ��������
int ZChart::yToScreen( long double y ) {
     return ysMax - ( int )(( y - yMin ) / ( yMax - yMin ) * ( ysMax - ysMin ) );
}

// �������������� �� �������� ���������� "x" � ��������
long double ZChart::xToReal( int x ) {
     return ( long double )( x - xsMin ) / ( long double )( xsMax - xsMin )
	  * ( long double )( xMax - xMin ) + xMin;
}

// �������������� �� �������� ���������� "y" � ��������
long double ZChart::yToReal( int y ) {
     return yMin + ( long double )( ysMax - y ) / ( long double )( ysMax - ysMin )
	  * ( long double )( yMax - yMin );
}

// ���������� ��������� ��������� �������� ������
void ZChart::fixSize( void ) {
     int w = width();
     int h = height();
     QFontMetrics fm( mainFont );
     fontSize = mainFont.pointSize();

     //���������� ��������� ������ ���������
     titleLeft = margin;
     titleTop = margin;
     titleWidth = w - 2 * margin;
     titleHeight = fontSize + 2 * spacing + 2;
     xTitleText = ( titleWidth - fm.width( titleText ) - 4 ) / 2;
     yTitleText = titleTop + fontSize + spacing;

     //���������� ��������� ������ �������
     sbLeft = margin;
     sbWidth = w - 2 * margin;
     sbHeight = fontSize + 2 * spacing + 2;
     sbTop = h - margin - sbHeight - 2;
     xFirstText = sbLeft + 4;
     xSecondText = sbLeft + sbBaffles[0] + 4;
     yStatusBarText = sbTop + fontSize + spacing;

     //���������� ��������� ������ ��������
     gpLeft = margin;
     gpWidth = w - 2 * margin;
     gpTop = titleTop + titleHeight + spacing;
     gpHeight = sbTop - gpTop - spacing;

     //���������� ��������� �������������� �����
     xScaleLeft = gpLeft + spacing + 2;
     xScaleWidth = gpWidth - 2 * spacing - 4;
     xScaleHeight = fixXScaleHeight();
     xScaleTop = gpTop + gpHeight - xScaleHeight - spacing - 2;
     //���������� ��������� ������������ �����
     yScaleLeft = gpLeft + spacing + 2;
     yScaleWidth = fixYScaleWidth();
     yScaleTop = gpTop + spacing + 2;
     yScaleHeight = xScaleTop - yScaleTop;
     //���������� ��������� ������� �������
     axisLeft = yScaleLeft + yScaleWidth + spacing;
     axisWidth = gpLeft + gpWidth - axisLeft - spacing - 2;
     axisTop = gpTop + spacing + 2;
     axisHeight = xScaleTop - axisTop - spacing;
     xsMin = axisLeft;
     xsMax = axisLeft + axisWidth - 18;
     ysMin = axisTop + fontSize / 2 + 2;
     ysMax = axisTop + axisHeight;
}

// ��������� ����� ������
void ZChart::draw( void ) {
     // ���� ���� �� ������ �� ��������� �������� ������ ���, ��� �����������
     // �������� ��������� ��������, ������� ����������� ������ �������
     // � ���� ������ �����
     if ( zoomStack.size() == 0 ) {
	  ZGraphArea ga;
	  ga.xMin = xMin;
	  ga.xMax = xMax;
	  ga.yMin = yMin;
	  ga.yMax = yMax;
	  ga.xsMin = xsMin;
	  ga.xsMax = xsMax;
	  ga.ysMin = ysMin;
	  ga.ysMax = ysMax;
	  zoomStack.resize( 1 );
	  zoomStack[0] = ga;
     }

     // ������ �������, � ������� ����� �������� ������ ��
     // �������� ��������� � �����
     xMin = zoomStack[curZoom].xMin;
     xMax = zoomStack[curZoom].xMax;
     yMin = zoomStack[curZoom].yMin;
     yMax = zoomStack[curZoom].yMax;
     xsMin = zoomStack[curZoom].xsMin;
     xsMax = zoomStack[curZoom].xsMax;
     ysMin = zoomStack[curZoom].ysMin;
     ysMax = zoomStack[curZoom].ysMax;

     // ���������� ������������ ��� �����, ����� ��� ����������� ��
     // ����� �����, ��� �� ����������
     adjustXScale();
     adjustYScale();
     fixSize();

     drawStatusBar();
     drawTitle();
     drawGraphPanel();
     drawRubberBand();
}

// ������ ����� (����� �� �������x)
void ZChart::drawRubberBand( void ) {
     QPainter p( this );

     if ( rubberBandIsShown ) {
	  p.setPen( rubberPen );
	  p.drawRect( rubberBandRect.normalized() );
     }
     p.end();
}

// ��������� ��������� �����
void ZChart::updateRubberBandRegion() {
     QRect rect = rubberBandRect.normalized();

     update( rect.left(), rect.top(), rect.width(), 1 );
     update( rect.left(), rect.top(), 1, rect.height() );
     update( rect.left(), rect.bottom(), rect.width(), -1 );
     update( rect.right(), rect.top(), -1, rect.height() );
}

// ��������� ������ �������
void ZChart::drawStatusBar( void ) {
     QPainter p( this );
     p.setFont( mainFont );
     p.end();
     frames->drawStatusBarFrame( sbLeft, sbTop, sbWidth, sbHeight, sbBaffles );
     drawFirstText();
     drawSecondText();
}

// ��������� ������ ���������
void ZChart::drawTitle( void ) {
     QPainter p( this );
     p.setFont( mainFont );
     QString a = titleText;	// ������ � ������, ��, ��������, �����������
     QString realLength = titleText; // ������ ��� �����
     // ������ ������� ������, ��� ����� ������ �����
     // ��� ������ �� ������� ������ � ������ ���������������
     // �� ������� ���� ������� ����, �.�. ��� ������� ������ �����
     realLength.remove( "</b>" );
     int len;
     int zBsLen = 3 * ( titleHeight - 4 ) + 6; // ����� ������ ���������������
     int w = titleWidth - zBsLen - 4 - 10; // -10 - ����� �� ������ ����� (���� �������)
     if ( p.fontMetrics().boundingRect( realLength ).width() > w ) {
	  while ( p.fontMetrics().boundingRect( realLength ).width() > w ) {
	       len = a.length();
	       if ( len < 2 )
		    break;
	       a.truncate( len - 2 );
	       a += "~";
	       len = realLength.length();
	       realLength.truncate( len - 2 );
	       realLength += "~";
	  };
	  isTitleTruncated = true;
     };
     QString truncatedRealLength = a; // ������ ��� �����, ��, ��������, �����������
     truncatedRealLength.remove( "<b>" );
     truncatedRealLength.remove( "</b>" );

     xTitleText = titleLeft + ( titleWidth -
				p.fontMetrics()
				.boundingRect( truncatedRealLength ).width()
				- zBsLen ) / 2;
     p.eraseRect( titleLeft + 2, titleTop + 2, titleWidth - 4 - zBsLen, titleHeight - 4 );
     if ( isLightGraph )
	  p.fillRect( titleLeft + 2, titleTop + 2, titleWidth - 4 - zBsLen, titleHeight - 4, Qt::red );

     // ���� � �������� ������ ���� ���� <b></b>,
     // �.�. ������� �������� ������ ����� ������.
     int lastX = 0, i = 0;
     while ( i < a.length() ) {
	  // ���� ��� ����� �����������
	  if ( a[i] != QChar( '<' ) ) {
	       QString text;
	       text[0] = a[i];
	       p.drawText( xTitleText + lastX, yTitleText, text );
	       lastX += p.fontMetrics().width( text );
	       i++;
	  } else {
	       // ���� ��� �������� ������ ��������� ������
	       if ( a[i+1] == QChar( 'b' ) ) {
		    i += 3;
		    p.setFont( boldFont );

		    // ���� ��� �����������
	       } else
		    if ( a[i+1] == QChar( '/' ) ) {
			 i += 4;
			 p.setFont( mainFont );
		    }
	  }
     }
     p.end();

     switch ( titleBorderType ) {
     case btRaisedNone:
	  frames->drawRaisedNoneFrame( titleLeft, titleTop, titleWidth, titleHeight );
	  break;
     case btRaisedLowered:
	  frames->drawRaisedLoweredFrame( titleLeft, titleTop, titleWidth, titleHeight );
	  break;
     };
}

// ������ ����������� ������: �����, ���, �����, ������� � ������
void ZChart::drawGraphPanel( void ) {
     int w = gpWidth - 4;
     int h = gpHeight - 4;
     int x = 2;
     int y = 0;

     QPainter p( this );

     gpTop = titleTop + titleHeight + spacing;
     p.eraseRect( x, y + gpTop, w, h );
     drawXScale( &p, x, y );
     drawYScale( &p, x, y );
     drawGrid( &p, x, y );
     drawAxes( &p, x, y );
     int cx = axisLeft - x;
     int cy = axisTop - y;
     p.setClipRect( cx, cy, axisWidth, axisHeight );
     drawSeries( &p, x, y );
     if ( showLegendFlag )
	  drawLegend( &p );
     p.end();
     drawZoomButtons();
     drawAutoscaleButton();

     // � ������� ��������� ...
     switch ( graphPanelBorderType ) {
     case btRaisedNone:
	  frames->drawRaisedNoneFrame( gpLeft, gpTop, gpWidth, gpHeight );
	  break;
     case btRaisedLowered:
	  frames->drawRaisedLoweredFrame( gpLeft, gpTop, gpWidth, gpHeight );
	  break;
     };
}

// ������ ������
void ZChart::drawSeries( QPainter *p, int xOffset, int yOffset ) {
     ZChartSeries *s;
     ZGraphArea ga;
     ga.xMin = xMin;
     ga.xMax = xMax;
     ga.yMin = yMin;
     ga.yMax = yMax;
     ga.xsMin = xsMin - xOffset;
     ga.xsMax = xsMax - xOffset;
     ga.ysMin = ysMin - yOffset;
     ga.ysMax = ysMax - yOffset;
     ga.painter = p;

     for ( int i = 0;i < series.count();i++ ) {
	  s = series.at( i );
	  if ( s ) {
	       s->setGraphArea( ga );
	       s->draw();
	  };
     };
}

// ������ �������
void ZChart::drawLegend( QPainter *p ) {
     ZChartSeries *s;
     int maxLength = 0;
     QFontMetrics fm( mainFont );
     // ���������� ����� ������� �������� �����
     for ( int i = 0; i < series.count(); i++ ) {
	  s = series.at( i );
	  if ( s ) {
	       if ( fm.width( s->name ) > maxLength )
		    maxLength = fm.width( s->name );
	  };
     };
     int xOffset = gpWidth - maxLength - 23;
     int yOffset = gpHeight - 15 * series.count() - 20;
     // ������ �������
     // ������ ����� �����
     p->setPen( legendFramePen );
     p->drawRect( xOffset, yOffset,
		  maxLength + 18, 15*series.count() );
     p->setPen( zeroPen );
     p->setFont( mainFont );
     // ������������ ������ �����
     for ( int i = 0; i < series.count(); i++ ) {
	  s = series.at( i );
	  if ( s ) {
	       // ������ �������������, ����������� ���� �����
	       QBrush brush( s->color, Qt::SolidPattern );
	       p->setBrush( brush );
	       p->drawRect( xOffset + 5, 15*i + yOffset + 5, 7, 7 );
	       // ����� �������� �����
	       QString a = s->name;
	       p->drawText( xOffset + 16, 15*i + yOffset + 13, a );
	  };
     };
}

// ������ ������ ����������� �� ���������
void ZChart::drawZoomButtons( void ) {
     QPainter p( this );
     int xOffset1 = titleWidth - 3 * ( titleHeight - 4 ) - 2 - 2;
     int xOffset2 = titleWidth - 2 * ( titleHeight - 4 ) - 2;
     int yOffset = titleTop + 2;
     p.setPen( zoomButtonsPen );
     // ������ ������
     p.drawRoundRect( xOffset1, yOffset, titleHeight - 4, titleHeight - 4, 35, 35 );
     p.drawRoundRect( xOffset2, yOffset, titleHeight - 4, titleHeight - 4, 35, 35 );
     // ������ ������� �� ���
     int x1 = xOffset1 + 2, x2 = xOffset1 + 3 * ( titleHeight - 4 ) / 4, x3 = x2;
     int y1 = yOffset + ( titleHeight - 4 ) / 2, y2 = yOffset + 2, y3 = yOffset + ( titleHeight - 4 ) - 2;
     QPolygon pa;
     p.setBrush( Qt::darkGreen );
     p.setPen( Qt::darkGreen );
     pa.putPoints( 0, 3, x1, y1, x2, y2, x3, y3 );
     p.drawPolygon( pa );
     x1 = xOffset2 + ( titleHeight - 4 ) - 2;
     x2 = xOffset2 + ( titleHeight - 4 ) / 4;
     x3 = x2;
     pa.putPoints( 0, 3, x1, y1, x2, y2, x3, y3 );
     p.drawPolygon( pa );
     p.setBrush( Qt::NoBrush );
     p.end();
}

// ��������, ����������� �� ���������� ������ �����
bool ZChart::isZoomOutButton( int x, int y ) {
     int xOffset = titleWidth - 1 - 3 * ( titleHeight - 4 ) - 2 - 2;
     int yOffset = titleTop + 1;
     return ( x >= xOffset ) && ( x <= ( xOffset + ( titleHeight - 4 ) ) )
	  && ( y >= yOffset ) && ( y <= ( yOffset + ( titleHeight - 4 ) ) );
}

// ��������, ����������� �� ���������� ������ ������
bool ZChart::isZoomInButton( int x, int y ) {
     int xOffset = titleWidth - 1 - 2 * ( titleHeight - 4 ) - 2;
     int yOffset = titleTop + 1;
     return ( x >= xOffset ) && ( x <= ( xOffset + ( titleHeight - 4 ) ) )
	  && ( y >= yOffset ) && ( y <= ( yOffset + ( titleHeight - 4 ) ) );
}

// ������ ������ �������������������
void ZChart::drawAutoscaleButton( void ) {
     QPainter p( this );
     int xOffset = titleWidth - ( titleHeight - 4 );
     int yOffset = titleTop + 2;
     p.setPen( zoomButtonsPen );
     // ������ ������
     p.drawRoundRect( xOffset, yOffset, titleHeight - 4, titleHeight - 4, 35, 35 );
     // ������ ������� �� ���
     int x1 = xOffset + ( titleHeight - 4 ) / 2, x4 = xOffset + 2, x2 = x4 + 2 * ( x1 - x4 ), x3 = x1;
     int y1 = yOffset + 2, y2 = yOffset + ( titleHeight - 4 ) / 2, y4 = y2, y3 = y1 + 2 * ( y2 - y1 );
     QPolygon pa;
     p.setBrush( Qt::darkGreen );
     p.setPen( Qt::darkGreen );
     pa.putPoints( 0, 4, x1, y1, x2, y2, x3, y3, x4, y4 );
     p.drawPolygon( pa );
     p.setBrush( Qt::NoBrush );
     p.end();
}

// ��������, ����������� �� ���������� ������ �������������������
bool ZChart::isAutoscaleButton( int x, int y ) {
     int xOffset = titleWidth - 1 - ( titleHeight - 4 );
     int yOffset = titleTop + 1;
     return ( x >= xOffset ) && ( x <= ( xOffset + ( titleHeight - 4 ) ) )
	  && ( y >= yOffset ) && ( y <= ( yOffset + ( titleHeight - 4 ) ) );
}

// ������ �������������� �����
void ZChart::drawXScale( QPainter *p, int xOffset, int yOffset ) {
     QString s;
     int precision;
     long double x = xMinTick;
     long double xf = xFMin;
     int count = 0;
     int xRight;
     int xLeftEdge = gpLeft - xOffset;
     int xRightEdge = xScaleLeft + xScaleWidth - xOffset;
     int w;
     int ix, iy;
     QFontMetrics fm( mainFont );
     int first = 1;

     iy = xScaleTop + fontSize - 1 - yOffset;
     p->setFont( mainFont );
     while ( 1 ) {
	  if ( xPrecision < 0 ) {
	       precision = -xPrecision;
	       s = QString::number( x, 'f', precision );
	  } else {
	       s = QString::number( x, 'f', 1 );
	  };
	  w = fm.width( s );
	  ix = xToScreen( x ) - w / 2 - xOffset;
	  xRight = ix + w;
	  if (( ix - 8 > xLeftEdge ) && ( xRight <= xRightEdge ) ) {
	       if (first && !zeroLabel.isEmpty()) {
		    p->drawText( ix, iy, zeroLabel );
		    first = 0;
	       } else if (xFMin != xFMax) {
		    QString s1 = QString::number(xf, 'f', 0);
		    p->drawText(ix, iy, s1);
	       } else p->drawText( ix, iy, s );
	       xLeftEdge = xRight;
	  };
	  if ( xLeftEdge >= xRightEdge )
	       break;
	  x += xStepTick;
	  xf += (xFMax-xFMin)/(double)((xMax-xMin)/(double)xStepTick);
	  if ( ++count > 500 )
	       break;
     };
     p->setFont( boldFont );
     p->drawText( xRightEdge - fontMetrics().width( xAxName + "  " ), iy - 15, xAxName );
}

// ������ ������������ �����
void ZChart::drawYScale( QPainter *p, int xOffset, int yOffset ) {
     long double y = yMinTick;    // ������� ������
     int precision; // ���-�� ������ ����� ������� ��� ������
     int count = 0;   // ���-�� �������� �����
     int ix, iy;     // ���������� ���������� �����
     int yTop;      // ������� ���������� ������ ������� �������� �����
     int yTopEdge    = gpTop - yOffset;                // ������� ����������, �� ������� ������ ���������
     int yBottomEdge = axisTop + axisHeight - yOffset; // ������ ����������, �� ������� ������ ���������
     QString s;
     int w;         // ������ ������ ������� �������� �����
     QFontMetrics fm( mainFont );

     ix = axisLeft - spacing - xOffset;
     p->setFont( mainFont );
     while ( 1 ) {
	  if ( yPrecision < 0 ) {
	       precision = -yPrecision;
	       s = QString::number( y, 'f', precision );
	  } else {
	       s = QString::number( y, 'f', 1 );
	  };
	  w = fm.width( s );
	  iy = yToScreen( y ) + fontSize / 2 - 2 - yOffset;
	  yTop = iy - fontSize;
	  if (( iy < yBottomEdge ) && ( yTop >= yTopEdge ) ) {
	       p->drawText( ix - w - 2, iy, s );
	       yBottomEdge = yTop;
	  };
	  if ( yBottomEdge <= yTopEdge )
	       break;
	  y += yStepTick;
	  if ( ++count > 500 )
	       break; // ������������ ������ ������������ ���� long double ��� y
     };
     p->setFont( boldFont );
     p->drawText( ix + 5, yTopEdge + fontSize, yAxName );
}

// ������ ���
void ZChart::drawAxes( QPainter *p, int xOffset, int yOffset ) {
     p->setPen( axesPen );
     p->drawLine( axisLeft - xOffset, axisTop - yOffset,
		  axisLeft - xOffset, axisTop + axisHeight - yOffset );
     p->drawLine( axisLeft - xOffset, axisTop + axisHeight - yOffset,
		  axisLeft + axisWidth - xOffset, axisTop + axisHeight - yOffset );
}

// ������ �����
void ZChart::drawGrid( QPainter *p, int xOffset, int yOffset ) {
     long double x, y;
     int ix, iy;
     int count = 0;

     // �������� ����
     p->setPen( gridPen );

     // ������ �������������� �����
     y  = yMinTick;
     ix = axisLeft + axisWidth - xOffset;
     while (( iy = yToScreen( y ) ) > axisTop ) {
	  iy -= yOffset;
	  p->drawLine( axisLeft - xOffset, iy, ix, iy );
	  y += yStepTick;
	  if ( ++count > 500 )
	       break;
     };

     // ������ ������������ �����
     x  = xMinTick;
     iy = axisTop + axisHeight - yOffset;
     count = 0;
     int w = axisLeft + axisWidth;
     while (( ix = xToScreen( x ) ) < w ) {
	  ix -= xOffset;
	  p->drawLine( ix, axisTop - yOffset, ix, iy );
	  x += xStepTick;
	  if ( ++count > 500 )
	       break;
     };
}

// ��������� ������������ ����
void ZChart::setAxesName( const QString &xName, const QString &yName ) {
     xAxName = xName;
     yAxName = yName;
}

void ZChart::setZeroLabel( const QString &zLabel) {
     zeroLabel = zLabel;
}

// ��������� ���� ������� ��� ����� ������
void ZChart::setBorderType( int aBorderType ) {
     borderType = aBorderType;
}

// ��������� ���� ������� ��� ������� ����������� ��������
void ZChart::setGraphPanelBorderType( int aGraphPanelBorderType ) {
     graphPanelBorderType = aGraphPanelBorderType;
}

// ��������� ������� ������� ����� ������
void ZChart::setMargin( int aMargin ) {
     margin = aMargin;
}

// ��������� ������� ���������� ����� ������
void ZChart::setSpacing( int aSpacing ) {
     spacing = aSpacing;
}

// ��������, ����������� �� ���������� x,y ������� ����������� � ������ �������
bool ZChart::isStatusBarFirstBaffle( int x, int y ) {
     x -= sbLeft;
     y -= sbTop;
     return (( x >= sbBaffles[0] - 2 ) && ( x <= sbBaffles[0] + 2 )
	     && ( y >= 0 ) && ( y <= sbHeight ) );
}

// ���������� ��������� �������� ������� ����������� � ������ �������, ��� 0
int ZChart::isPositionStatusBarFirstBafflePossible( int x ) {
     x -= sbLeft;
     if (( x > 30 ) && ( x < sbWidth - 30 ) )
	  return x;
     else
	  return 0;
}

//======== protected ����� - ��������� ��������� �������� ===========//

// ���������� ������� ����������� ������
void ZChart::paintEvent( QPaintEvent * ) {
     draw();
}

// ���������� ������� ��������� ������� ������
void ZChart::resizeEvent( QResizeEvent * ) {
     fixSize();
}

// ���������� ������� ����������� ����
void ZChart::mouseMoveEvent( QMouseEvent *ev ) {
     int x = ev->x();
     int y = ev->y();
     double fx, fy;
     QString s;
     int xp, yp;

     if ( baffleMovedActive ) {
	  int i = isPositionStatusBarFirstBafflePossible( x );
	  if ( i > 0 ) {
	       sbBaffles[0] = i;
	       xSecondText = sbLeft + sbBaffles[0] + 4;
	       update();
	  };
     };

     if ( isStatusBarFirstBaffle( x, y ) ) {
	  setCursor( Qt::SizeHorCursor );
     } else {
	  if (( x >= axisLeft ) && ( x <= axisLeft + axisWidth )
	      && ( y >= axisTop ) && ( y <= axisTop + axisHeight ) ) {
	       setCursor( Qt::CrossCursor );
	       if ( xPrecision < 0 )
		    xp = -xPrecision + 2;
	       else
		    xp = 2;
	       if ( yPrecision < 0 )
		    yp = -yPrecision + 2;
	       else
		    yp = 2;
	       fx = xToReal( x );
	       fy = yToReal( y );
	       if ( leftMouseDownActive ) {
		    xsUp = x;
		    ysUp = y;
		    xUp = xToReal( x );
		    yUp = yToReal( y );
		    updateRubberBandRegion();
		    rubberBandRect.setBottomRight( ev->pos() );
		    updateRubberBandRegion();
		    s = QString( "%1; " ).arg( fabs( xUp - xDown ), 0, 'f', xp )
			 + QString( "%1" ).arg( fabs( yUp - yDown ), 0, 'f', yp );
		    setFirstText( s );
	       } else {
		    if ( rightMouseDownActive ) {
			 xUp = xToReal( x );
			 yUp = yToReal( y );
			 long double foo = xUp - xDown;
			 xMin -= foo;
			 xMax -= foo;
			 foo = yUp - yDown;
			 yMin -= foo;
			 yMax -= foo;
			 setXRange( xMin, xMax );
			 setYRange( yMin, yMax );
			 adjustXScale();
			 adjustYScale();
			 chCurrStack();
			 update();
		    } else {
			 s = QString( "%1; " ).arg( fx, 0, 'f', xp )
			      + QString( "%1" ).arg( fy, 0, 'f', yp );
			 setFirstText( s );
		    };
	       };
	  } else {
	       setCursor( Qt::ArrowCursor );
	  };
     };
}

// ���������� ������� �� ������� ����
void ZChart::mousePressEvent( QMouseEvent *ev ) {
     xsUp = xsDown = ev->x();
     ysUp = ysDown = ev->y();

     if ( isZoomInButton( xsDown, ysDown ) )
	  zoomIn();
     if ( isZoomOutButton( xsDown, ysDown ) )
	  zoomOut();
     if ( isAutoscaleButton( xsDown, ysDown ) )
	  autoscale();

     if ( isStatusBarFirstBaffle( xsDown, ysDown ) ) {
	  baffleMovedActive = true;
     } else {
	  if (( xsDown >= axisLeft ) && ( xsDown <= axisLeft + axisWidth )
	      && ( ysDown >= axisTop ) && ( ysDown <= axisTop + axisHeight ) ) {
	       xUp = xDown = xToReal( xsDown );
	       yUp = yDown = yToReal( ysDown );
	       switch ( ev->button() ) {
	       case Qt::LeftButton:
		    leftMouseDownActive = true;
		    emit clickCoord( xDown, yDown );
		    emit clickCoordX( xDown );
		    emit clickCoordY( yDown );
		    rubberBandIsShown = true;
		    rubberBandRect.setTopLeft( ev->pos() );
		    rubberBandRect.setBottomRight( ev->pos() );
		    updateRubberBandRegion();
		    break;
	       case Qt::RightButton:
		    rightMouseDownActive = true;
		    break;
	       case Qt::NoButton:
		    break;
	       case Qt::MidButton:
		    break;
	       case Qt::XButton1:
		    break;
	       case Qt::XButton2:
		    break;
	       case Qt::MouseButtonMask:
		    break;
	       };
	  };
     };
}

// ���������� ���������� ������� ����
void ZChart::mouseReleaseEvent( QMouseEvent * ) {
     if ( leftMouseDownActive ) {
	  rubberBandIsShown = false;
	  updateRubberBandRegion();
	  if (( abs( xsUp - xsDown ) > 20 ) && ( abs( ysUp - ysDown ) > 20 ) ) {
	       if ( xsUp > xsDown ) {
		    xMin = xDown;
		    xMax = xUp;
		    if ( ysUp > ysDown ) {
			 yMin = yUp;
			 yMax = yDown;
		    } else {
			 yMin = yDown;
			 yMax = yUp;
		    };
	       } else {
		    xMin = xUp;
		    xMax = xDown;
		    if ( ysUp > ysDown ) {
			 yMin = yUp;
			 yMax = yDown;
		    } else {
			 yMin = yDown;
			 yMax = yUp;
		    };
	       };
	       adjustXScale();
	       adjustYScale();
	       fixSize();
	       // ��������� ����� �������� �������� � ����
	       ZGraphArea ga;
	       ga.xMin = xMin;
	       ga.xMax = xMax;
	       ga.yMin = yMin;
	       ga.yMax = yMax;
	       ga.xsMin = xsMin;
	       ga.xsMax = xsMax;
	       ga.ysMin = ysMin;
	       ga.ysMax = ysMax;
	       zoomStack.resize( curZoom + 1 );
	       zoomStack.push_back( ga );
	       // �������� ������� ���������� ��������
	       zoomIn();
	  };
     } else {

     };

     leftMouseDownActive  = false;
     rightMouseDownActive = false;
     baffleMovedActive    = false;

     update();
}

void ZChart::keyPressEvent( QKeyEvent *event ) {
     switch ( event->key() ) {

     case Qt::Key_Plus:
	  zoomIn();
	  break;
     case Qt::Key_Minus:
	  zoomOut();
	  break;
     case Qt::Key_Left:
	  setXRange( xMin - (( xMax - xMin ) / 10.0 ), xMax - (( xMax - xMin ) / 10.0 ) );
	  adjustXScale();
	  chCurrStack();
	  update();
	  break;
     case Qt::Key_Right:
	  setXRange( xMin + (( xMax - xMin ) / 10.0 ), xMax + (( xMax - xMin ) / 10.0 ) );
	  adjustXScale();
	  chCurrStack();
	  update();
	  break;
     case Qt::Key_Down:
	  setYRange( yMin - (( yMax - yMin ) / 10.0 ), yMax - (( yMax - yMin ) / 10.0 ) );
	  adjustYScale();
	  chCurrStack();
	  update();
	  break;
     case Qt::Key_Up:
	  setYRange( yMin + (( yMax - yMin ) / 10.0 ), yMax + (( yMax - yMin ) / 10.0 ) );
	  adjustYScale();
	  chCurrStack();
	  update();
	  break;
     default:
	  QWidget::keyPressEvent( event );

     }
}

// ������� ���������� �� ����� ��������� ������
void ZChart::zoomIn() {
     if ( curZoom < ( int )zoomStack.size() - 1 ) {
	  ++curZoom;
	  update();
     }
}

// ������� ���������� �� ����� ��������� �����
void ZChart::zoomOut() {
     if ( curZoom > 0 ) {
	  --curZoom;
	  update();
     }
}

// �������� ������� ��������� ��������
void ZChart::chCurrStack() {
     if ( zoomStack.size() == 0 ) {
	  ZGraphArea ga;
	  ga.xMin = xMin;
	  ga.xMax = xMax;
	  ga.yMin = yMin;
	  ga.yMax = yMax;
	  ga.xsMin = xsMin;
	  ga.xsMax = xsMax;
	  ga.ysMin = ysMin;
	  ga.ysMax = ysMax;
	  zoomStack.resize( 1 );
	  zoomStack[0] = ga;
     }
     // �������� ������� �������� �������� � �����
     ZGraphArea ga;
     ga.xMin = xMin;
     ga.xMax = xMax;
     ga.yMin = yMin;
     ga.yMax = yMax;
     ga.xsMin = xsMin;
     ga.xsMax = xsMax;
     ga.ysMin = ysMin;
     ga.ysMax = ysMax;
     zoomStack[curZoom] = ga;
}

// ������������ � ������
void ZChart::scaleIn() {
     setXRange( xMin + (( xMax - xMin ) / 10.0 ), xMax - (( xMax - xMin ) / 10.0 ) );
     setYRange( yMin + (( yMax - yMin ) / 10.0 ), yMax - (( yMax - yMin ) / 10.0 ) );
     adjustXScale();
     adjustYScale();
     chCurrStack();
     update();
}

// ���������� �� ������
void ZChart::scaleOut() {
     setXRange( xMin - (( xMax - xMin ) / 10.0 ), xMax + (( xMax - xMin ) / 10.0 ) );
     setYRange( yMin - (( yMax - yMin ) / 10.0 ), yMax + (( yMax - yMin ) / 10.0 ) );
     adjustXScale();
     adjustYScale();
     chCurrStack();
     update();
}

// ���������� ��������� ������ ����
void ZChart::wheelEvent( QWheelEvent *event ) {
     if ( event->delta() > 0 ) {
	  scaleIn();
     } else {
	  scaleOut();
     }
}

// ����������� ��� ������ � ����� ���������� ������� �� �����
bool ZChart::event( QEvent *event ) {
     if ( event->type() == QEvent::ToolTip ) {
	  QHelpEvent *helpEvent = static_cast<QHelpEvent *>( event );
	  // ���� ������� ��������� � ����������� ������, �� ������� ����������
	  if ( isTitleTruncated
	       && ( helpEvent->pos().x() >= titleLeft )
	       && ( helpEvent->pos().x() <= titleLeft + titleWidth )
	       && ( helpEvent->pos().y() >= titleTop )
	       && ( helpEvent->pos().y() <= titleTop + titleHeight ) ) {
	       QToolTip::showText( helpEvent->globalPos(), titleText );
	  }
     }
     return QWidget::event( event );

}
