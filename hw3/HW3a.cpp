// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW3a.cpp - HW3a class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW3a.h"

// shader ID
enum {TEXTURE, WIREFRAME};

// uniform ID
enum {
	MV,
	PROJ,
	THETA,
	SUBDIV,
	TWIST,
	SAMPLER
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::HW3a:
//
// HW3a constructor.
//
HW3a::HW3a(const QGLFormat &glf, QWidget *parent) : HW(glf, parent)
{
	// init vars
	m_theta = 0.0f;
	m_subdivisions = 4;
	m_twist = true;
	m_wire  = true;
	m_modelview .setToIdentity();
	m_projection.setToIdentity();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW3a::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init texture
	initTexture();

	// init vertex and fragment shaders
	initShaders();

	// initialize vertex buffer and copy data to vertex shader
	glGenBuffers(1, &m_vertexBuffer);	// create vertex buffer
	glGenBuffers(1, &m_texBuffer);		// create texture coord buffer
	initVertexBuffer();

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// init state variables
	glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f   (1.0, 1.0, 0.0);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW3a::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float ar = (float) w / h;
	float xmax, ymax;
	if(ar > 1.0) {		// wide screen
		xmax = ar;
		ymax = 1.;
	}
	else {			// tall screen
		xmax = 1.;
		ymax = 1 / ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// init viewing coordinates for orthographic projection
	m_projection.setToIdentity();
	m_projection.ortho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::paintGL:
//
// Update GL scene.
//
void
HW3a::paintGL()
{
	// clear canvas with background color
	glClear(GL_COLOR_BUFFER_BIT);

	// bind vertex buffer to the GPU; enable buffer to be copied to the
	// attribute vertex variable and specify data format
	// PUT YOUR CODE HERE

	// bind texture coord buffer to the GPU; enable buffer to be copied to the
	// attribute texture coordinate variable and specify data format
	// PUT YOUR CODE HERE

	// use texture glsl program
	// PUT YOUR CODE HERE

	// pass parameters to vertex shader
	// PUT YOUR CODE HERE

	// draw texture mapped triangles
	// PUT YOUR CODE HERE

	glLineWidth(1.5f);

	// draw wireframe, if necessary
	if(m_wire) {
		// PUT YOUR CODE HERE
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW3a::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 3a");
	groupBox->setStyleSheet(GroupBoxStyle);
	// create labels
	QLabel *label[2];
	label[0] = new QLabel("Theta");
	label[1] = new QLabel("Subdivide");

	// create sliders
	m_sliderTheta  = new QSlider(Qt::Horizontal);
	m_sliderSubdiv = new QSlider(Qt::Horizontal);

	// init sliders
	m_sliderTheta->setRange(0, 360);
	m_sliderTheta->setValue(0);
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

	// init wireframe checkbox
	m_checkBoxWire = new QCheckBox("Wireframe");
	m_checkBoxWire ->setChecked(m_wire );

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0],	   0, 0);
	layout->addWidget(m_sliderTheta,   0, 1);
	layout->addWidget(m_spinBoxTheta,  0, 2);
	layout->addWidget(label[1],	   1, 0);
	layout->addWidget(m_sliderSubdiv,  1, 1);
	layout->addWidget(m_spinBoxSubdiv, 1, 2);
	layout->addWidget(m_checkBoxTwist, 2, 0);
	layout->addWidget(m_checkBoxWire,  3, 0);

	// assign layout to group box
	groupBox->setLayout(layout);

	// init signal/slot connections
	connect(m_sliderTheta,   SIGNAL(valueChanged(int)), this, SLOT(changeTheta (int)));
	connect(m_sliderSubdiv,  SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
	connect(m_spinBoxTheta,  SIGNAL(valueChanged(int)), this, SLOT(changeTheta (int)));
	connect(m_spinBoxSubdiv, SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
	connect(m_checkBoxTwist, SIGNAL(stateChanged(int)), this, SLOT(changeTwist (int)));
	connect(m_checkBoxWire,  SIGNAL(stateChanged(int)), this, SLOT(changeWire  (int)));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::reset:
//
// Reset parameters.
//
void
HW3a::reset()
{
	// reset parameters
	m_theta		= 0.0f;
	m_subdivisions	= 4;
	m_twist		= true;
	m_wire		= true;

	// reset sliders and spinboxes
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

	// reset checkboxes
	m_checkBoxTwist->setChecked(m_twist);
	m_checkBoxWire ->setChecked(m_wire );

	// recompute geometry
	initVertexBuffer();

	// reset 4x4 modelview matrix
	m_modelview.setToIdentity();

	// redraw with default values
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::initTexture:
//
// Initialize texture.
//
void
HW3a::initTexture()
{
	// read image from file
	m_image.load(QString(":/mandrill.jpg"));

	// convert jpg to GL formatted image
	QImage qImage = QGLWidget::convertToGLFormat(m_image);

	// init vars
	int w = qImage.width ();
	int h = qImage.height();

	// generate texture name and bind it
	glActiveTexture(0);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW3a::initShaders()
{

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_Modelview" ] = MV;
	uniforms["u_Projection"] = PROJ;
	uniforms["u_Theta"     ] = THETA;
	uniforms["u_Twist"     ] = TWIST;
	uniforms["u_Sampler"   ] = SAMPLER;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(TEXTURE,   QString(":/hw3/vshader3a1.glsl"), QString(":/hw3/fshader3a1.glsl"), uniforms);

	// reset uniform hash table for next shader
	uniforms.clear();
	uniforms["u_Modelview" ] = MV;
	uniforms["u_Projection"] = PROJ;
	uniforms["u_Theta"     ] = THETA;
	uniforms["u_Twist"     ] = TWIST;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(WIREFRAME, QString(":/hw3/vshader3a2.glsl"), QString(":/hw3/fshader3a2.glsl"), uniforms);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
HW3a::initVertexBuffer()
{
	// init geometry data
	const vec2 vertices[] = {
		vec2( 0.0f,   0.75f ),
		vec2( 0.65f, -0.375f),
		vec2(-0.65f, -0.375f)
	};

	// PUT YOUR CODE HERE
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::divideTriangle:
//
// Recursive subdivision of triangle (a,b,c). Recurse count times.
//
void
HW3a::divideTriangle(vec2 a, vec2 b, vec2 c, int count)
{
	// PUT YOUR CODE HERE
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::triangle:
//
// Push positions and colors of the three triangle vertices.
//
void
HW3a::triangle(vec2 v1, vec2 v2, vec2 v3)
{
	// init geometry
	m_points.push_back(v1);
	m_points.push_back(v2);
	m_points.push_back(v3);

	// init texture coordinates
	m_coords.push_back(vec2((v1.x()+.65)/1.3, (v1.y()+.375)/1.125));
	m_coords.push_back(vec2((v2.x()+.65)/1.3, (v2.y()+.375)/1.125));
	m_coords.push_back(vec2((v3.x()+.65)/1.3, (v3.y()+.375)/1.125));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::changeTheta:
//
// Slot function to change rotation angle.
//
void
HW3a::changeTheta(int angle)
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

	// update model's rotation matrix
	m_modelview.setToIdentity();
	m_modelview.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));

	// redraw with new theta
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::changeSubdiv:
//
// Slot function to change number of recursive subdivisions.
//
void
HW3a::changeSubdiv(int subdivisions)
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

	// compute new vertices and texture coordinates
	initVertexBuffer();

	// redraw with new subdivisions
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::changeTwist:
//
// Slot function to turn on/off m_twist;
//
void
HW3a::changeTwist(int twist)
{
	// init vars
	m_twist = twist;

	// redraw with new twist flag
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3a::changeWire:
//
// Slot function to turn on/off m_wire;
//
void
HW3a::changeWire(int wire)
{
	// init vars
	m_wire = wire;

	// redraw with new wire flag
	updateGL();
}
