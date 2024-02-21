// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Cylinder.cpp - file for Cylinder class implementation 
//
// Written by: George Wolberg, 2017
// ======================================================================


#include "Cylinder.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::Cylinder:
//
// Cylinder default constructor function.
//
Cylinder::Cylinder():	m_color (vec3(1.0f, 1.0f, 1.0f)),
			m_center(vec3(0.0f, 0.0f, 0.0f)),
			m_rotation(vec3(0.0f, 0.0f, 0.0f)),
			m_radius(1.0f),
			m_height(1.0f),
			m_nSeg(8)


{
	initializeGLFunctions();
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);
	glGenBuffers(1, &m_normalBuffer);
	glGenBuffers(1, &m_indicesBuffer);
	m_topCap	= false;		// draw cylinder without cap at top
	m_bottomCap	= false;		// draw cylinder without cap at bottom
	setRotation(vec3(0.0f, 0.0f, 0.0f));	// default cylinder rotation
	castShadow(true);
	setDrawable(true);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::Cylinder:
//
// Cylinder constructor function.
//
Cylinder::Cylinder(vec3 center, float radius, float height, int nSeg):
	m_color(vec3(1.0f, 1.0f, 1.0f)),
	m_rotation(vec3(0.0f, 0.0f, 0.0f))
{
	initializeGLFunctions();
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);
	glGenBuffers(1, &m_normalBuffer);
	glGenBuffers(1, &m_indicesBuffer);
	m_topCap	= false;		// draw cylinder without cap at top
	m_bottomCap	= false;		// draw cylinder without cap at bottom
	setRotation(vec3(0.0f, 0.0f, 0.0f));	// default cylinder rotation
	m_center = center;
	m_radius = radius;
	m_height = height;
	m_nSeg   = nSeg;
	castShadow(true);
	setDrawable(true);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::drawTopCap:
