// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW4a.cpp - HW4a class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW4a.h"
#include <stack>

#define FAR_DIST	50.0f
#define NEAR_DIST	0.1f
#define Radian2Deg	180.0 / 3.141592653589793238463

// shader ID
enum { WIRE_SHADER, SMOOTH_SHADER, SMOOTH_TEX };

// uniform ID
enum { MODEL, VIEW, PROJ, LIGHTDIR, SAMPLER };


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::HW4a:
//
// HW4a constructor.
//
HW4a::HW4a(const QGLFormat &glf, QWidget *parent)
	: HW(glf, parent)
{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));
	reset();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::reset:
//
// Reset parameters.
//
void
HW4a::reset() 
{
	m_showLight	= false;
	m_animate	= false;
	m_angle		= (float) PI2/4;
	m_displayMode   = SMOOTH_TEX;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW4a::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 4a");
	groupBox->setStyleSheet(GroupBoxStyle);

	// init checkboxes
	m_checkBox[0] = new QCheckBox("Show light");
	m_checkBox[0]->setChecked(false);

	// assemble radio buttons into horizontal widget
	QVBoxLayout *vbox = new QVBoxLayout;
	
	QLabel *label;
	label = new QLabel("Display:");
	// create Display comboBox
	m_comboBoxDisplay = new QComboBox;
	m_comboBoxDisplay->addItem("Wireframe");
	m_comboBoxDisplay->addItem("Smooth Shading");
	m_comboBoxDisplay->addItem("Smooth Shading + Texture");
	m_comboBoxDisplay->setCurrentIndex(m_displayMode);

	// create animation start/stop button
	m_buttonStart = new QPushButton("Play", this);
	m_buttonStart->setMaximumWidth(70);

	QHBoxLayout *hbox1 = new QHBoxLayout;
	hbox1->addWidget(m_buttonStart);

	QHBoxLayout *hbox2 = new QHBoxLayout;
	hbox2->addWidget(label);
	hbox2->addWidget(m_comboBoxDisplay);

	vbox->addLayout(hbox2);
	vbox->addLayout(hbox1);
	vbox->addWidget(m_checkBox[0]);
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_checkBox[0], SIGNAL(stateChanged(int)), this, SLOT(showLight(int)));
	connect(m_buttonStart, SIGNAL(released()),        this, SLOT(playPauseAnimation()));
	connect(m_comboBoxDisplay, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDisplay(int)));
	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW4a::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	makeCurrent();

	// init vertex and fragment shaders
	initShaders();

	// init 3D scene
	initScene();

	// create camera
	m_camera = new Camera;

	// set camera position and direction
	m_camera->set(vec3(0.0f, 0.0f, 10.0f), vec3(0, 0, 0));

	// create light
	m_light = new Light;

	// set light position and direction	
	m_light->set(vec3(0.0f, 2.0f, 0.0f), vec3(0, 0, 0));

	// enable depth test
	glEnable(GL_DEPTH_TEST);
	
	// accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	
	// cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// enable back face culling
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);	// set background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::initScene:
//
// Initialize scene.
//
void
HW4a::initScene() 	
{
	m_sun = new Sphere(vec3(0.0f, 0.0f, 0.0f), 1.0f, 32);
	m_sun->setColor(vec3(1.0f, 1.0f, 0.3f));
	QString imageN = ":/sun.jpg";
	m_sun->setTexture(imageN);
	m_sun->createGeometry();

	m_earth = new Sphere(vec3(0.0f, 0.0f, 0.0f), 0.3f, 32);
	m_earth->setColor(vec3(0.0f, 0.6f, 1.0f));
	imageN = ":/earth.jpg";
	m_earth->setTexture(imageN);
	m_earth->createGeometry();

	m_moon = new Sphere(vec3(0.0f, 0.0f, 0.0f), 0.1f, 32);
	m_moon->setColor(vec3(0.6f, 0.6f, 0.6f));
	imageN = ":/moon.jpg";
	m_moon->setTexture(imageN);
	m_moon->createGeometry();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW4a::initShaders()
{
	UniformMap uniforms;

	// reset uniform hash table for next shader
	uniforms.clear();
	uniforms["u_View"] = VIEW;
	uniforms["u_Projection"] = PROJ;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(WIRE_SHADER, QString(":/hw3/vshader3b2.glsl"), QString(":/hw3/fshader3b2.glsl"), uniforms);

	// reset uniform hash table for next shader
	uniforms.clear();
	uniforms["u_View"] = VIEW;
	uniforms["u_Projection"] = PROJ;
	uniforms["u_LightDirection"] = LIGHTDIR;


	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(SMOOTH_SHADER, QString(":/hw3/vshader3b4.glsl"), QString(":/hw3/fshader3b4.glsl"), uniforms);

	// reset uniform hash table for next shader
	uniforms.clear();
	uniforms["u_View"] = VIEW;
	uniforms["u_Projection"] = PROJ;
	uniforms["u_LightDirection"] = LIGHTDIR;
	uniforms["u_Sampler"] = SAMPLER;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(SMOOTH_TEX, QString(":/hw3/vshader3b5.glsl"), QString(":/hw3/fshader3b5.glsl"), uniforms);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW4a::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// calculate aspect ratio
	float aspect = (float)m_winW / (float)m_winH;

	// init projection matrix
	m_projection.setToIdentity();
	m_projection.perspective(45.0f, aspect, NEAR_DIST, FAR_DIST);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::paintGL:
//
// Update GL scene.
//
void
HW4a::paintGL()
{
	glViewport(0, 0, m_winW, m_winH);			// render to whole window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen and depth map
	std::stack<QMatrix4x4> mvStack;

	glUseProgram(m_program[SMOOTH_SHADER].programId());
	glUniformMatrix4fv(m_uniform[SMOOTH_SHADER][PROJ], 1, GL_FALSE, m_projection.constData());
	glUniform3fv(m_uniform[SMOOTH_SHADER][LIGHTDIR], 1, &m_light->eye()[0]);


	mvStack.push(m_camera->view());

	glUseProgram(m_program[m_displayMode].programId());
	glUniform3fv(m_uniform[m_displayMode][LIGHTDIR], 1, &m_light->eye()[0]);
	glUniformMatrix4fv(m_uniform[m_displayMode][PROJ], 1, GL_FALSE, m_projection.constData());
	
	// init sun translation to identity matrix
	QMatrix4x4 translate_sun;
	translate_sun.setToIdentity();

	// apply translate_sun to top of stack
	// PUT YOUR CODE HERE

	// duplicate top of stack
	// PUT YOUR CODE HERE

	// init sun rotation: m_angle about the y-axis
	QMatrix4x4 rotate_sun;
	rotate_sun.rotate(m_angle*Radian2Deg / 2.0f, vec3(0, 1, 0));

	// apply sun rotation to top of stack
	// PUT YOUR CODE HERE

	// display sun according to display mode
	glUniformMatrix4fv(m_uniform[m_displayMode][VIEW], 1, GL_FALSE, mvStack.top().constData());
	glUniform1i(m_uniform[m_displayMode][SAMPLER], 0);
	m_sun->display(m_displayMode);
	mvStack.pop();

	// earth: push top of stack (sun model-view matrix) to build up earth transform
	// PUT YOUR CODE

	// apply earth translation of [5*sin(m_angle), 0, 5*cos(m_angle)]
	QMatrix4x4 translate_earth;
	translate_earth.setToIdentity();
	translate_earth.translate(5.0f*sin(m_angle), 0.0f, 5.0f * cos(m_angle));
	mvStack.top() *= translate_earth;

	// save top of stack and apply 5*m_angle earth rotation about y-axis
	mvStack.push(mvStack.top());  // duplicating
	QMatrix4x4 rotate_earth;
	rotate_earth.rotate(m_angle*Radian2Deg / 0.2f, vec3(0, 1, 0));
	mvStack.top() *= rotate_earth;

	// display earth according to display mode 
	glUniformMatrix4fv(m_uniform[m_displayMode][VIEW], 1, GL_FALSE, mvStack.top().constData());
	m_earth->display(m_displayMode);
	glBindTexture(GL_TEXTURE_2D, 0);
	mvStack.pop();

	// moon: save top of stack and apply moon transform
	mvStack.push(mvStack.top());  // mvMat of earth
	QMatrix4x4 translate_moon;
	float angle1 = m_angle + PII / 4;
	translate_moon.translate(sin(angle1*3.0f), 0,  cos(angle1*3.0f));
	mvStack.push(mvStack.top());  // duplicating
	mvStack.top() *= translate_moon;
	QMatrix4x4 rotate_moon;
	rotate_moon.rotate(m_angle*Radian2Deg / 0.3f, vec3(0, 1, 0));
	mvStack.top() *= rotate_moon;

	// display moon according to display mode
	glUniformMatrix4fv(m_uniform[m_displayMode][VIEW], 1, GL_FALSE, mvStack.top().constData());
	m_moon->display(m_displayMode);
	mvStack.pop();

	mvStack.pop(); mvStack.pop();
	// draw light
	glUseProgram(m_program[SMOOTH_SHADER].programId());
	glUniformMatrix4fv(m_uniform[SMOOTH_SHADER][PROJ], 1, GL_FALSE, m_projection.constData());
	glUniform3fv(m_uniform[SMOOTH_SHADER][LIGHTDIR], 1, &m_light->eye()[0]);
	glUniformMatrix4fv(m_uniform[SMOOTH_SHADER][VIEW], 1, GL_FALSE, m_camera->view().constData());
	if (m_showLight)
		m_light->display();

	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_NORMAL);
	glUseProgram(0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::mousePressEvent:
//
// Mouse press event handler.
//
void 
HW4a::mousePressEvent(QMouseEvent *e)
{
	// Saving mouse press position
	m_mousePressPosition = QVector2D(e->pos());
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::mouseMoveEvent:
//
// Mouse move event handler.
//
void 
HW4a::mouseMoveEvent(QMouseEvent *e)
{
	vec2 currentMouse = vec2(e->pos());
	vec2 diff = (currentMouse - m_mousePressPosition) * 0.001f;

	// Mouse release position - mouse press position
	m_mousePressPosition = currentMouse;
	if(!(e->modifiers() & Qt::CTRL)) {
		m_camera->rotate(diff);
	} else {
		m_light->rotate(diff);

	}

	updateGL();	// update scene
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::mouseWheelEvent:
//
// Mouse wheel event handler.
// allow to zoom in/out in the scene
//
void
HW4a::wheelEvent(QWheelEvent *event) 
{
	m_camera->zoom(event->delta());
	updateGL();	// update scene
}



void
HW4a::showLight(int value) 
{
	if(value) m_showLight = true;
	else      m_showLight = false;
	updateGL();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::startAnimation:
//
// Play/Pause animation timer.
//
void
HW4a::playPauseAnimation() 
{
	if(m_animate) {
		m_animate = false;
		m_timer->stop();
		m_buttonStart->setText("Play");
	} else {
		m_animate = true;
		m_buttonStart->setText("Pause");
		m_timer->start(50);
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::timerEvent:
//
// Virtual function called when timer times out.
// We use it to stop timer, recompute surface heights, and restart timer.
//
void
HW4a::animate() 
{
	// pause animation to reset grid without interruption by timer
	m_timer->stop();

	// rotate light on a circular path
	m_angle += 0.02f;

	updateGL();

	// restart animation
	m_timer->start(20);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4a::changeDisplay:
//
// Change display mode.
//
void
HW4a::changeDisplay(int val)
{

	m_displayMode = val;
	updateGL();
}
