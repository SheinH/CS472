// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW3b.cpp - HW3b class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW3b.h"

// shader ID
enum {TEX_SHADER, WIRE_SHADER, FLAT_SHADER, SMOOTH_SHADER, SMOOTH_TEX};

// uniform ID
enum { MODEL, VIEW, PROJ, LIGHTDIR, SAMPLER };

// wave program enums
enum { WIREFRAME, TEXTURED, TEXTURED_WIREFRAME, FLAT_COLOR, SMOOTH_COLOR, SMOOTH_TEXTURE };
enum {WEAK, NORMAL, STRONG};
enum {FLAT, SPIKE, DIAGONALWALL, SIDEWALL, HOLE, MIDDLEBLOCK, DIAGONALBLOCK, CORNERBLOCK, HILL, HILLFOUR};
#define SQRTOFTWOINV 1.0 / 1.414213562


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::HW3b:
//
// HW3b constructor.
//
HW3b::HW3b(const QGLFormat &glf, QWidget *parent) : HW(glf, parent)
{
	m_timer = new QTimer;
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));

	// reset parameters
	m_grid		= 32;
	m_displayMode	= TEXTURED_WIREFRAME;
	m_geometryMode  = MIDDLEBLOCK;
	m_speed		= 1;
	m_angularSpeed	= -30.0f;
	m_wave		= false;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW3b::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init texture
	initTexture();

	// init vertex and fragment shaders
	initShaders();

	// init XY vertices in mesh and texture coords
	initVertices();

	// init Z values in height field
	resetMesh();

	// set dt based on m_grid size and user selection for m_speed
	setDT();

	// initialize vertex buffer and write positions to vertex shader
	initBuffers();

	// create camera
	m_camera = new Camera;

	// set camera position and direction
	m_camera->set(vec3(1.0f, -4.0f, 3.0f), vec3(0, 0, 0));

	// create light
	m_light = new Light;

	// set light position and direction	
	m_light->set (vec3(1.0f, -2.0f, 2.0f), vec3(0, 0, 0));

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// enable/specify hints for antialiased lines
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// set background and foreground colors
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW3b::resizeGL(int w, int h)
{
	// PUT YOUR CODE (use perspective projection)
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::paintGL:
//
// Update GL scene.
//
void
HW3b::paintGL()
{
	// clear canvas with background color and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(1.5f);

	// bind vertex buffer to the GPU; enable buffer to be accessed
	// via the attribute vertex variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, false, 0, NULL);

	// bind texture buffer to the GPU; enable buffer to be accessed
	// via the attribute texture coordinate and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_texBuffer);
	glEnableVertexAttribArray(ATTRIB_TEXCOORD);
	glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, false, 0, NULL);

	// bind color buffer to the GPU; enable buffer to be accessed
	// via the attribute color variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	// bind normal buffer to the GPU; enable buffer to be accessed
	// via the attribute normal variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, false, 0, NULL);

	// draw textured triangles
	switch(m_displayMode) {
	case TEXTURED_WIREFRAME:
	case TEXTURED:
		// draw textured surface
		// PUT YOUR CODE HERE
		if(m_displayMode != TEXTURED_WIREFRAME)
			break;
	case WIREFRAME:
		// draw wireframe
		// PUT YOUR CODE HERE
		break;
	case FLAT_COLOR:
		glUseProgram(m_program[FLAT_SHADER].programId());	
		glUniformMatrix4fv(m_uniform[FLAT_SHADER][VIEW ], 1, GL_FALSE, m_camera->view().constData());
		glUniformMatrix4fv(m_uniform[FLAT_SHADER][PROJ ], 1, GL_FALSE, m_projection.constData());
		glUniform3fv(m_uniform[FLAT_SHADER][LIGHTDIR], 1, &m_light->eye()[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[0]);
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) m_indices_triangles.size(), GL_UNSIGNED_SHORT, 0);
		break;
	case SMOOTH_COLOR:
		glUseProgram(m_program[SMOOTH_SHADER].programId());	
		glUniformMatrix4fv(m_uniform[SMOOTH_SHADER][VIEW ], 1, GL_FALSE, m_camera->view().constData());
		glUniformMatrix4fv(m_uniform[SMOOTH_SHADER][PROJ ], 1, GL_FALSE, m_projection.constData());
		glUniform3fv(m_uniform[FLAT_SHADER][LIGHTDIR], 1, &m_light->eye()[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[0]);
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) m_indices_triangles.size(), GL_UNSIGNED_SHORT, 0);
		break;
	case SMOOTH_TEXTURE:
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glUseProgram(m_program[SMOOTH_TEX].programId());	
		glUniformMatrix4fv(m_uniform[SMOOTH_TEX][VIEW ], 1, GL_FALSE, m_camera->view().constData());
		glUniformMatrix4fv(m_uniform[SMOOTH_TEX][PROJ ], 1, GL_FALSE, m_projection.constData());
		glUniform3fv(m_uniform[SMOOTH_TEX][LIGHTDIR], 1, &m_light->eye()[0]);
		glUniform1i(m_uniform[SMOOTH_TEX][SAMPLER], 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[0]);
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) m_indices_triangles.size(), GL_UNSIGNED_SHORT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// display light position and direction
	glUseProgram(m_program[WIRE_SHADER].programId());	// use wireframe glsl progam
	glUniformMatrix4fv(m_uniform[WIRE_SHADER][VIEW ], 1, GL_FALSE, m_camera->view().constData());
	glUniformMatrix4fv(m_uniform[WIRE_SHADER][PROJ ], 1, GL_FALSE, m_projection.constData());
	m_light->display();
	glUseProgram(0);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW3b::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 3b");
	groupBox->setStyleSheet(GroupBoxStyle);
	// create labels
	QLabel *label[4];
	label[0] = new QLabel("Display:");
	label[1] = new QLabel("Geometry:");
	label[2] = new QLabel("Size:");
	label[3] = new QLabel("Speed:");

	// create Display comboBox
	m_comboBoxDisplay = new QComboBox;
	m_comboBoxDisplay->addItem("Wireframe");
	m_comboBoxDisplay->addItem("Textured");
	m_comboBoxDisplay->addItem("Textured+Wireframe");
	m_comboBoxDisplay->addItem("Flat Shading");
	m_comboBoxDisplay->addItem("Smooth Shading");
	m_comboBoxDisplay->addItem("Smooth Shading + Texture");
	m_comboBoxDisplay->setCurrentIndex(m_displayMode);

	// create Geometry comboBox
	m_comboBoxGeom = new QComboBox;
	m_comboBoxGeom->addItem("Flat");
	m_comboBoxGeom->addItem("Spike");
	m_comboBoxGeom->addItem("Diagonal Wall");
	m_comboBoxGeom->addItem("Side Wall");
	m_comboBoxGeom->addItem("Hole");
	m_comboBoxGeom->addItem("Middle Block");
	m_comboBoxGeom->addItem("Diagonal Block");
	m_comboBoxGeom->addItem("Corner Block");
	m_comboBoxGeom->addItem("Hill");
	m_comboBoxGeom->addItem("Hill Four");
	m_comboBoxGeom->setCurrentIndex(m_geometryMode);

	// create Size slider
	m_sliderSize = new QSlider(Qt::Horizontal);
	m_sliderSize->setRange(2, MAXGRID);
	m_sliderSize->setValue(m_grid);

	// create Size spinbox
	m_spinBoxSize = new QSpinBox;
	m_spinBoxSize->setRange(2, MAXGRID);
	m_spinBoxSize->setValue(m_grid);

	// create Speed slider
	m_sliderSpeed = new QSlider(Qt::Horizontal);
	m_sliderSpeed->setRange(1, 20);
	m_sliderSpeed->setValue(m_speed);

	// create Speed spinbox
	m_spinBoxSpeed = new QSpinBox;
	m_spinBoxSpeed->setRange(1, 20);
	m_spinBoxSpeed->setValue(m_speed);

	// create animation start/stop button
	m_buttonStart = new QPushButton("Play", this);
	m_buttonStart->setMaximumWidth(70);

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(m_buttonStart);

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0],		0, 0);
	layout->addWidget(m_comboBoxDisplay,	0, 1);
	layout->addWidget(label[1],		1, 0);
	layout->addWidget(m_comboBoxGeom,	1, 1);
	layout->addWidget(label[2],		2, 0);
	layout->addWidget(m_sliderSize,		2, 1);
	layout->addWidget(m_spinBoxSize,	2, 2);
	layout->addWidget(label[3],		3, 0);
	layout->addWidget(m_sliderSpeed,	3, 1);
	layout->addWidget(m_spinBoxSpeed,	3, 2);

	// vertical layout for assembling widgets
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(layout);
	vbox->addLayout(hbox);

	// assign layout to group box
	groupBox->setLayout(vbox);

	// init signal/slot connections
	connect(m_comboBoxDisplay,SIGNAL(currentIndexChanged(int)), this, SLOT(changeDisplay(int)));
	connect(m_comboBoxGeom,   SIGNAL(currentIndexChanged(int)), this, SLOT(changeGeom   (int)));
	connect(m_sliderSize,	  SIGNAL(valueChanged(int)),	    this, SLOT(changeSize(int)));
	connect(m_spinBoxSize,    SIGNAL(valueChanged(int)),        this, SLOT(changeSize(int)));
	connect(m_sliderSpeed,    SIGNAL(valueChanged(int)),        this, SLOT(changeSpeed(int)));
	connect(m_buttonStart,    SIGNAL(released()),               this, SLOT(playPauseAnimation()));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::reset:
