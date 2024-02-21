// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW4c.cpp - HW4c class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW4c.h"

#define FAR_DIST	50.0f
#define NEAR_DIST	0.1f
#define DEPTHSIZE	2048*2

enum { MVP, VIEW, DEPTHMVP, DEPTHBIASMVP, SAMPLER, LIGHT_DIR, DEPTH};

enum { DEPTHMAP_SHADER, SHADOW_SHADER, TEXTURE_SHADER};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::HW4c:
//
// HW4c constructor.
//
HW4c::HW4c(const QGLFormat &glf, QWidget *parent)
	: HW(glf, parent)
{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(moveLight()));
	m_biasMatrix(0, 0) = 0.5f;
	m_biasMatrix(0, 1) = 0.0f;
	m_biasMatrix(0, 2) = 0.0f;
	m_biasMatrix(0, 3) = 0.5f;
	m_biasMatrix(1, 0) = 0.0f;
	m_biasMatrix(1, 1) = 0.5f;
	m_biasMatrix(1, 2) = 0.0f;
	m_biasMatrix(1, 3) = 0.5f;
	m_biasMatrix(2, 0) = 0.0f;
	m_biasMatrix(2, 1) = 0.0f;
	m_biasMatrix(2, 2) = 0.5f;
	m_biasMatrix(2, 3) = 0.5f;
	m_biasMatrix(3, 0) = 0.0f;
	m_biasMatrix(3, 1) = 0.0f;
	m_biasMatrix(3, 2) = 0.0f;
	m_biasMatrix(3, 3) = 1.0f;
	reset();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::reset:
//
// Reset parameters.
//
void
HW4c::reset() 
{
	m_showDepthMap	= false;
	m_showLight	= true;
	m_computeDepth	= true;
	m_rotateLigh	= false;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW4c::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 4b");
	groupBox->setStyleSheet(GroupBoxStyle);

	// init checkboxes
	m_checkBox[0] = new QCheckBox("Show depth map");
	m_checkBox[0]->setChecked(false);
	m_checkBox[1] = new QCheckBox("Show light");
	m_checkBox[1]->setChecked(true);

	// assemble radio buttons into horizontal widget
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_checkBox[0]);
	vbox->addWidget(m_checkBox[1]);

	// create animation start/stop button
	m_buttonStart = new QPushButton("Play", this);
	m_buttonStart->setMaximumWidth(70);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(m_buttonStart);

	vbox->addLayout(hbox);
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_checkBox[0], SIGNAL(stateChanged(int)), this, SLOT(showDepthMap(int)));
	connect(m_checkBox[1], SIGNAL(stateChanged(int)), this, SLOT(showLight(int)));
	connect(m_buttonStart, SIGNAL(released()),        this, SLOT(playPauseAnimation()));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW4c::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	makeCurrent();

	// init vertex and fragment shaders
	initShaders();

	initDepthBuffer();

	initScene();

	// create camera
	m_camera = new Camera;

	// set camera position and direction
	m_camera->set(vec3(2.0f, 4.0f, 8.0f), vec3(0, 0, 0));

	// create light
	m_light = new Light;

	// set light position and direction	
	m_light->set(vec3(1.0f, 2.0f, 1.0f), vec3(0, 0, 0));

	// The quad's VBO. Used only for visualizing the shadowmap.
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &m_quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0);	// set background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



