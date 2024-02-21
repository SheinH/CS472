// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW0a.cpp - HW0a class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW0a.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::HW0a:
//
// HW0a constructor.
//
HW0a::HW0a(const QGLFormat &glf, QWidget *parent)
	 : HW(glf, parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW0a::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init state variables
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// set background color
	glColor3f   (1.0F, 1.0f, 1.0f);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW0a::resizeGL(int w, int h)
{
	// compute aspect ratio
	float xmax, ymax;
	float ar = (float) w / h;
	if(ar > 1.0) {		// wide screen
		xmax = ar;
		ymax = 1.;
	} else {		// tall screen
		xmax = 1.;
		ymax = 1/ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// init viewing coordinates for orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::paintGL:
//
// Update GL scene.
//
void
HW0a::paintGL()
{
	// clear canvas with background values
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// experiment with transformation by uncommenting next line
	// glRotatef(45, 0, 0, 1);

	// define polygon
	glBegin(GL_POLYGON);
		glVertex2f(-0.25f, -0.25f);
		glVertex2f(-0.25f,  0.25f);
		glVertex2f(0.25f, 0.25f);
		glVertex2f(0.25f, -0.25f);
	glEnd();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0a::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW0a::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 0a");

	return(groupBox);
}
