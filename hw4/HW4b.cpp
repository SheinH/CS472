// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW4b.cpp - HW4b class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW4b.h"


#define PII		3.1415926535897931160E0
#define PI2		6.2831853071795862320E0
#define PI_2		1.5707963267948965580E0
#define DEGtoRAD	0.0174532927777777777E0
#define RADtoDEG	57.295778666661658617E0



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4b::HW4b:
//
// HW4b constructor.
//
HW4b::HW4b(const QGLFormat &glf, QWidget *parent) : HW(glf, parent)
{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
	m_midlight[0] = .8f; 
	m_midlight[1] = .8f; 
	m_midlight[2] = .8f; 
	m_midlight[3] = .8f;

	m_gray[0]     = .5f; 
	m_gray[1]     = .5f; 
	m_gray[2]     = .5f; 
	m_gray[3]     = .5f;

	// define light position
	m_light_pos[0] = 8.0f; 
	m_light_pos[1] = 8.0f; 
	m_light_pos[2] = 8.0f; 
	m_light_pos[3] = 8.0f;

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4b::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW4b::initializeGL()
{
	// init state variables
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);	// set background color
	glColor3f   (1.0f, 1.0f, 1.0f);		// set foreground color
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthRange(0.01f, 1000.0f);

	// enable light0 and set its position and colors
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, m_light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  m_midlight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_midlight);
	glEnable(GL_NORMALIZE);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_time = 0;
	m_timer->start(5);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4b::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW4b::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	double fovy   = 45;
	double aspect = 1.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	m_projection.setToIdentity();
	m_projection.perspective(fovy, aspect*w/h, 0.01f, 1000.0f);

	// Add to current matrix
	glMultMatrixf(m_projection.constData());
	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4b::paintGL:
//
// Update GL scene.
//
void
HW4b::paintGL()
{
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// clear transformation matrix
	GLfloat m[16] ={0};

	// init scale parameters for sphere
	float	timescale = 0.5;
	float	height = .8+2*(1+sin(m_time*DEGtoRAD*timescale));
	float	s1 = 2 - fabs(sin(m_time*DEGtoRAD*timescale));
	float	s2 = 1.2 / s1;

	// compute nlp[4] light source position; will rotate it around the y-axis 
	GLfloat nlp[4], r = m_light_pos[0];
	float	angle = m_time*.01;
	nlp[1] = m_light_pos[1];
	nlp[0] = r*cos(angle*DEGtoRAD);
	nlp[2] = r*sin(angle*DEGtoRAD);
	nlp[3] = 1.0;

	// init projection matrix with identity matrix and a non-zero m[7] entry
	// based on light position component
// PUT YOUR CODE HERE

	// setup camera view
	glLoadIdentity();

	// define eye position and where we are looking at
	vec3 eye(10.0f, 30.0f, 40.0f);
	vec3 origin(0.0f, 0.0f, 0.0f);
	vec3 dir = (origin - eye).normalized();
	vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 right   = vec3::crossProduct(dir, worldUp).normalized();
	vec3 up      = vec3::crossProduct(right, dir).normalized();
	m_cameraView.setToIdentity();
	m_cameraView.lookAt(eye, origin, up);
	glMultMatrixf(m_cameraView.constData());

	// update the position of light0
// PUT YOUR CODE HERE

	// enable lighting and color material
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialf(GL_FRONT, GL_SHININESS, 32.f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_gray);

	// draw the translated/scaled sphere object
// PUT YOUR CODE HERE

	// disable lighting
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	// draw object's shadow (projected onto the xz plane)
// PUT YOUR CODE HERE

	// draw the light source
// PUT YOUR CODE HERE

	// draw the xz floor
	glColor4f(.5f, .5f, .5f, 1);
	glBegin(GL_QUADS);
	float w = 25; //floor half width
	glVertex3f(-w, -0.05f, -w);
	glVertex3f( w, -0.05f, -w);
	glVertex3f( w, -0.05f,  w);
	glVertex3f(-w, -0.05f,  w);
	glEnd();

	glFlush();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4b::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW4b::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 4a");
	groupBox->setStyleSheet(GroupBoxStyle);
	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4b::drawSphere::
//
// Draw unit sphere
//
void 
HW4b::drawSphere(int lats, int longs)
{
// PUT YOUR CODE HERE
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4b::timerEvent:
//
// Virtual function called when timer times out.
//
//
void
HW4b::timeOut() 
{

	// pause animation to reset grid without interruption by timer
	m_timer->stop();
	m_time += 3.0f;
	updateGL();

	// restart animation
	m_timer->start(5);
}
