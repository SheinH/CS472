// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW1a.h - Header file for HW1a class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW1A_H
#define HW1A_H

#include "HW.h"

// ----------------------------------------------------------------------
// standard include files
//

class HW1a : public HW 
{
	Q_OBJECT
public:
	HW1a(const QGLFormat &glf, QWidget *parent=0);	// constructor
	QGroupBox*	controlPanel	();		// create control panel

public slots:

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene

private:
	int		m_winW;				// window width
	int		m_winH;				// window height
};

#endif // HW1A_H
