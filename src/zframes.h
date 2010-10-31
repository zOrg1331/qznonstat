// Time-stamp: <zframes.h - 01:23:24 04.07.2006>

#ifndef ZFRAMES_H
#define ZFRAMES_H

#include <qwidget.h>

class ZFrames {
	public:
		ZFrames( QWidget *aDrawField );
		void drawRaisedNoneFrame( int left, int top, int width, int height );
		void drawRaisedLoweredFrame( int left, int top, int width, int height );
		void drawStatusBarFrame( int left, int top, int width, int height, int *baffles );
	private:
		QWidget *drawField;
};

#endif