bool		
HW4c::initDepthBuffer()
{
	// depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, DEPTHSIZE, DEPTHSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	glGenFramebuffers(1, &m_depthFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthFrameBuffer);

	// no color output in the bound framebuffer, only depth
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// always check that our framebuffer is ok
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( err != GL_FRAMEBUFFER_COMPLETE)
		qDebug() << "Error:: Simple Depth frame buffer:" << err;
	return true;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::initTexture:
//
// Initialize texture.
//
void
HW4c::initScene() 	
{
	m_sphere[0] = new Sphere(vec3(0.0f, 0.5f, 0.0f), 0.5f, 64);
	m_sphere[0]->setColor(vec3(1.0f, 0.0f, 0.0f));
	m_scene.addGeometry(m_sphere[0]);

	// floor
	m_cube[0] = new Cube;
	m_cube[0]->setCenter(vec3(0.0f, 0.0f, 0.0f));
	m_cube[0]->setSize(vec3(2.0f, 0.01f, 2.0f));
	m_cube[0]->setColor(vec3(1.0f, 1.0f, 1.0f));
	m_scene.addGeometry(m_cube[0]);

	// side wall
	m_cube[3] = new Cube;
	m_cube[3]->setCenter(vec3(0.0f, 1.0f, -2.0f));
	m_cube[3]->setSize(vec3(2.0f, 1.0f, 0.01f));
	m_cube[3]->setColor(vec3(0.5f, 1.0f, 0.8f));
	m_cube[3]->castShadow(false);
	m_scene.addGeometry(m_cube[3]);

	// side wall
	m_cube[4] = new Cube;
	m_cube[4]->setCenter(vec3(-2.0f, 1.0f, 0.0f));
	m_cube[4]->setSize(vec3(0.01f, 1.0f, 2.0f));
	m_cube[4]->setColor(vec3(0.5f, 0.8f, 1.0f));
	m_cube[4]->castShadow(false);
	m_scene.addGeometry(m_cube[4]);

	m_cube[1] = new Cube;
	m_cube[1]->setCenter(vec3(-1.0f, 0.1f, -1.0f));
	m_cube[1]->setSize(vec3(0.2f, 0.1f, 0.6f));
	m_cube[1]->setColor(vec3(0.0f, 0.0f, 1.0f));
	m_scene.addGeometry(m_cube[1]);

	m_cube[2] = new Cube;
	m_cube[2]->setCenter(vec3(-1.0f, 0.3f, -1.0f));
	m_cube[2]->setSize(vec3(0.1f, 0.2f, 0.3f));
	m_cube[2]->setColor(vec3(0.0f, 0.0f, 1.0f));
	m_scene.addGeometry(m_cube[2]);

	m_cylinder[0] = new Cylinder(vec3(0.0f, 0.0f, -1.0f), 0.1f, 1.5f, 64);
	m_cylinder[0]->setColor( vec3(0.0f, 1.0f, 0.0f));
	m_cylinder[0]->drawTopCap();
	m_cylinder[0]->drawBottomCap();
	m_scene.addGeometry(m_cylinder[0]);

	m_cylinder[1] = new Cylinder(vec3(0.0f, 0.0f, 1.0f), 0.3f, 0.2f, 64);
	m_cylinder[1]->setColor(vec3(1.0f, 1.0f, 0.2f));
	m_cylinder[1]->drawTopCap();
	m_cylinder[1]->drawBottomCap();
	m_scene.addGeometry(m_cylinder[1]);

	m_cylinder[2] = new Cylinder(vec3(0.0f, 0.2f, 1.0f), 0.2f, 0.2f, 64);
	m_cylinder[2]->setColor(vec3(1.0f, 1.0f, 0.2f));
	m_cylinder[2]->drawTopCap();
	m_cylinder[2]->drawBottomCap();
	m_scene.addGeometry(m_cylinder[2]);

	m_cone[0] = new Cone(vec3(1.5f, 1.01f, 1.0f), 0.2f, 1.0f, 64);
	m_cone[0]->setRotation(vec3(0, 0, 180));
	m_cone[0]->setColor(vec3(1.0f, 0.3f, 0.7f));
	m_cone[0]->drawTopCap();
	m_scene.addGeometry(m_cone[0]);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW4c::initShaders()
{
	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_depthMVP"] = DEPTHMVP;
	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(DEPTHMAP_SHADER,
		   QString(":/hw4/vshader4c1.glsl"),
		   QString(":/hw4/fshader4c1.glsl"),
		   uniforms);

	uniforms.clear();

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_MVP"              ] = MVP;
	uniforms["u_View"             ] = VIEW;
	uniforms["u_LightInvDirection"] = LIGHT_DIR;
	uniforms["u_DepthBiasMVP"     ] = DEPTHBIASMVP;
	uniforms["u_shadowMap"        ] = SAMPLER;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(SHADOW_SHADER,
		   QString(":/hw4/vshader4c2.glsl"),
		   QString(":/hw4/fshader4c2.glsl"),
		   uniforms);

	uniforms.clear();

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_shadowMap"] = SAMPLER;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(TEXTURE_SHADER,
		   QString(":/hw4/vshader4c3.glsl"),
		   QString(":/hw4/fshader4c3.glsl"),
		   uniforms);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW4c::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);
}



