// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW0c.cpp - HW0c class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW0c.h"

// shader ID
enum { HW0C };

// uniform ID
enum { PROJ, MODEL, REVERSE };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::HW0c:
//
// HW0c constructor.
//
HW0c::HW0c(const QGLFormat &glf, QWidget *parent)
	: HW(glf, parent)
{
	// init vars
	m_reverseColor = false;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW0c::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init vertex and fragment shaders
	initShaders();

	// create vertex and color buffers
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);

	// initialize vertex and color buffer and write positions to vertex shader
	initBuffers();

	// init model matrix
	m_ModelMatrix.setToIdentity();

	// init state variables
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// set background color
	glColor3f   (1.0f, 1.0f, 1.0f);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW0c::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float ar = (float) w / h;

	// set xmax, ymax;
	float xmax, ymax;
	if(ar > 1.0) {		// wide screen
		xmax = ar;
		ymax = 1.;
	} else {		// tall screen
		xmax = 1.;
		ymax = 1 / ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// compute orthographic projection from viewing coordinates;
	// we use Qt's 4x4 matrix class in place of legacy OpenGL code:
	// glLoadIdentity();
	// glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
	m_projection.setToIdentity();
	m_projection.ortho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::paintGL:
//
// Update GL scene.
//
void
HW0c::paintGL()
{
	// clear canvas with background color
	glClear(GL_COLOR_BUFFER_BIT);

	// bind vertex buffer to the GPU; enable buffer to be accessed
	// via the attribute vertex variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, NULL);

	// bind color buffer to the GPU; enable buffer to be accessed
	// via the attribute color variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	// use glsl progam
	glUseProgram(m_program[HW0C].programId());

	// pass the following parameters to vertex the shader:
	// projection matrix, modelview matrix, and "reverse" flag
	glUniformMatrix4fv(m_uniform[HW0C][PROJ ], 1, GL_FALSE, m_projection.constData ());
	glUniformMatrix4fv(m_uniform[HW0C][MODEL], 1, GL_FALSE, m_ModelMatrix.constData());
	glUniform1i	  (m_uniform[HW0C][REVERSE], m_reverseColor);

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// terminate program; rendering is done
	glUseProgram(0);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW0c::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 0c");

	// create labels
	QLabel *label[2];
	label[0] = new QLabel("Theta");
	label[1] = new QLabel("Reverse color?");

	// create sliders
	m_sliderTheta  = new QSlider(Qt::Horizontal);

	// init sliders
	m_sliderTheta->setRange(0, 360);
	m_sliderTheta->setValue(0);

	// create spinBoxes
	m_spinBoxTheta = new QSpinBox;
	m_spinBoxTheta->setRange(0, 360);
	m_spinBoxTheta->setValue(0);

	// init radio buttons
	m_radio[0] = new QRadioButton("Yes");
	m_radio[1] = new QRadioButton("No");

	// set "No" radio button to be default
	m_radio[1]->setChecked(true);

	// assemble radio buttons into horizontal widget
	QWidget *widget   = new QWidget;
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(m_radio[0]);
	hbox->addWidget(m_radio[1]);
	hbox->addStretch();
	widget->setLayout(hbox);

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0],	  0, 0);
	layout->addWidget(m_sliderTheta,  0, 1);
	layout->addWidget(m_spinBoxTheta, 0, 2);
	layout->addWidget(label[1],	  1, 0);
	layout->addWidget(widget,	  1, 1);

	// assign layout to group box
	groupBox->setLayout(layout);

	// init signal/slot connections
	connect(m_sliderTheta,  SIGNAL(valueChanged(int)), this, SLOT(changeTheta(int)));
	connect(m_spinBoxTheta, SIGNAL(valueChanged(int)), this, SLOT(changeTheta(int)));
	connect(m_radio[0],	SIGNAL(clicked()),	   this, SLOT(reverseOn()));
	connect(m_radio[1],	SIGNAL(clicked()),	   this, SLOT(reverseOff()));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::reset:
