// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW2a.h - Header file for HW2a class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW2A_H
#define HW2A_H


#include "HW.h"


// ----------------------------------------------------------------------
// standard include files
//

class HW2a : public HW 
{
	Q_OBJECT
public:
	// constructor
	HW2a		(const QGLFormat &glf, QWidget *parent = 0);
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters
	void		initVertexBuffer();		// init vertices
	void		initShaders();			// init shaders

public slots:

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene

private:
	int		 m_winW;			// window width
	int		 m_winH;			// window height
	int		 m_vertNum;			// number of vertices
	QMatrix4x4	 m_projection;			// 4x4 projection matrix
};

#endif // HW2A_H
