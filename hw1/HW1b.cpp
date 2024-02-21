// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW1b.cpp - HW1b class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW1b.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::HW1b:
//
// HW1b constructor.
//
HW1b::HW1b(const QGLFormat &glf, QWidget *parent)
	: HW(glf, parent)
{
	// init vars
	m_theta		= 0;
	m_subdivisions	= 4;
	m_updateColor	= 1;
	m_twist		= 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW1b::initializeGL()
{
	// init vertex and color buffers
	initBuffers();

	// init state variables
	glClearColor(0.0, 0.0, 0.0, 1.0);	// set background color
	glColor3f   (1.0, 1.0, 1.0);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW1b::resizeGL(int w, int h)
{
	// PUT YOUR CODE HERE
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::paintGL:
//
// Update GL scene.
//
void
HW1b::paintGL()
{
	// PUT YOUR CODE HERE
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW1b::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 1b");
	groupBox->setStyleSheet(GroupBoxStyle);
	// create labels
	QLabel *label[2];
	label[0] = new QLabel("Theta");
	label[1] = new QLabel("Subdivide");

	// create sliders
	m_sliderTheta  = new QSlider(Qt::Horizontal);
	m_sliderSubdiv = new QSlider(Qt::Horizontal);

	// init sliders
	m_sliderTheta ->setRange(0, 360);
	m_sliderTheta ->setValue(0);
	m_sliderSubdiv->setRange(0, 6);
	m_sliderSubdiv->setValue(m_subdivisions);

	// create spinBoxes
	m_spinBoxTheta = new QSpinBox;
	m_spinBoxTheta->setRange(0, 360);
	m_spinBoxTheta->setValue(0);
	m_spinBoxSubdiv = new QSpinBox;
	m_spinBoxSubdiv->setRange(0, 6);
	m_spinBoxSubdiv->setValue(m_subdivisions);

	// init checkbox
	m_checkBoxTwist = new QCheckBox("Twist");
	m_checkBoxTwist->setChecked(m_twist);

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0],	   0, 0);
	layout->addWidget(m_sliderTheta,   0, 1);
	layout->addWidget(m_spinBoxTheta,  0, 2);
	layout->addWidget(label[1],	   1, 0);
	layout->addWidget(m_sliderSubdiv,  1, 1);
	layout->addWidget(m_spinBoxSubdiv, 1, 2);
	layout->addWidget(m_checkBoxTwist, 2, 0);

	// assign layout to group box
	groupBox->setLayout(layout);

	// init signal/slot connections
	connect(m_sliderTheta  , SIGNAL(valueChanged(int)), this, SLOT(changeTheta (int)));
	connect(m_sliderSubdiv , SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
	connect(m_spinBoxTheta,  SIGNAL(valueChanged(int)), this, SLOT(changeTheta (int)));
	connect(m_spinBoxSubdiv, SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
	connect(m_checkBoxTwist, SIGNAL(stateChanged(int)), this, SLOT(changeTwist (int)));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::reset:
//
// Reset parameters.
//
void
HW1b::reset()
{
	// reset parameters
	m_theta		= 0;
	m_subdivisions	= 4;
	m_updateColor	= true;
	m_twist		= true;
	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(0.0f);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(0.0f);
	m_spinBoxTheta->blockSignals(false);
	m_sliderSubdiv->blockSignals(true);
	m_sliderSubdiv->setValue(m_subdivisions);
	m_sliderSubdiv->blockSignals(false);

	m_spinBoxSubdiv->blockSignals(true);
	m_spinBoxSubdiv->setValue(m_subdivisions);
	m_spinBoxSubdiv->blockSignals(false);

	// reset twist checkbox
	m_checkBoxTwist->setChecked(m_twist);

	// redraw
	m_points.clear();
	m_colors.clear();
	initBuffers();
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::initBuffers:
//
// Initialize vertex buffer.
//
void
HW1b::initBuffers()
{
	const QVector2D vertices[] = {
		 QVector2D( 0.0f ,  0.75f),
		 QVector2D( 0.65f, -0.375f),
		 QVector2D(-0.65f, -0.375f)
	};

	// recursively subdivide triangle into triangular facets;
	// store vertex positions and colors in m_points and m_colors, respectively
	divideTriangle(vertices[0], vertices[1], vertices[2], m_subdivisions);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::divideTriangle:
//
// Recursive subdivision of triangle (a,b,c). Recurse count times.
//
void
HW1b::divideTriangle(vec2 a, vec2 b, vec2 c, int count)
{
	// PUT YOUR CODE HERE
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::triangle:
//
// Push positions and colors of the three triangle vertices.
//
void
HW1b::triangle(vec2 a, vec2 b, vec2 c)
{
	// init color
	if(m_updateColor) {
		m_colors.push_back(vec3((float) rand()/RAND_MAX,
					(float) rand()/RAND_MAX,
					(float) rand()/RAND_MAX));
	}

	// init geometry
	m_points.push_back(rotTwist(a));
	m_points.push_back(rotTwist(b));
	m_points.push_back(rotTwist(c));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::rotTwist:
//
// Rotate point p based on rotation angle Theta and distance from origin (for Twist = true)
//
vec2
HW1b::rotTwist(vec2 p)
{
	float d = m_twist ? sqrt(p[0]*p[0] + p[1]*p[1]) : 1;
	float sinTheta = sin(d*m_theta);
	float cosTheta = cos(d*m_theta);
	return vec2(p[0]*cosTheta - p[1]*sinTheta, p[0]*sinTheta + p[1]*cosTheta);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::changeTheta:
//
// Slot function to change rotation angle.
//
void
HW1b::changeTheta(int angle)
{
	// update slider and spinbox
	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(angle);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(angle);
	m_spinBoxTheta->blockSignals(false);

	// init vars
	m_theta = angle * (M_PI / 180.);	// convert angle to radians
	m_updateColor = 0;			// do not update color during rotation

	// redraw
	m_points.clear();
	initBuffers();
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::changeSubdiv:
//
// Slot function to change number of recursive subdivisions.
//
void
HW1b::changeSubdiv(int subdivisions)
{
	// update slider and spinbox
	m_sliderSubdiv->blockSignals(true);
	m_sliderSubdiv->setValue(subdivisions);
	m_sliderSubdiv->blockSignals(false);

	m_spinBoxSubdiv->blockSignals(true);
	m_spinBoxSubdiv->setValue(subdivisions);
	m_spinBoxSubdiv->blockSignals(false);

	// init vars
	m_subdivisions = subdivisions;
	m_updateColor  = 1;

	// redraw
	m_points.clear();
	m_colors.clear();
	initBuffers();
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1b::changeTwist:
//
// Slot function to turn on/off m_twist;
//
void
HW1b::changeTwist(int twist)
{
	// init vars
	m_twist = twist;
	m_updateColor = 0;

	// redraw
	m_points.clear();
	initBuffers();
	updateGL();
}
