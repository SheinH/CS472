// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Sphere.cpp -  Sphere class implementation
//
// Written by: George Wolberg, Siavash Zokai 2017
// ======================================================================


#include "Sphere.h"
#include <algorithm>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere::Sphere:
//
// Sphere default constructor function.
//
Sphere::Sphere(): m_color   (vec3(1.0f, 1.0f, 1.0f)),
		  m_center  (vec3(0.0f, 0.0f, 0.0f)),
		  m_radius  (1.0f),
		  m_nSeg    (8)

{
	initializeGLFunctions();
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);
	glGenBuffers(1, &m_normalBuffer);
	glGenBuffers(2, m_indicesBuffer);
	glGenBuffers(1, &m_texCoordBuffer);
	castShadow(true);
	setDrawable(true);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere::set:
//
// Set sphere: center, radius, #rings (slices), #segments, and color
//
Sphere::Sphere(vec3 center, float radius, uint nSeg): m_color(vec3(1.0f, 1.0f, 1.0f))
{
	m_center = center;
	m_radius = radius;
	m_nSeg   = nSeg;
	initializeGLFunctions();
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);
	glGenBuffers(1, &m_normalBuffer);
	glGenBuffers(2, m_indicesBuffer);
	glGenBuffers(1, &m_texCoordBuffer);
	castShadow(true);
	setDrawable(true);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initTexture:
