// Time-stamp: <zchart.h - 01:37:39 04.07.2006>
/******************************************************************
 *** ������ ������������ ����� ����������� �������� � ��������  ***
 ***         ������������� � ������� ��������� ������           ***
 ******************************************************************/

/* code: ������ ������� <knzsoft@mail.ru> */
/* some changes & improvements: ���������� ����� <zorg1331@mail.ru> */

#ifndef ZCHART_H
#define ZCHART_H

#include <QWidget>
#include <QPen>

#include "zframes.h"

// �������� ���� ������� (bt - border type)
#define btNoneNone 0
#define btRaisedNone 1
#define btRaisedLowered 2

// ���������, ������������ �� ������ �������� �������� � ������
// ������� ������, ��� ����, ����� ������ ������� ����� ����������
// ���� ������ � ������ ��������
struct ZGraphArea {
     long double xMin, xMax;
     long double yMin, yMax;
     int xsMin, xsMax;
     int ysMin, ysMax;
     QPainter *painter;
};

// ����� ������ ������ ��� ������� ZChart
class ZChartSeries {
public:
     QString name;
     QColor color;
     ZChartSeries( const QString &aName );
     ZChartSeries( const QString &aName, const QColor &aColor );
     virtual ~ZChartSeries() {}
     void setGraphArea( const ZGraphArea &g );
     void setColor( const QColor &c );
     virtual void draw( void );
     // ��� ���� �������������������
     virtual double getMaxX( void );
     virtual double getMaxY( void );
     virtual double getMinX( void );
     virtual double getMinY( void );
     virtual double getMaxYy( double xl,double xr );
     virtual double getMinYy( double xl,double xr );
private:
     ZGraphArea ga;

     // ����� ���������� �� ��������� "ga" ��� �������� ���������
     long double xMin, xMax;
     long double yMin, yMax;
     int xsMin, xsMax;
     int ysMin, ysMax;
     int xPenPos, yPenPos;

     // ����������� ������������ ��������
     long double xScreen;
     long double yScreen;
     bool isLastPointVisible;

protected:
     int xToScreen( long double x );
     int yToScreen( long double y );
     void moveTo( int x, int y );
     void lineTo( int x, int y );
     QPainter *painter;
};

struct LongDoublePoint {
     long double x, y;
};

class ZSimpleSeries: public ZChartSeries {
public:
     ZSimpleSeries( const QString &aName );
     ZSimpleSeries( const QString &aName, const QColor &aColor );
     virtual ~ZSimpleSeries() {}
     virtual void draw( void );
     void add( long double x, long double y );
     // ���������� ��������� �������� �����, ��������������� ������� ��������
     double getYValue( double x );
     void clear( void );
     // ��� ���� �������������������
     virtual double getMaxX();
     virtual double getMaxY();
     virtual double getMinX();
     virtual double getMinY();
     virtual double getMaxYy( double xl,double xr );
     virtual double getMinYy( double xl,double xr );

private:
     QVector <LongDoublePoint> point;
};

class ZLimitSeries: public ZChartSeries {
public:
     ZLimitSeries( const QString &aName );
     ZLimitSeries( const QString &aName, const QColor &aColor,
		   double baseLine, bool isFilled );
     virtual ~ZLimitSeries() {}
     virtual void draw( void );
     void add( long double x, long double y );
     // ���������� ��������� �������� �����, ��������������� ������� ��������
     double getYValue( double x );
     void clear( void );
     // ��� ���� �������������������
     virtual double getMaxX();
     virtual double getMaxY();
     virtual double getMinX();
     virtual double getMinY();
     virtual double getMaxYy( double xl,double xr );
     virtual double getMinYy( double xl,double xr );
private:
     QVector <LongDoublePoint> point;
     double baseLine;
     bool isFilled;
     double x_old, y_old;
};

// ����� �������� �������
class ZChart: public QWidget {
     Q_OBJECT

	  public:
     ZChart( QWidget *parent = 0 );