//
// Reset parameters.
//
void
HW3b::reset()
{
	// reset parameters
	m_grid		= 32;
	m_displayMode	= TEXTURED_WIREFRAME;
	m_geometryMode	= MIDDLEBLOCK;
	m_speed		= 1;
	m_angularSpeed	= 30.0f;
	m_wave		= false;

	// reset variables
	m_comboBoxDisplay->setCurrentIndex(m_displayMode);
	m_comboBoxGeom->setCurrentIndex(m_geometryMode);
	m_sliderSize->setValue(m_grid);
	m_sliderSpeed->setValue(m_speed);

	// init XY vertices in mesh and texture coords
	initVertices();

	// init Z values in height field
	resetMesh();

	// set dt based on m_grid size and user selection for m_speed
	setDT();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::resetMesh:
//
// Reset height field based on geometry mode flag.
//
void
HW3b::resetMesh()
{
	// pause animation to reset grid without interruption by timer
	if(m_wave) m_timer->stop();

	for(int i=0; i< m_grid; ++i) {
	   for(int j=0; j< m_grid; ++j) {
		m_force[i][j] = 0.0f;
		m_veloc[i][j] = 0.0f;
		QVector3D &vec = m_vertices[i][j];
		switch(m_geometryMode) {
			case FLAT:
				vec.setZ(0.0f);
				break;
			case SPIKE:
				vec.setZ((i==j && i==m_grid/2) ? 1.0f : 0.0f);
				break;
			case HOLE:
				// PUT YOUR CODE HERE
				break;
			case DIAGONALWALL:
				// PUT YOUR CODE HERE
				break;
			case SIDEWALL:
				// PUT YOUR CODE HERE
				break;
			case DIAGONALBLOCK:
				// PUT YOUR CODE HERE
				break;
			case MIDDLEBLOCK:
				// PUT YOUR CODE HERE
				break;
			case CORNERBLOCK:
				// PUT YOUR CODE HERE
				break;
			case HILL:
				// PUT YOUR CODE HERE
				break;
			case HILLFOUR:
				// PUT YOUR CODE HERE
				break;
		}
	   }
	}
	getFaceNorms();
	getVertNorms();

	// copy 2D vertex array to 1D array that will be passed to GPU
	flatten2D();

	// restart animation
	if(m_wave) m_timer->start(10);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::getForce:
//
// Compute force values based on differences with eight neighbors
// around each pixel.
//
void HW3b::getForce(void)
{
	float d;

	for(int i=0; i<m_grid; ++i)
	    for(int j = 0; j<m_grid; ++j)
		m_force[i][j] = 0.0f;

	for(int i=1; i<m_grid-1; ++i) {
	    for(int j = 1; j<m_grid-1; ++j) {
		d = m_vertices[i][j].z() - m_vertices[i][j-1].z();
		m_force[i][j]   -= d;
		m_force[i][j-1] += d;

		d = m_vertices[i][j].z() - m_vertices[i-1][j].z();
		m_force[i][j]   -= d;
		m_force[i-1][j] += d;

		d = m_vertices[i][j].z() - m_vertices[i][j+1].z();
		m_force[i][j]   -= d;
		m_force[i][j+1] += d;

		d = m_vertices[i][j].z() - m_vertices[i+1][j].z();
		m_force[i][j]   -= d;
		m_force[i+1][j] += d;

		d = (m_vertices[i][j].z() - m_vertices[i+1][j+1].z())*SQRTOFTWOINV;
		m_force[i][j]     -= d;
		m_force[i+1][j+1] += d;

		d = (m_vertices[i][j].z() - m_vertices[i-1][j-1].z())*SQRTOFTWOINV;
		m_force[i][j]     -= d;
		m_force[i-1][j-1] += d;

		d = (m_vertices[i][j].z() - m_vertices[i+1][j-1].z())*SQRTOFTWOINV;
		m_force[i][j]     -= d;
		m_force[i+1][j-1] += d;

		d = (m_vertices[i][j].z() - m_vertices[i-1][j+1].z())*SQRTOFTWOINV;
		m_force[i][j]     -= d;
		m_force[i-1][j+1] += d;
	    }
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::getVelocity:
//
// Compute velocity values based on forces at each pixel.
//
void HW3b::getVelocity()
{
	for(int i=0; i<m_grid; ++i)
	   for(int j=0; j<m_grid; ++j)
		m_veloc[i][j] += m_force[i][j] * m_dt;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::getPosition:
//
// Compute vertex positions values based on velocity at each pixel.
//
void HW3b::getPosition()
{
	for(int i=0; i<m_grid; ++i) {
	   for(int j=0; j<m_grid; ++j)
		m_vertices[i][j].setZ(m_vertices[i][j].z() + m_veloc[i][j]);
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::timerOut:
//
// Virtual function called when timer times out.
// We use it to stop timer, recompute surface heights, and restart timer.
//
void 
HW3b::timeOut()
{
	// pause animation to reset grid without interruption by timer
	m_timer->stop();

	// recompute surface height
	getForce();
	getVelocity();
	getPosition();
	getFaceNorms();
	getVertNorms();
	flatten2D();

	// copy to GPU and redraw
	uploadVertices();
	uploadNormals();
	updateGL();

	// restart animation
	m_timer->start(10);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initTexture:
//
// Initialize texture.
//
void
HW3b::initTexture()
{
	// read image from file
	m_image.load(QString(":/mandrill.jpg"));

	// convert jpg to GL formatted image
	QImage qImage = QGLWidget::convertToGLFormat(m_image);

	// init vars
	int w = qImage.width ();
	int h = qImage.height();

	// generate texture name and bind it
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());
	glBindTexture(GL_TEXTURE_2D, 0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW3b::initShaders()
{

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_View"      ] = VIEW;
	uniforms["u_Projection"] = PROJ;
	uniforms["u_Sampler"   ] = SAMPLER;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(TEX_SHADER,  QString(":/hw3/vshader3b1.glsl"), 
				QString(":/hw3/fshader3b1.glsl"), uniforms);

	// reset uniform hash table for next shader
	uniforms.clear();
	uniforms["u_View"      ] = VIEW;
	uniforms["u_Projection"] = PROJ;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(WIRE_SHADER, QString(":/hw3/vshader3b2.glsl"), 
				QString(":/hw3/fshader3b2.glsl"), uniforms);

	// reset uniform hash table for next shader
	uniforms.clear();
	uniforms["u_View"          ] = VIEW;
	uniforms["u_Projection"    ] = PROJ;
	uniforms["u_LightDirection"] = LIGHTDIR;
	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(FLAT_SHADER, QString(":/hw3/vshader3b3.glsl"), 
				QString(":/hw3/fshader3b3.glsl"), uniforms);

	uniforms.clear();
	uniforms["u_View"          ] = VIEW;
	uniforms["u_Projection"    ] = PROJ;
	uniforms["u_LightDirection"] = LIGHTDIR;
	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(SMOOTH_SHADER, QString(":/hw3/vshader3b4.glsl"), 
				  QString(":/hw3/fshader3b4.glsl"), uniforms);

	uniforms.clear();
	uniforms["u_View"          ] = VIEW;
	uniforms["u_Projection"    ] = PROJ;
	uniforms["u_LightDirection"] = LIGHTDIR;
	uniforms["u_Sampler"       ] = SAMPLER;
	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(SMOOTH_TEX,  QString(":/hw3/vshader3b5.glsl"), 
				QString(":/hw3/fshader3b5.glsl"), uniforms);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initVertices:
//
// Initialize (x,y) positions of vertices so that they lie in [-1,1] range.
// Initialize texture coordinates.
// Initialize vertex indices for texture mapped triangles and wireframe.
// Surface height values (z) are evaluated in resetMesh().
//
void
HW3b::initVertices()
{
	// map the grid coordinates to [-1.0, 1.0]; function called when m_grid changes
	// flip y-coordinates because array orientation is upside down wrt graphics coordinates
	int size = m_grid - 1;
	for(int y=0; y < m_grid; ++y) {
		for(int x=0; x < m_grid; ++x) {
			vec3 &vec = m_vertices[y][x];
			vec.setX(-1 + (2. * x / size));
			vec.setY(-1 + (2. * (size-y) / size));
			m_colors.push_back(vec3(0.2f, 0.9f, 1.0f));

			// init texture coordinates
			vec2 &texCoord = m_texCoords[y][x];
			texCoord.setX((float) x / size);
			texCoord.setY((float) (size-y) / size);
		}
	}

	// create indices with degenerate triangles (stitching)
	m_indices_triangles.clear();
	for(int y=0; y < m_grid-1; ++y) {
		ushort base = y * m_grid;
		if(y > 0) {
			// Degenerate begin: repeat first vertex
			m_indices_triangles.push_back(base);
		}
		for(int x=0; x < m_grid; ++x){
			m_indices_triangles.push_back(base + x);
			m_indices_triangles.push_back(base + m_grid + x);
		}
		if(y < m_grid-2)
			m_indices_triangles.push_back((y+1) * m_grid + (m_grid - 1));
	}

	// create indices for wireframe rendering (horizontal lines)
	m_indices_wireframe.clear();
	for(int y=0; y < m_grid; ++y) {
		ushort base = y * m_grid;
		for(int x = 0; x < m_grid-1; ++x){
			m_indices_wireframe.push_back(base + x);
			m_indices_wireframe.push_back(base + x + 1);
		}
	}

	// create indices for wireframe rendering (vertical lines)
	for(int x=0; x < m_grid; ++x) {
		for(int y=0; y < m_grid-1; ++y){
			ushort base = y * m_grid;
			m_indices_wireframe.push_back(base + x);
			m_indices_wireframe.push_back(base + x + m_grid);
		}
	}

	m_numPoints = m_grid * m_grid;		// save number of vertices
	m_points.resize(m_numPoints);
	m_coords.resize(m_numPoints);
	m_normals.resize(m_numPoints);


}



// face normals - for flat shading
void 
HW3b::getFaceNorms()
{

	for(int i = 0; i < m_grid-1; ++i) {
		for(int j = 0; j < m_grid-1; ++j) {

			// get vectors from geometry points
			vec3 &p0 = m_vertices[i  ][j  ];
			vec3 &p1 = m_vertices[i  ][j+1];
			vec3 &p2 = m_vertices[i+1][j  ];
			vec3 &p3 = m_vertices[i+1][j+1];

			vec3 v0 = p1 - p0;
			vec3 v1 = p2 - p0;
			vec3 v2 = p3 - p0;

			// get triangle face normals from vectors & normalize them
			vec3 norm0 = vec3::crossProduct(v2, v0);
			norm0.normalize();

			// get triangle face normals from vectors & normalize them
			vec3 norm1 = vec3::crossProduct(v1, v2);
			norm1.normalize();

			m_faceNorms[0][i][j] = norm0;
			m_faceNorms[1][i][j] = norm1;
		}
	}
}



// vertex normals - average of face normals for smooth shading
void 
HW3b::getVertNorms()
{
	for(int i = 0; i < m_grid; ++i) {
		for(int j = 0; j < m_grid; ++j) {
			// for each vertex, average normals from all faces sharing vertex;
			// check each quadrant in turn
			vec3 avg;
			// right & above
			if(j < m_grid-1 && i < m_grid-1)
				avg += m_faceNorms[0][i][j];

			// right & below
			if(j < m_grid-1 && i > 0) {
				avg += m_faceNorms[0][i-1][j];
				avg += m_faceNorms[1][i-1][j];
			}

			// left & above
			if(j > 0 && i < m_grid-1 ) {
				avg += m_faceNorms[0][i][j-1];
				avg += m_faceNorms[1][i][j-1];
			}

			// left & below
			if(j > 0 && i > 0)
				avg += m_faceNorms[1][i-1][j-1];

			// normalize
			m_vertNorms[i][j] = avg.normalized();
		}
	}
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initBuffers:
//
// Initialize vertex buffer.
//
void
HW3b::initBuffers()
{
	glGenBuffers(1, &m_vertexBuffer);
	uploadVertices();

	glGenBuffers(1, &m_texBuffer);
	uploadTexCoords();

	glGenBuffers(2, m_indicesBuffer);
	uploadIndices();

	glGenBuffers(1, &m_normalBuffer);
	uploadNormals();

	glGenBuffers(1, &m_colorBuffer);
	uploadColors();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::uploadVertices:
//
// Copy vertices (QVector3D) to GPU.
//
void
HW3b::uploadVertices()
{
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_points[0], GL_DYNAMIC_DRAW);

	// enable buffer to be copied to the attribute vertex variable and specify data format
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer	 (ATTRIB_VERTEX, 3, GL_FLOAT, false, 0, NULL);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::uploadTexCoords:
//
// Copy texture coordinates (QVector2D) to GPU.
//
void
HW3b::uploadTexCoords()
{
	// bind texture coord buffer to the GPU and copy the texture coordinates from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_texBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_coords[0], GL_STATIC_DRAW);

	// enable buffer to be copied to the attribute texture coord variable and specify data format
	glEnableVertexAttribArray(ATTRIB_TEXCOORD);
	glVertexAttribPointer	 (ATTRIB_TEXCOORD, 2, GL_FLOAT, false, 0, NULL);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::uploadNormals:
//
// Copy normals (QVector3D) to GPU.
//
void
HW3b::uploadNormals()
{
	// bind normal buffer to the GPU and copy the normals from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_normals[0], GL_DYNAMIC_DRAW);

	// enable buffer to be accessed via the attribute normal variable and specify data format
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, false, 0, NULL);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::uploadColors:
//
// Copy colors (QVector3D) to GPU.
//
void
HW3b::uploadColors()
{
	// bind color buffer to the GPU and copy the colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);

	// enable buffer to be accessed via the attribute color variable and specify data format
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::uploadIndices:
//
// Copy vertex indices to GPU for texture mapped triangles and wireframe.
//
void
HW3b::uploadIndices()
{
	// bind indices buffer to the GPU and copy the indices from CPU to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices_triangles.size() * sizeof(GLushort),
			&m_indices_triangles[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices_wireframe.size() * sizeof(GLushort),
			&m_indices_wireframe[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::flatten:
//
// Copy 2D vertex array to 1D array that will be passed to GPU
//
void
HW3b::flatten2D()
{
	for(int k=0, i=0; i < m_grid; ++i) {
		for(int j=0;  j < m_grid; ++j, k++) {
			m_points[k]  = m_vertices [i][j];
			m_coords[k]  = m_texCoords[i][j];
			m_normals[k] = m_vertNorms[i][j];
		}
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::changeDisplay:
//
// Change display mode.
//
void
HW3b::changeDisplay(int val)
{

	m_displayMode = val;
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::changeGeom:
//
// Slot function to change geometry.
//
void
HW3b::changeGeom(int val)
{
	// pause animation if waving
	if(m_wave) m_timer->stop();

	// recompute height field
	m_geometryMode = val;
	resetMesh();

	// copy to GPU
	uploadVertices();
	uploadNormals();
	uploadTexCoords();
	uploadIndices();

	// draw
	updateGL();

	// restart animation if it was waving
	if(m_wave) m_timer->start(10);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::changeSize:
//
// Slot function to change grid size.
//
void
HW3b::changeSize(int val)
{
	// pause animation if waving
	if(m_wave) m_timer->stop();

	// update slider and spinbox
	m_sliderSize->blockSignals(true);
	m_sliderSize->setValue(val);
	m_sliderSize->blockSignals(false);

	m_spinBoxSize->blockSignals(true);
	m_spinBoxSize->setValue(val);
	m_spinBoxSize->blockSignals(false);

	// update variable
	m_grid = val;

	// recompute new vertices on 2D grid and triangle indices
	initVertices();

	// recompute height field
	resetMesh();
	changeSpeed(m_speed);

	// copy to GPU
	uploadVertices();
	uploadColors();
	uploadNormals();
	uploadTexCoords();
	uploadIndices();

	// draw
	updateGL();

	// restart animation if it was waving
	if(m_wave) m_timer->start(10);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::setDT:
//
// Change dt based on animation speed and mesh size;
//
void
HW3b::setDT()
{
	float scale = (float) m_grid / MAXGRID;
	scale *= scale;
	m_dt = (m_speed/100.) * scale;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::changeSpeed:
//
// Slot function to change animation speed.
//
void
HW3b::changeSpeed(int val)
{
	m_speed = val;
	setDT();

	// update slider and spinbox
	m_sliderSpeed->blockSignals(true);
	m_sliderSpeed->setValue(val);
	m_sliderSpeed->blockSignals(false);

	m_spinBoxSpeed->blockSignals(true);
	m_spinBoxSpeed->setValue(val);
	m_spinBoxSpeed->blockSignals(false);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::playPauseAnimation:
//
// Play/pause animation timer.
//
void 
HW3b::playPauseAnimation()
{
	if(m_wave) {
		m_wave = false;
		m_timer->stop();
		m_buttonStart->setText("Play");
	} else {
		m_wave = true;
		m_buttonStart->setText("Pause");
		m_timer->start(50);

	}
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::mousePressEvent:
//
// Mouse press event handler.
//
void 
HW3b::mousePressEvent(QMouseEvent *e)
{
	// save mouse press position
	m_mousePressPosition = QVector2D(e->pos());
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::mouseMoveEvent:
//
// Mouse move event handler.
//
void HW3b::mouseMoveEvent(QMouseEvent *e)
{

	vec2 currentMouse = vec2(e->pos());
	vec2 diff = (currentMouse - m_mousePressPosition) * 0.001f;
	// Mouse release position - mouse press position
	m_mousePressPosition = currentMouse;
	if (!(e->modifiers() & Qt::CTRL)) {
		m_camera->rotate(diff);
	} else {
		m_light->rotate(diff);
	}

	// Update scene
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::mouseWheelEvent:
//
// Mouse wheel event handler.
// allow to zoom in/out in the scene
//
void
HW3b::wheelEvent(QWheelEvent *event)
{
	m_camera->zoom(event->delta());
	// update scene
	updateGL();
}