//
// Initialize texture.
//
void
Sphere::setTexture(QString& name)
{
	// read image from file
	m_image.load(name);

	// convert jpg to GL formatted image
	QImage qImage = QGLWidget::convertToGLFormat(m_image);

	// init vars
	int w = qImage.width();
	int h = qImage.height();

	// generate texture name and bind it
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());
	glBindTexture(GL_TEXTURE_2D, 0);
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere::initVertices
//
//init vertices, colors, and normals:
//
void
Sphere::initVertices()
{
	// init increments for drawing sphere rings (slices) and segments
	float r = 1.0f / (m_nSeg - 1);
	float s = 1.0f / (m_nSeg - 1);

	m_numPoints = m_nSeg * m_nSeg;
	// visit all segments on each sphere ring (slice)
	for(uint y = 0; y < m_nSeg; ++y) {
		for(uint x = 0; x < m_nSeg; ++x) {
			// init spherical coordinate
			double theta = M_PI * y * r;
			double phi = 2 * M_PI * x * s;
			float const yy = -cos(theta);
			float const xx =  cos(phi) * sin(theta);
			float const zz =  sin(phi) * sin(theta);

			// push point, color, and normal to array
			m_vertices.push_back(vec3(xx*m_radius, yy*m_radius, zz*m_radius));
			m_colors.push_back(m_color);
			m_normals.push_back(vec3(xx, yy, zz));

			float u = (x*s);
			float v = (y*r);

			m_texCoords[y][x].setX(u);
			m_texCoords[y][x].setY(v);
		}
	}
	m_coords.resize(m_numPoints);
	for (int k = 0, i = 0; i < (int)m_nSeg; ++i) {
		for (int j = 0; j < (int)m_nSeg; ++j, k++) {
			m_coords[k] = m_texCoords[i][j];
		}
	}
	m_verticesSize =(GLuint) m_vertices.size();

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere:initIndices:
//
// Indices for drawing sphere triangle strips.
// Triangle strips can be connected by duplicating indices
// between the strips. If connecting strips have opposite
// vertex order then last index of the first strip and first
// index of the second strip needs to be duplicated. If
// connecting strips have same vertex order then only last
// index of the first strip needs to be duplicated.
//
void
Sphere::initIndices()
{
	// push indices to array
	for(uint y = 0; y < m_nSeg-1; ++y) {
		for(uint x = 0; x < m_nSeg-1; ++x) {
			m_indices.push_back((y) * m_nSeg     + x);
			m_indices.push_back((y + 1) * m_nSeg + x);
		}
	}

	// store the number of triangluate indices for later use
	m_indicesSize = (GLuint) m_indices.size();

	// bind index buffer to the GPU and copy indices from CPU to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesSize * sizeof(GLushort), &m_indices[0], GL_STATIC_DRAW);

	// create indices for wireframe rendering (horizontal lines)
	m_indices_wireframe.clear();
	for (int y = 0; y < (int)m_nSeg; ++y) {
		ushort base = y * m_nSeg;
		for (int x = 0; x < (int)m_nSeg - 1; ++x) {
			m_indices_wireframe.push_back(base + x);
			m_indices_wireframe.push_back(base + x + 1);
		}
	}

	// create indices for wireframe rendering (vertical lines)
	for (int x = 0; x < (int)m_nSeg; ++x) {
		for (int y = 0; y < (int)m_nSeg - 1; ++y) {
			ushort base = y * m_nSeg;
			m_indices_wireframe.push_back(base + x);
			m_indices_wireframe.push_back(base + x + m_nSeg);
		}
	}
	// store the number of wireframe indices for later use
	m_indicesWireFrameSize = (GLuint)m_indices_wireframe.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices_wireframe.size() * sizeof(GLushort),
		&m_indices_wireframe[0], GL_STATIC_DRAW);
	// clear CPU indices buffers
	m_indices_wireframe.clear();
	m_indices.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere:uploadVertices:
//
// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
// 
void
Sphere::uploadVertices()
{
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_vertices[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere:uploadColors:
//
// bind color buffer to the GPU and copy the color for each vertex from CPU to GPU
// 
//
void
Sphere::uploadColors()
{
	// bind color buffer to the GPU and copy colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere:uploadNormals:
//
// bind normals buffer to the GPU and copy the normal for each vertex from CPU to GPU
// 
//
void
Sphere::uploadNormals()
{
	// bind normal buffer to the GPU and copy normals from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_normals[0], GL_STATIC_DRAW);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere::uploadTexCoords:
//
// Copy texture coordinates (QVector2D) to GPU.
//
void
Sphere::uploadTexCoords()
{

	if (!m_image.isNull()) {
		// bind texture coord buffer to the GPU and copy the texture coordinates from CPU to GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_numPoints * sizeof(vec2), &m_coords[0], GL_STATIC_DRAW);
	}

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere::createGeometry:
//
// create sphere geometry.
//
void
Sphere::createGeometry()
{
	initVertices();
	initIndices();
	applyTransformation();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere::updateGeometry:
//
// update Sphere geometry whenever apply transformation.
//
void
Sphere::updateGeometry()
{
	uploadVertices();
	uploadColors();
	uploadNormals();
	uploadTexCoords();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere:translate:
//
// Apply translation transformations.
//
void
Sphere::translate(vec3 &t)
{
	m_t_vertices.clear();
	m_t_vertices.resize(m_verticesSize);
	m_center = t;
	for(uint i = 0; i < m_verticesSize; ++i) {
		// translate
		m_t_vertices[i] = m_vertices[i] + t;
	}
	uploadVertices();
	m_t_vertices.clear();

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere:applyTransformation:
//
// Apply translation transformations to sphere.
//
void
Sphere::applyTransformation()
{
	m_t_vertices.clear();
	m_t_normals.clear();

	m_t_vertices.resize(m_verticesSize);
	m_t_normals.resize(m_verticesSize);
	for(uint i = 0; i < m_verticesSize; ++i) {
		m_t_vertices[i] = m_vertices[i] + m_center;
		m_t_normals[i]  = m_normals[i];
	}
	updateGeometry();
	m_t_vertices.clear();
	m_t_normals.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sphere::display:
//
// Display sphere.
//
void
Sphere::display(int mode = 1)
{
	// bind vertex buffer to the GPU; enable buffer to be copied to the
	// attribute vertex variable and specify data format
	glBindBuffer		 (GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer	 (ATTRIB_VERTEX, 3, GL_FLOAT, false, 0, NULL);

	// bind color buffer to the GPU; enable buffer to be copied to the
	// attribute color variable and specify data format
	glBindBuffer		 (GL_ARRAY_BUFFER, m_colorBuffer);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer	 (ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	// bind normal buffer to the GPU; enable buffer to be copied to the
	// attribute normal variable and specify data format
	glBindBuffer		 (GL_ARRAY_BUFFER, m_normalBuffer);
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer	 (ATTRIB_NORMAL, 3, GL_FLOAT, false, 0, NULL);
	if (!m_image.isNull()) {
		// enable buffer to be copied to the attribute texture coord variable and specify data format
		glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
		glEnableVertexAttribArray(ATTRIB_TEXCOORD);
		glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, false, 0, NULL);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
	
	// bind index buffer to the GPU and use it to draw triangle strip
	if (mode == 1 || mode == 2) { // smooth shading or textured
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[0]);
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)m_indicesSize, GL_UNSIGNED_SHORT, 0);
	}
	if (mode == 0) { // Wire frame mode
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer[1]);
		glDrawElements(GL_LINES, m_indicesWireFrameSize, GL_UNSIGNED_SHORT, 0);
	}
}
