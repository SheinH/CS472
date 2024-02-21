// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW0d.cpp - HW0d class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW0d.h"

// shader ID
enum { HW0D };

// uniform ID
enum { PROJ, MODEL };

QString glsl_rst = "\
QMatrix4x4 m;\n\
m.setToIdentity();\n\
m.rotate(m_angle, vec3(0, 0, 1));\n\
m.scale(0.5f);\n\
m.translate(vec3(1, 0, 0));\n\
\n\
Composite matrix: RST\n\
";

QString glsl_tsr = "\
QMatrix4x4 m;\n\
m.setToIdentity();\n\
m.translate(vec3(1.0f, 0.5f, 0.0f));\n\
m.scale(0.5f);\n\
m.rotate(m_angle, vec3(0, 0, 1));\n\
\n\
Composite matrix = TSR\n\
";

QString opengl_rst = "\
glMatrixMode(GL_MODELVIEW);\n\
glLoadIdentity();\n\
glRotatef(m_angle, 0, 0, 1);\n\
glScalef(0.5f, 0.5f, 0.5f);\n\
glTranslatef(1.0f, 0.5f, 0.0f);\n\
\n\
Composite matrix: IRST\n\
";

QString opengl_tsr = "\
glMatrixMode(GL_MODELVIEW);\n\
glLoadIdentity();\n\
glTranslatef(1.0f, 0.5f, 0.0f);\n\
glScalef(0.5f, 0.5f, 0.5f);\n\
glRotatef(m_angle, 0, 0, 1);\n\
\n\
Composite matrix = ITSR\n\
";


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::HW0d:
//
// HW0d constructor.
//
HW0d::HW0d(const QGLFormat &glf, QWidget *parent)
	: HW(glf, parent)
{
	// init vars
	m_rst	= true;
	m_glsl	= true;
	m_angle	= 0;
	m_timer	= new QTimer;
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW0d::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init vertex and fragment shaders
	initShaders();

	// create vertex and color buffers
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);

	// initialize vertex buffer and write positions to vertex shader
	initBuffers();

	// init state variables
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// set background color
	m_timer->start(10);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW0d::resizeGL(int w, int h)
{
	// compute aspect ratio
	float ar = (float) w / h;

	// set xmax, ymax;
	if(ar > 1.0) {		// wide screen
		m_xmax = ar;
		m_ymax = 1.;
	} else {		// tall screen
		m_xmax = 1.;
		m_ymax = 1 / ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::paintGL:
//
// Update GL scene.
//
void
HW0d::paintGL()
{
	// clear canvas with background color
	glClear(GL_COLOR_BUFFER_BIT);

	if(m_glsl) renderGLSL();
	else	   renderGL  ();

	// draw center point
	glLoadIdentity();
	glPointSize(5);
	glBegin(GL_POINTS);
		glColor3f(1, 1, 1);
		glVertex2f(0, 0);
	glEnd();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::renderGL:
//
// Update GL scene using standard opengl.
//
void
HW0d::renderGL() 
{
	// compute orthographic projection from viewing coordinates;
	// we use legacy OpenGL code:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_xmax, m_xmax, -m_ymax, m_ymax, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(m_rst) {
		glRotatef(m_angle, 0, 0, 1);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(1.0f, 0.5f, 0.0f);
	} else {
		glTranslatef(1.0f, 0.5f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glRotatef(m_angle, 0, 0, 1);
	}

	// draw triangle
	glBegin(GL_TRIANGLES);
		for(int i=0; i<(int)m_points.size(); ++i) {
			glColor3f (m_colors[i].x(), m_colors[i].y(), m_colors[i].z());
			glVertex2f(m_points[i].x(), m_points[i].y());
		}
	glEnd();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::renderGLSL:
//
// Update GL scene using shader (GLSL).
//
void
HW0d::renderGLSL() 
{
	// compute orthographic projection from viewing coordinates;
	// we use Qt's 4x4 matrix class in place of legacy OpenGL code
	m_projection.setToIdentity();
	m_projection.ortho(-m_xmax, m_xmax, -m_ymax, m_ymax, -1.0, 1.0);

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
	glUseProgram(m_program[HW0D].programId());

	m_ModelMatrix.setToIdentity();
	if(m_rst) {
		m_ModelMatrix.rotate(m_angle, vec3(0.0f, 0.0f, 1.0f));
		m_ModelMatrix.scale(0.5f);
		m_ModelMatrix.translate(vec3(1.0f, 0.5f, 0.0f));
	} else {
		m_ModelMatrix.translate(vec3(1.0f, 0.5f, 0.0f));
		m_ModelMatrix.scale(0.5f);
		m_ModelMatrix.rotate(m_angle, vec3(0.0f, 0.0f, 1.0f));
	}

	// pass the following parameters to vertex the shader:
	// projection matrix, modelview matrix, and "reverse" flag
	glUniformMatrix4fv(m_uniform[HW0D][PROJ ], 1, GL_FALSE, m_projection.constData());
	glUniformMatrix4fv(m_uniform[HW0D][MODEL], 1, GL_FALSE, m_ModelMatrix.constData());

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// terminate program; rendering is done
	glUseProgram(0);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW0d::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 0d");

	// create labels
	QLabel *label[2];
	label[0] = new QLabel("Shader mode?");
	label[1] = new QLabel("rot/scale/trans order?");

	// init radio buttons
	m_radio[0] = new QRadioButton("GLSL");
	m_radio[1] = new QRadioButton("OpenGL");
	m_radio[2] = new QRadioButton("Yes");
	m_radio[3] = new QRadioButton("No");

	// set "GLSL" radio button to be default
	m_radio[0]->setChecked(true);
	m_radio[1]->setChecked(false);

	// set "NO" radio button to be default
	m_radio[2]->setChecked(true);
	m_radio[3]->setChecked(false);

	// assemble radio buttons into horizontal widget
	QWidget *widget0   = new QWidget;
	QHBoxLayout *hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_radio[0]);
	hbox0->addWidget(m_radio[1]);
	hbox0->addStretch();
	widget0->setLayout(hbox0);

	QWidget *widget1   = new QWidget;
	QHBoxLayout *hbox1 = new QHBoxLayout;
	hbox1->addWidget(m_radio[2]);
	hbox1->addWidget(m_radio[3]);
	hbox1->addStretch();
	widget1->setLayout(hbox1);

	// create text edit widget
	m_codes = new QTextEdit();
	m_codes->setReadOnly(true);

	QFont* font = new QFont("Courier New");
	font->setBold(true);
	m_codes->setFont(*font);
	setCodes();

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0], 0, 0);
	layout->addWidget(widget0,  0, 1);
	layout->addWidget(label[1], 1, 0);
	layout->addWidget(widget1,  1, 1);
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(layout);
	vbox->addWidget(m_codes);

	// assign layout to group box
	groupBox->setLayout(vbox);

	connect(m_radio[0], SIGNAL(clicked()), this, SLOT(shaderOn()));
	connect(m_radio[1], SIGNAL(clicked()), this, SLOT(shaderOff()));
	connect(m_radio[2], SIGNAL(clicked()), this, SLOT(reverseOn()));
	connect(m_radio[3], SIGNAL(clicked()), this, SLOT(reverseOff()));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::reset:
//
// Reset parameters.
//
void
HW0d::reset()
{
	// reset parameters
	m_rst  = false;
	m_glsl = true;

	// set "GLSL" radio button to be default
	m_radio[0]->setChecked(true);
	m_radio[1]->setChecked(false);

	// set "YES" radio button to be default
	m_radio[2]->setChecked(true);
	m_radio[3]->setChecked(false);
	setCodes();

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
HW0d::initShaders()
{
	// init uniforms hash table based on uniform variable names and location IDs
	UniformMap uniforms;
	uniforms["u_ProjMatrix"  ] = PROJ;
	uniforms["u_ModelMatrix" ] = MODEL;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(HW0D, QString(":/hw0/vshader0d.glsl"), QString(":/hw0/fshader0d.glsl"), uniforms);
	glUseProgram(0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
HW0d::initBuffers()
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::reverseOn:
//
// Slot function to turn on m_reverseOrder;
//
void
HW0d::reverseOn()
{
	// init vars
	m_rst	= true;
	m_angle = 0;
	setCodes();
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::reverseOff:
//
// Slot function to turn off m_reverseOrder;
//
void
HW0d::reverseOff()
{
	// init vars
	m_rst	= false;
	m_angle = 0;
	setCodes();
	updateGL();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::shaderOn:
//
// Slot function to use GLSL mode
//
void
HW0d::shaderOn() 
{
	// init vars
	m_glsl = true;
	m_angle = 0;
	setCodes();
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::shaderOff:
//
// Slot function to turn off GLSL mode
//
void
HW0d::shaderOff() 
{
	// init vars
	m_glsl = false;
	m_angle = 0;
	setCodes();
	updateGL();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::timerOut:
//
// Virtual function called when timer times out.
// We use it to stop timer, modifiy model view matrix, and restart timer.
//
void
HW0d::timeOut() 
{
	// pause animation to reset grid without interruption by timer
	m_timer->stop();

	m_angle += 1.0f;
	updateGL();

	// restart animation
	m_timer->start(10);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0d::setCodes:
//
//
void
HW0d::setCodes() 
{
	// clear m_codes to avoid appending text
	m_codes->clear();

	// set text
	if(m_glsl) {	// use GLSL
		if(m_rst) m_codes->setText(glsl_rst);
		else	  m_codes->setText(glsl_tsr);
	} else {	// use OpenGL library
		if(m_rst) m_codes->setText(opengl_rst);
		else	  m_codes->setText(opengl_tsr);
	}
}
