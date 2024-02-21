// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// Dummy.cpp - Dummy class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "dummy.h"



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::Dummy:
//
// Dummy constructor.
//
Dummy::Dummy(const QGLFormat &glf, QWidget *parent) : HW(glf, parent)
{
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
Dummy::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init state variables
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);	// set background color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
Dummy::resizeGL(int w, int h)
{
	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::paintGL:
//
// Update GL scene.
//
void
Dummy::paintGL()
{
	// clear the screen 
	glClear(GL_COLOR_BUFFER_BIT);	
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Dummy::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
Dummy::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("CS472 Homework");
	groupBox->setStyleSheet(GroupBoxStyle);
	return(groupBox);
}
