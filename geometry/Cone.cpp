// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// Sphere.h - Header file for Sphere class
//
// Written by: George Wolberg, 2015
// ======================================================================


#include "Cone.h"

Cone::Cone():	m_color(vec3(1.0f, 1.0f, 1.0f)),
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
	m_topCap = false;			// draw cylinder without cap at top
	setRotation(vec3(0.0f, 0.0f, 0.0f));	// default cylinder rotation
	castShadow(true);
	setDrawable(true);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cone::Cone:
//
// Cone constructor function.
//
Cone::Cone(vec3 center, float radius, float height, int nSeg):
				m_color   (vec3(1.0f, 1.0f, 1.0f)),
				m_rotation(vec3(0.0f, 0.0f, 0.0f))
{
	initializeGLFunctions();
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);
	glGenBuffers(1, &m_normalBuffer);
	glGenBuffers(1, &m_indicesBuffer);
	m_topCap = false;		// draw cylinder without cap at top
	m_bottomCap = false;		// draw cylinder without cap at bottom
	setRotation(vec3(0.0f, 0.0f, 0.0f));	// default cylinder rotation
	m_center = center;
	m_radius = radius;
	m_height = height;
	m_nSeg   = nSeg;
	castShadow(true);
	setDrawable(true);
}


void
Cone::drawTopCap()
{
	m_topCap = true;
}

void
Cone::initVertices()
{

	m_verticesSize = m_nSeg * 3 + 1;
	m_topCapStart = 2 * (m_nSeg + 1);

	// store the number of indices for later use
	m_indicesSize = (GLuint) 2*(m_nSeg + 1) + 2*(m_nSeg + 1) + 1;

	const double delta = 2*M_PI / m_nSeg;
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

	theta = 0.0f;
	vec3 vertex = vec3(0.0f, 0.0f, 0.0f);
	for(int i = 0; i<m_nSeg; ++i) {
		const float cosTheta = (float) cos(theta);
		const float sinTheta = (float) sin(theta);
		m_vertices.push_back(vertex);
		m_colors.push_back(m_color);
		vec3 normal = vec3(cosTheta, 0.0f, sinTheta);
		m_normals.push_back(normal);
		theta += delta;
	}

	vertex = vec3(0.0f, m_height, 0.0f);
	m_vertices.push_back(vertex);
	m_colors.push_back(m_color);
	vec3 normal = vec3(0.0f, 1.0f, 0.0f);
	m_normals.push_back(normal);

	theta = 0.0f;
	// top cap vertices
	for(int i = 0; i<m_nSeg; ++i) {
		const float cosTheta = (float) cos(theta);
		const float sinTheta = (float) sin(theta);
		vec3 vertex = vec3(cosTheta * m_radius, m_height, sinTheta * m_radius);
		m_vertices.push_back(vertex);
		m_colors.push_back(m_color);
		m_normals.push_back(normal);
		theta += delta;
	}

}


void
Cone::initIndices()
{

	// Sides
	for(int i = 0; i < m_nSeg + 1; ++i) {
		unsigned int i1 = i  % m_nSeg;
		unsigned int i2 = i1 + m_nSeg;
		m_indices.push_back(i2);
		m_indices.push_back(i1);
	}
	// Caps top
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
	// store the number of indices for later use
	m_indicesSize = (GLuint) m_indices.size();

	// bind index buffer to the GPU and copy indices from CPU to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesSize * sizeof(GLushort), &m_indices[0], GL_STATIC_DRAW);

	// clear CPU indices buffers
	m_indices.clear();
}



void
Cone::uploadVertices()
{
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_vertices[0], GL_STATIC_DRAW);
}


void
Cone::uploadColors()
{
	// bind color buffer to the GPU and copy colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);
}

void
Cone::uploadNormals()
{
	// bind normal buffer to the GPU and copy normals from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_normals[0], GL_STATIC_DRAW);
}



void
Cone::createGeometry()
{
	initVertices();
	initIndices();
	applyTransformation();
}


void
Cone::updateGeometry()
{
	uploadVertices();
	uploadColors();
	uploadNormals();
}


void
Cone::applyTransformation()
{
	m_t_vertices.clear();
	m_t_normals.clear();

	m_rotationMatrix = setRotationMatrix(m_rotation);
	m_t_vertices.resize(m_verticesSize);
	m_t_normals.resize(m_verticesSize);
	vec3 center = m_center;
	for(uint i = 0; i < m_verticesSize; ++i) {
		vec3 v = m_vertices[i];
		vectorRotate(m_rotationMatrix, v);	// rotate
		v = v + center;				// translate
		m_t_vertices[i] = v;
		vec3 n = m_normals[i];
		vectorRotate(m_rotationMatrix, n);	// rotate normals
		m_t_normals[i] = n;
	}
	uploadVertices();
	uploadNormals();
	uploadColors();
	m_t_vertices.clear();
	m_t_normals.clear();
}



void
Cone::rotate(vec3 &r)
{
	m_t_vertices.clear();
	m_t_normals.clear();
	m_rotation = r;
	m_rotationMatrix = setRotationMatrix(m_rotation);
	m_t_vertices.resize(m_verticesSize);
	m_t_normals.resize(m_verticesSize);
	for(uint i = 0; i < m_verticesSize; ++i) {
		vec3 v = m_vertices[i];
		v -= m_center;
		vectorRotate(m_rotationMatrix, v);	// rotate
		v += m_center;
		m_t_vertices[i] = v;
		vec3 n = m_normals[i];
		vectorRotate(m_rotationMatrix, n);	// rotate normals
		m_t_normals[i] = n;
	}
	uploadVertices();
	uploadNormals();
	m_t_vertices.clear();
	m_t_normals.clear();
}


void
Cone::translate(vec3 &t)
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




void
Cone::display()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	// enable the assignment of attribute vertex variable
	glEnableVertexAttribArray(ATTRIB_VERTEX);

	// assign the buffer object to the attribute vertex variable
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, false, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);

	// enable the assignment of attribute vertex variable
	glEnableVertexAttribArray(ATTRIB_COLOR);

	// assign the buffer object to the attribute vertex variable
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, false, 0, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) 2*(m_nSeg + 1), GL_UNSIGNED_SHORT, 0);
	if(m_topCap)
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) 2 * (m_nSeg + 1), GL_UNSIGNED_SHORT, (void *) (m_topCapStart*sizeof(GLushort)));

}


