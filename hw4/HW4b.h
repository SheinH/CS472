// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW4b.h - Header file for HW4b class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW4B_H
#define HW4B_H

#include "HW.h"

// ----------------------------------------------------------------------
// standard include files
//

class HW4b : public HW 
{
	Q_OBJECT
public:
	// constructor
	HW4b		(const QGLFormat &glf, QWidget *parent = 0); 
	QGroupBox*	controlPanel	();		// create control panel

public slots:
	void		timeOut		();

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene
	void		drawSphere	(int, int);


private:
	int		 m_winW;			// window width
	int		 m_winH;			// window height
	QMatrix4x4	 m_projection;			// 4x4 projection matrix
	QMatrix4x4	 m_cameraView;			// 4x4 view  matrix
	QTimer		*m_timer;
	double		 m_time;
	GLfloat		 m_midlight[4];
	GLfloat		 m_gray[4];
	GLfloat		 m_light_pos[4];		// define light position 
};

#endif // HW4B_H