     void draw( void );
     void setAxesName( const QString &xName, const QString &yName );
     void setZeroLabel( const QString &zeroLabel );
     void setBorderType( int aBorderType );
     void setGraphPanelBorderType( int aGraphPanelBorderType );
     void setMargin( int aMargin );
     void setSpacing( int aSpacing );
     void setFirstText( const QString &s );
     void setSecondText( const QString &s );
     void setFirstStatusPanelWidth( int w );
     void setTitle( const QString &s );
     void setSettings( QFont _mainFont, QFont _boldFont, QPen _axesPen, QPen _gridPen,
		       QPen _legendFramePen, QPen _rubberPen, QPen _zeroPen );
     void showLegend( bool show );
     void addSeries( ZChartSeries *s );
     void addSeriesAt( ZChartSeries *s, int index );
     void removeSeries();
     void removeSeriesAt( int index );

     // ��������� ���������� ����
     void setXRange( long double aXMin, long double aXMax );
     void setYRange( long double aYMin, long double aYMax );
     void setFakeXRange( long double aFXMin, long double aFXMax );

     // �������� �������������� �������������������
     void setAutoscale( bool _autoscale );
     void autoscale();
     void autoscale1(double xl,double xr);

     // ��������� ������ :)
     bool isLightGraph;

signals:
     // ���� �������� ������
     void clickCoord( double x, double y );
     void clickCoordX( double x );
     void clickCoordY( double y );

     public slots:
     void zoomIn();
     void zoomOut();

//private slots:
     // �������������������
//  void autoscale();

private:
     // �������� ������ ��� ��������� �����
     ZFrames *frames;
     QList <ZChartSeries *> series;

     // ��������� ����������� ��������
     QFont mainFont;		// �������� �����
     QFont boldFont;		// ������ �����
     QPen axesPen;			// ����� ��������� ����
     QPen gridPen;			// ����� ��������� �����
     QPen legendFramePen;		// ����� ��������� ����� �������
     QPen rubberPen;		// ����� ��������� ����� ���������
     QPen zeroPen;			// ����� ������� �������
     QPen zoomButtonsPen;		// ����� ������� ������ ����������� �� ������� ���������

     int borderType;  // ��� ������ �������
     int margin;      // ������ ������� �����
     int spacing;     // ������ ���������� �����
     int fontSize;    // ������ ������������� ������

     // ���������� ��� ������ ���������
     int titleLeft, titleTop, titleWidth, titleHeight; // ��������� ������
     int xTitleText, yTitleText;                    // ���������� ������ ������
     QString titleText;   // ����� ���������
     QString xAxName, yAxName;  // ������������ ����
     QString zeroLabel;
     int titleBorderType; // ��� ������� ������ ���������

     bool showLegendFlag; // ���������� �� ������� � ��������� ����������� ��������
     bool isTitleTruncated; // ������� �� �����

     // ���������� ��� ������ �������
     int sbLeft, sbTop, sbWidth, sbHeight;          // ��������� ������
     int sbBaffles[1];                              // ������ ����������� � ������ �������
     int xFirstText, xSecondText, yStatusBarText;   // ���������� ������ ������
     QString sbFirstText;                           // ����� ������ ������ ������ �������
     QString sbSecondText;                          // ����� ������ ������ ������ �������

     // ���������� ��� ����������� ������ �������
     int graphPanelBorderType;
     int gpLeft, gpTop, gpWidth, gpHeight;
     // ���������� ��� ����������� ����
     int xScaleLeft, xScaleTop, xScaleWidth, xScaleHeight;
     int yScaleLeft, yScaleTop, yScaleWidth, yScaleHeight;
     // ���������� ��� ����������� ������� �������
     int axisLeft, axisTop, axisWidth, axisHeight;

     // ���������� ��������� � ����������� ����
     long double xMin, xMax;        // �������������� �������� ����� X
     long double xMinOrig, xMaxOrig; // ������������ �������� ����� X
     long double yMin, yMax;        // �������������� �������� ����� Y
     long double yMinOrig, yMaxOrig; // ������������ �������� ����� Y
     long double xMinTick;          // ����������� �������� ����� �� ����� X
     long double yMinTick;          // ����������� �������� ����� �� ����� Y
     long double xStepTick;         // ��� ����� X
     long double yStepTick;         // ��� ����� Y
     long double xFMin, xFMax;
     int xsMin, xsMax;              // ����������� � ������������ �������� x-���������� � ��������
     int ysMin, ysMax;              // ����������� � ������������ �������� y-���������� � ��������
     // �������� ������������ ���� - ���������� ������� 10, ������������ � �����
     int xPrecision;
     int yPrecision;
     bool autoscaleFlag;