//
// Reset parameters.
//
void
HW0c::reset()
{
	// reset parameters
	m_reverseColor = false;

	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(0);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(0);
	m_spinBoxTheta->blockSignals(false);

	// set "No" radio button to be default
	m_radio[1]->setChecked(true);

	// reset 4x4 transformation matrix for model
	m_ModelMatrix.setToIdentity();

	// draw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW0c::initShaders()
{

	// init uniforms hash table based on uniform variable names and location IDs
	UniformMap uniforms;
	uniforms["u_ProjMatrix"  ] = PROJ;
	uniforms["u_ModelMatrix" ] = MODEL;
	uniforms["u_ReverseColor"] = REVERSE;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(HW0C, QString(":/hw0/vshader0c.glsl"), QString(":/hw0/fshader0c.glsl"), uniforms);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
HW0c::initBuffers()
{

	// init geometry data
	const vec2 vertices[] = {
		vec2( 0.0f,   0.75f ),
		vec2( 0.65f, -0.375f),
		vec2(-0.65f, -0.375f)
	};

	// init color data
	const vec3 colors[] = {
		vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f)
	};

	// store vertex positions and colors in m_points and m_colors, respectively
	for(int i=0; i<3; ++i) {
		m_points.push_back(vertices[i]);
		m_colors.push_back(colors  [i]);
	}
	m_numPoints = (int) m_points.size();		// save number of vertices

	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_points[0], GL_STATIC_DRAW);

	// bind color buffer to the GPU and copy the colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);

	// clear vertex and color vectors because they have already been copied into GPU
	m_points.clear();
	m_colors.clear();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::changeTheta:
//
// Slot function to change rotation angle.
//
void
HW0c::changeTheta(int angle)
{
	// update slider and spinbox
	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(angle);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(angle);
	m_spinBoxTheta->blockSignals(false);

	// init vars
	m_reverseColor = false;			// do not update color during rotation

	// update model's rotation matrix
	m_ModelMatrix.setToIdentity();
	m_ModelMatrix.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));

	// draw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::reverseOn:
//
// Slot function to turn on m_reverseColor;
//
void
HW0c::reverseOn()
{
	// init vars
	m_reverseColor = true;

	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::reverseOff:
//
// Slot function to turn off m_reverseColor;
//
void
HW0c::reverseOff()
{
	// init vars
	m_reverseColor = false;

	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::mousePressEvent:
//
// Event handler for mouse press events.
//
void
HW0c::mousePressEvent(QMouseEvent *event)
{
	m_mousePosition = event->pos();
	QGLWidget::mousePressEvent(event);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::mouseReleaseEvent:
//
// Event handler for mouse release events.
//
void
HW0c::mouseReleaseEvent(QMouseEvent *event)
{
	QGLWidget::mouseReleaseEvent(event);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::mouseMoveEvent:
//
// Event handler for mouse move events.
//
void
HW0c::mouseMoveEvent(QMouseEvent *event)
{
	// compute a1: angle of previous mouse position
	int x  = m_mousePosition.x() - m_winW/2;
	int y  = m_winH/2 - m_mousePosition.y();
	int a1 = atan2(y,x) * 180/M_PI;

	// get current mouse position and save it
	m_mousePosition = event->pos();

	if(event->buttons() & Qt::LeftButton) {
		// compute a2: angle of current mouse position
		x = m_mousePosition.x() - m_winW/2;
		y = m_winH/2 - m_mousePosition.y();
		int a2 = atan2(y,x) * 180/M_PI;

		// update angle based on difference between a1 and a2;
		int angle = m_sliderTheta->value() + (a2-a1);

		// convert angle into [0,360] range
		if(angle < 0)	angle += 360;
		if(angle > 360)	angle -= 360;

		// call slot function to update slider/spinbox and render
		changeTheta(angle);
	}

	QGLWidget::mouseMoveEvent(event);
}
