// Time-stamp: <zframes.cpp - 01:23:15 04.07.2006>

#include "zframes.h"

#include <QPainter>

ZFrames::ZFrames( QWidget *aDrawField ) {
	drawField = aDrawField;
}

void ZFrames::drawRaisedNoneFrame( int left, int top, int width, int height ) {
	QPainter p( drawField );

	p.setPen( Qt::SolidLine );
	p.setPen( Qt::white );
	p.drawLine( left, top + height - 2, left, top );
	p.drawLine( left, top, left + width - 2, top );
	p.setPen( Qt::gray );
	p.drawLine( left + width - 1, top, left + width - 1, top + height - 1 );
	p.drawLine( left + width - 1, top + height - 1, left, top + height - 1 );
	p.end();
}

void ZFrames::drawRaisedLoweredFrame( int left, int top, int width, int height ) {
	QPainter p( drawField );

	p.setPen( Qt::SolidLine );
	p.setPen( Qt::white );
	p.drawRect( left, top, width - 2, height - 2 );
	p.setPen( Qt::gray );
	p.drawLine( left + 1, top + height - 3, left + 1, top + 1 );
	p.drawLine( left + 1, top + 1, left + width - 3, top + 1 );
	p.drawLine( left, top + height - 1, left + width - 1, top + height - 1 );
	p.drawLine( left + width - 1, top + height - 1, left + width - 1, top );
	p.end();
}

void ZFrames::drawStatusBarFrame( int left, int top, int width, int height, int *baffles ) {
	QPainter p( drawField );
	int n = sizeof( baffles ) / sizeof( baffles[0] );

	p.setPen( Qt::SolidLine );
	p.setPen( Qt::gray );
	p.drawRect( left, top, width - 1, height - 1 );
	p.setPen( Qt::gray );
	p.drawLine( left + 1, top + height - 3, left + 1, top + 1 );
	p.drawLine( left + 1, top + 1, left + width - 3, top + 1 );
	p.setPen( Qt::white );
	p.drawLine( left, top + height - 2, left + width - 2, top + height - 2 );
	p.drawLine( left + width - 2, top + height - 2, left + width - 2, top );
	p.setPen( Qt::gray );
	for ( int i = 0; i < n; i++ ) {
		p.drawLine( left + baffles[i], top, left + baffles[i], top + height - 3 );
	}
	p.setPen( Qt::white );
	for ( int i = 0; i < n; i++ ) {
		p.drawLine( left + baffles[i] - 1, top + 1, left + baffles[i] - 1, top + height - 2 );
	}
	p.setPen( Qt::gray );
	for ( int i = 0; i < n; i++ ) {
		p.drawLine( left + baffles[i] + 1, top + 1, left + baffles[i] + 1, top + height - 3 );
	}
	p.end();
}