//
// Set flag to draw cylinder with cap at top.
//
void
Cylinder::drawTopCap()
{
	m_topCap = true;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::drawBottomCap:
//
// Set flag to draw cylinder with cap at bottom.
//
void
Cylinder::drawBottomCap()
{
	m_bottomCap = true;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::initVertices
//
//init vertices, colors, and normals:
//
void
Cylinder::initVertices()
{
	// init number of vertices
	m_verticesSize	 = m_nSeg * 2 + m_nSeg * 2 + 2;
	m_topCapStart    = 2 * (m_nSeg + 1);
	m_bottomCapStart = 2 * m_topCapStart + 2;

	// store the number of indices for later use
	m_indicesSize = (GLuint) 2*(m_nSeg + 1) + 2*(2*m_nSeg + 2) + 2;

	const double delta = 2*M_PI / m_nSeg;

	// top cap
	float theta = 0.0f;
	for(int i = 0; i<m_nSeg; ++i) {
		const float cosTheta = (float) cos(theta);
		const float sinTheta = (float) sin(theta);
		vec3 vertex = vec3(cosTheta * m_radius, m_height, sinTheta * m_radius);
		m_vertices.push_back(vertex);
		m_colors.push_back(m_color);
		vec3 normal = vec3(cosTheta, 0.0f, sinTheta);
		m_normals.push_back(normal);
		theta += delta;
	}

	// Sides
	theta = 0.0f;
	for(int i = 0; i<m_nSeg; ++i) {
		const float cosTheta = (float) cos(theta);
		const float sinTheta = (float) sin(theta);
		vec3 vertex = vec3(cosTheta * m_radius, 0.0f, sinTheta * m_radius);
		m_vertices.push_back(vertex);
		m_colors.push_back(m_color);
		vec3 normal = vec3(cosTheta, 0.0f, sinTheta);
		m_normals.push_back(normal);
		theta += delta;
	}

	vec3 vertex = vec3(0.0f, m_height, 0.0f);
	m_vertices.push_back(vertex);
	m_colors.push_back(m_color);
	vec3 normal = vec3(0.0f, 1.0f, 0.0f);
	m_normals.push_back(normal);

	theta = 0.0f;
	for(int i = 0; i<m_nSeg; ++i) {
		const float cosTheta = (float) cos(theta);
		const float sinTheta = (float) sin(theta);
		vec3 vertex = vec3(cosTheta * m_radius, m_height, sinTheta * m_radius);
		m_vertices.push_back(vertex);
		m_colors.push_back(m_color);
		m_normals.push_back(normal);
		theta += delta;
	}

	// bottom cap
	vertex = vec3(0.0f, 0.0f, 0.0f);
	m_vertices.push_back(vertex);
	m_colors.push_back(m_color);
	normal = vec3(0.0f, -1.0f, 0.0f);
	m_normals.push_back(normal);

	theta = 0.0f;
	for(int i = 0; i<m_nSeg; ++i) {
		const float cosTheta = (float) cos(theta);
		const float sinTheta = (float) sin(theta);
		vec3 vertex = vec3(cosTheta * m_radius, 0.0f, sinTheta * m_radius);
		m_vertices.push_back(vertex);
		m_colors.push_back(m_color);
		m_normals.push_back(normal);
		theta += delta;
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder:initIndices:
//
// Indices for drawing cylider faces using triangle strips.
// Triangle strips can be connected by duplicating indices
// between the strips. If connecting strips have opposite
// vertex order then last index of the first strip and first
// index of the second strip needs to be duplicated. If
// connecting strips have same vertex order then only last
// index of the first strip needs to be duplicated.
//
void
Cylinder::initIndices()
{
	// Sides
	for(int i = 0; i < m_nSeg + 1; ++i) {
		unsigned int i1 = i % m_nSeg;
		unsigned int i2 = i1 + m_nSeg;
		m_indices.push_back(i2);
		m_indices.push_back(i1);
	}
	// top cap
	uint vbase = 2 * m_nSeg;
	for(int i = 0; i < m_nSeg + 1; ++i) {
		uint i1 = i % m_nSeg;
		m_indices.push_back(vbase + 1 + i1);
		m_indices.push_back(vbase);
		if(i == m_nSeg) { // degenerated triangle
			m_indices.push_back(vbase);
			m_indices.push_back(vbase + m_nSeg + 1);
		}
	}

	// bottom cap
	vbase = 2 * m_nSeg + m_nSeg + 1;
	for(int i = 0; i < m_nSeg + 1; ++i) {
		uint i1 = i % m_nSeg;
		m_indices.push_back(vbase);
		m_indices.push_back(vbase + 1 + i1);
	}

	// store the number of indices for later use
	m_indicesSize = (GLuint) m_indices.size();

	// bind index buffer to the GPU and copy indices from CPU to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesSize * sizeof(GLushort), &m_indices[0], GL_STATIC_DRAW);

	// clear CPU indices buffers
	m_indices.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder:uploadVertices:
//
// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
// 
void
Cylinder::uploadVertices()
{
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_vertices[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder:uploadColors:
//
// bind color buffer to the GPU and copy the color for each vertex from CPU to GPU
// 
//
void
Cylinder::uploadColors()
{
	// bind color buffer to the GPU and copy colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder:uploadNormals:
//
// bind normals buffer to the GPU and copy the normal for each vertex from CPU to GPU
// 
//
void
Cylinder::uploadNormals()
{
	// bind normal buffer to the GPU and copy normals from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_normals[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::createGeometry:
//
// create cylinder geometry.
//
void
Cylinder::createGeometry()
{
	initVertices();
	initIndices();
	applyTransformation();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::updateGeometry:
//
// update cylinder geometry whenever apply transformation.
//
void
Cylinder::updateGeometry()
{
	uploadVertices();
	uploadColors();
	uploadNormals();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder:applyTransformation:
//
// Apply scale, rotation, and translation transformations.
//
void
Cylinder::applyTransformation()
{
	m_t_vertices.clear();
	m_t_normals.clear();

	m_rotationMatrix = setRotationMatrix(m_rotation);
	m_t_vertices.resize(m_verticesSize);
	m_t_normals.resize(m_verticesSize);
	for(uint i = 0; i < m_verticesSize; ++i) {
		vec3 v = m_vertices[i];
		vectorRotate(m_rotationMatrix, v);	// rotate
		v = v + m_center;				// translate
		m_t_vertices[i] = v;
		vec3 n = m_normals[i];
		vectorRotate(m_rotationMatrix, n);	// rotate normals
		m_t_normals[i] = n;
	}
	updateGeometry();
	m_t_vertices.clear();
	m_t_normals.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cylinder::display:
//
// Display cylinder.
//
void
Cylinder::display()
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

	// bind index buffer to the GPU and use it to draw triangle strip
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) 2*(m_nSeg + 1), GL_UNSIGNED_SHORT, 0);

	// draw caps, if necessary
	if(m_topCap)
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) 2 * (m_nSeg + 1), GL_UNSIGNED_SHORT,
				(void *) (m_topCapStart*sizeof(GLushort)));
	if(m_bottomCap)
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) 2 * (m_nSeg + 1), GL_UNSIGNED_SHORT,
				(void *) (m_bottomCapStart*sizeof(GLushort)));
}