void
HW4c::GetShadowMapExtends(vec3 &extends) {

	QMatrix4x4 inverseProjectionViewMatrix = (m_camera->view()).inverted();

	vec3    frustumVertices[8];
	frustumVertices[0] = inverseProjectionViewMatrix * vec3(-1.0f,  1.0f, -1.0f);  // near top-left
	frustumVertices[1] = inverseProjectionViewMatrix * vec3( 1.0f,  1.0f, -1.0f);  // near top-right
	frustumVertices[2] = inverseProjectionViewMatrix * vec3(-1.0f, -1.0f, -1.0f);  // near bottom-left
	frustumVertices[3] = inverseProjectionViewMatrix * vec3( 1.0f, -1.0f, -1.0f);  // near bottom-right
	frustumVertices[4] = inverseProjectionViewMatrix * vec3(-1.0f,  1.0f,  1.0f);  // far top-left
	frustumVertices[5] = inverseProjectionViewMatrix * vec3( 1.0f,  1.0f,  1.0f);  // far top-right
	frustumVertices[6] = inverseProjectionViewMatrix * vec3(-1.0f, -1.0f,  1.0f);  // far bottom-left
	frustumVertices[7] = inverseProjectionViewMatrix * vec3( 1.0f, -1.0f,  1.0f);  // far bottom-right


	vec3 direction = m_light->eye();							// z axis
	vec3 perpVec1 = (vec3::crossProduct(direction, vec3(0.0f, 1.0f, 0.0f))).normalized();	// y axis
	vec3 perpVec2 = (vec3::crossProduct(direction, perpVec1)).normalized();			// x axis

	QMatrix4x4 rotationMatrix;
	rotationMatrix.setToIdentity();
	rotationMatrix(0, 0) = perpVec2.x(); rotationMatrix(0, 1) = perpVec1.x(); rotationMatrix(0, 2) = direction.x();
	rotationMatrix(1, 0) = perpVec2.y(); rotationMatrix(1, 1) = perpVec1.y(); rotationMatrix(1, 2) = direction.y();
	rotationMatrix(2, 0) = perpVec2.z(); rotationMatrix(2, 1) = perpVec1.z(); rotationMatrix(2, 2) = direction.z();


	for(int i = 0; i < 8; ++i)
		frustumVertices[i] = rotationMatrix * frustumVertices[i];


	vec3 minV = frustumVertices[0], maxV = frustumVertices[0];

	for(int i = 1; i < 8; ++i) {
		minV.setX(qMin(minV.x(), frustumVertices[i].x()));
		minV.setY(qMin(minV.y(), frustumVertices[i].y()));
		minV.setZ(qMin(minV.z(), frustumVertices[i].z()));
		maxV.setX(qMax(maxV.x(), frustumVertices[i].x()));
		maxV.setY(qMax(maxV.y(), frustumVertices[i].y()));
		maxV.setZ(qMax(maxV.z(), frustumVertices[i].z()));
	}

	extends = maxV - minV;
	extends.setZ(extends.z()*2.0f);
}



void
HW4c::drawScene(bool depthMap) 
{
	if(depthMap) 
		m_scene.renderShadow();
	else	m_scene.render();

	// draw light
	if(m_showLight && !depthMap) 
		m_light->display();

	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_NORMAL);
}



void
HW4c::computeDepthMap() 	
{
	// cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// enable backface culling
	glCullFace(GL_BACK);

	// init projection matrix
	vec3 extends;
	GetShadowMapExtends(extends);
	m_projection.setToIdentity();
	m_projection.ortho(-extends.x(),
			    extends.x(),
			   -extends.y(),
			    extends.y(),
			   -extends.z(),
			    extends.z());

	// init projection*view matrix; copy it to uniform var and draw
	m_depthMVP = m_projection * m_light->view();

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthFrameBuffer);	// bind depth map buffer
	glViewport(0, 0, DEPTHSIZE, DEPTHSIZE);			// render to whole framebuffer
	glClear(GL_DEPTH_BUFFER_BIT);				// clear depth buffer
	glUseProgram(m_program[DEPTHMAP_SHADER].programId());	// use depth map shader
	glUniformMatrix4fv(m_uniform[DEPTHMAP_SHADER][DEPTHMVP], 1, GL_FALSE, m_depthMVP.constData());

	drawScene(true);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_CULL_FACE);
	m_computeDepth = false;
}