     // ���������� ��������� � ���������� ������� �����
     bool leftMouseDownActive;  // ������ ����� ������� ����
     bool rightMouseDownActive; // ������ ������ ������� ����
     bool baffleMovedActive;    // ������� ����������� � ������ �������
     long double xDown, yDown;  // �������� ���������� ������� ����
     long double xUp, yUp;      // �������� ���������� ���������� ����
     int xsDown, ysDown;        // �������� ���������� ������� ����
     int xsUp, ysUp;            // �������� ���������� ���������� ����

     // ������ ����������� �������� ����� � ���� ����
     void adjustXScale( void );
     void adjustYScale( void );
     // ����������� �������� � �������� ���������
     int xToScreen( long double x );
     int yToScreen( long double y );
     long double xToReal( int x );
     long double yToReal( int y );

     void fixSize( void );      // ������� ������� ��������� ������ ��������� �������� �������
     int  fixXScaleHeight( void );// ������� ������ �������������� �����
     int  fixYScaleWidth( void ); // ������� ������ ������������ �����
     void drawTitle( void );    // ������ ������ ���������
     void drawStatusBar( void );  // ������ ������ �������
     void drawFirstText( void );  // ������ ����� ������ ������ ������ �������
     void drawSecondText( void ); // ������ ����� ������ ������ ������ �������
     void drawGraphPanel( void ); // ������ ������ ��������
     void drawXScale( QPainter *p, int xOffset, int yOffset );   // ������ �������������� �����
     void drawYScale( QPainter *p, int xOffset, int yOffset );   // ������ ������������ �����
     void drawAxes( QPainter *p, int xOffset, int yOffset );     // ������ ���
     void drawGrid( QPainter *p, int xOffset, int yOffset );     // ������ �����
     void drawSeries( QPainter *p, int xOffset, int yOffset );   // ������ ������
     void updateRubberBandRegion( void );                        // ��������� ������� �����
     void drawRubberBand( void );                                // ������ �����
     void drawLegend( QPainter *p );   // ������ ������� � ��������� ����������� �����
     void drawZoomButtons( void ); // ������ ������, � ������� ������� ���� ����������� �� ������� ��������� ��������
     void drawAutoscaleButton( void ); // ������ ������ �������������������

     // ��������, ����������� �� ���������� x,y ������� ����������� � ������ �������
     bool isStatusBarFirstBaffle( int x, int y );
     // ���������� ��������� �������� ������� ����������� � ������ �������, ��� 0
     int  isPositionStatusBarFirstBafflePossible( int x );
     // ����������� �� ���������� ������ �����
     bool isZoomOutButton( int x, int y );
     // ����������� �� ���������� ������ ������
     bool isZoomInButton( int x, int y );
     // ����������� �� ���������� ������ �������������������
     bool isAutoscaleButton( int x, int y );
     // ���� �� �������� �����
     bool rubberBandIsShown;
     // ������������� �����
     QRect rubberBandRect;

     // ���������� ������ ������� ��������� ��������
     int curZoom;

     // ������ �� ���������� �� ��������� �������
     // ��� �����, ����� ������� ������� �� ��������� ��������
     QVector <ZGraphArea> zoomStack;

     // �������� ������� ��������� �������� � �����
     void chCurrStack();

     // �����������/��������� ������� � ������
     void scaleIn();
     void scaleOut();

protected:
     virtual bool event( QEvent *event );
     virtual void paintEvent( QPaintEvent *ev );
     virtual void resizeEvent( QResizeEvent *ev );
     virtual void mouseMoveEvent( QMouseEvent *ev );
     virtual void mousePressEvent( QMouseEvent *ev );
     virtual void mouseReleaseEvent( QMouseEvent *ev );
     virtual void wheelEvent( QWheelEvent *event );
     virtual void keyPressEvent( QKeyEvent *event );
};

#endif