void
HW4c::renderScene() 	
{
	glViewport(0, 0, m_winW, m_winH);			// render to whole window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen and depth map

	// calculate aspect ratio
	float aspect = (float) m_winW / (float) m_winH;

	// init projection matrix
	m_projection.setToIdentity();
	m_projection.perspective(45.0f, aspect, NEAR_DIST, FAR_DIST);

	// init projection * modelview matrix
	QMatrix4x4 mvp = m_projection * m_camera->view();
	QMatrix4x4 depthBiasMVP = m_biasMatrix * m_depthMVP;

	glUseProgram(m_program[SHADOW_SHADER].programId());	// use shadow shader
								// copy data to GPU via uniform variables
	glUniformMatrix4fv(m_uniform[SHADOW_SHADER][MVP ], 1, GL_FALSE, mvp.constData());
	glUniformMatrix4fv(m_uniform[SHADOW_SHADER][VIEW], 1, GL_FALSE, m_camera->view().constData());
	glUniformMatrix4fv(m_uniform[SHADOW_SHADER][DEPTHBIASMVP], 1, GL_FALSE, depthBiasMVP.constData());
	glUniform3fv(m_uniform[SHADOW_SHADER][LIGHT_DIR], 1, &m_light->eye()[0]);
	//glUniform3f(m_uniform[SHADOW_SHADER][LIGHT_DIR], m_light->eye().x(), m_light->eye().y(), m_light->eye().z());

	// bind texture and draw scene
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);		// bind depth texture
	glUniform1i(m_uniform[SHADOW_SHADER][DEPTH], 0);	// set sampler to use texture unit 0
	drawScene(false);					// draw scene
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::paintGL:
//
// Update GL scene.
//
void
HW4c::paintGL()
{
	if(m_computeDepth) computeDepthMap();
	renderScene();
	if(m_showDepthMap) displayDepthMap();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::mousePressEvent:
//
// Mouse press event handler.
//
void 
HW4c::mousePressEvent(QMouseEvent *e)
{
	// Saving mouse press position
	m_mousePressPosition = QVector2D(e->pos());
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::mouseMoveEvent:
//
// Mouse move event handler.
//
void 
HW4c::mouseMoveEvent(QMouseEvent *e)
{
	vec2 currentMouse = vec2(e->pos());
	vec2 diff = (currentMouse - m_mousePressPosition) * 0.001f;

	// Mouse release position - mouse press position
	m_mousePressPosition = currentMouse;
	if(!(e->modifiers() & Qt::CTRL)) {
		m_camera->rotate(diff);
	} else {
		m_light->rotate(diff);
		m_computeDepth = true;
	}

	updateGL();	// update scene
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::mouseWheelEvent:
//
// Mouse wheel event handler.
// allow to zoom in/out in the scene
//
void
HW4c::wheelEvent(QWheelEvent *event) 
{
	m_camera->zoom(event->delta());
	updateGL();	// update scene
}


void
HW4c::displayDepthMap() 
{
	glViewport(0, 0, 256, 256);				// render to 128x128 picture
	glUseProgram(m_program[TEXTURE_SHADER].programId());	// use texture shader

	glBindTexture(GL_TEXTURE_2D, m_depthTexture);		// bind depth texture
	glUniform1i(m_uniform[TEXTURE_SHADER][DEPTH], 0);	// set sampler to use texture unit 0

	// disable GL_COMPARE_R_TO_TEXTURE above in order to see anything!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	// bind vertex buffer to the GPU; enable buffer to be copied to the
	// attribute vertex variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_quad_vertexbuffer);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// draw a quad with two triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);			// 2*3 indices starting at 0

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}



void
HW4c::showDepthMap(int value) 
{
	if(value) m_showDepthMap = true;
	else      m_showDepthMap = false;
	updateGL();
}


void
HW4c::showLight(int value) 
{
	if(value) m_showLight = true;
	else      m_showLight = false;
	updateGL();
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::startAnimation:
//
// Play/Pause animation timer.
//
void
HW4c::playPauseAnimation() 
{
	if(m_rotateLigh) {
		m_rotateLigh = false;
		m_timer->stop();
		m_buttonStart->setText("Play");
	} else {
		m_rotateLigh = true;
		m_buttonStart->setText("Pause");
		m_timer->start(50);
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW4c::timerEvent:
//
// Virtual function called when timer times out.
// We use it to stop timer, recompute surface heights, and restart timer.
//
void
HW4c::moveLight() 
{
	// pause animation to reset grid without interruption by timer
	m_timer->stop();

	// rotate light on a circular path
	m_light->rotate(0.03f);
	m_computeDepth = true;
	updateGL();

	// restart animation
	m_timer->start(50);
}
