// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Cube.Cpp - Cube class implementation
//
// Written by: George Wolberg 2017
// ======================================================================


#include "Cube.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube::Cube:
//
// Cube default constructor function.
//
Cube::Cube():	m_verticesSize(24),
		m_indicesSize (34),
		m_size        (vec3(1.0f, 1.0f, 1.0f)),
		m_color       (vec3(1.0f, 1.0f, 1.0f)),
		m_center      (vec3(0.0f, 0.0f, 0.0f)),
		m_rotation    (vec3(0.0f, 0.0f, 0.0f))


{
	initializeGLFunctions();
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);
	glGenBuffers(1, &m_normalBuffer);
	glGenBuffers(1, &m_indicesBuffer);
	castShadow(true);
	setDrawable(true);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube::initVertices:
//
// intialize all veritices of a unit cube.
//
void
Cube::initVertices()
{
	// init vertices of the six cube faces
	m_vertices.resize(m_verticesSize);
	uint i = 0;
	// Vertex data for face 0 (Front)
	m_vertices[i++] = vec3(-1.0f, -1.0f, 1.0f);// v0
	m_vertices[i++] = vec3( 1.0f, -1.0f, 1.0f);// v1
	m_vertices[i++] = vec3(-1.0f,  1.0f, 1.0f);// v2
	m_vertices[i++] = vec3( 1.0f,  1.0f, 1.0f);// v3

	// Vertex data for face 1 (Right)
	m_vertices[i++] = vec3(1.0f, -1.0f,  1.0f);// v4
	m_vertices[i++] = vec3(1.0f, -1.0f, -1.0f);// v5
	m_vertices[i++] = vec3(1.0f,  1.0f,  1.0f);// v6
	m_vertices[i++] = vec3(1.0f,  1.0f, -1.0f);// v7

	// Vertex data for face 2 (Back)
	m_vertices[i++] = vec3( 1.0f, -1.0f, -1.0f); // v8
	m_vertices[i++] = vec3(-1.0f, -1.0f, -1.0f); // v9
	m_vertices[i++] = vec3( 1.0f,  1.0f, -1.0f); // v10
	m_vertices[i++] = vec3(-1.0f,  1.0f, -1.0f); // v11

	// Vertex data for face 3 (Left)
	m_vertices[i++] = vec3(-1.0f, -1.0f, -1.0f);// v12
	m_vertices[i++] = vec3(-1.0f, -1.0f,  1.0f);// v13
	m_vertices[i++] = vec3(-1.0f,  1.0f, -1.0f);// v14
	m_vertices[i++] = vec3(-1.0f,  1.0f,  1.0f);// v15

	// Vertex data for face 4 (Top)
	m_vertices[i++] = vec3(-1.0f, -1.0f, -1.0f);// v16
	m_vertices[i++] = vec3( 1.0f, -1.0f, -1.0f);// v17
	m_vertices[i++] = vec3(-1.0f, -1.0f,  1.0f);// v18
	m_vertices[i++] = vec3( 1.0f, -1.0f,  1.0f);// v19

	// Vertex data for face 5 (Bottom)
	m_vertices[i++] = vec3(-1.0f, 1.0f,  1.0f);// v20
	m_vertices[i++] = vec3( 1.0f, 1.0f,  1.0f);// v21
	m_vertices[i++] = vec3(-1.0f, 1.0f, -1.0f);// v22
	m_vertices[i++] = vec3( 1.0f, 1.0f, -1.0f);// v23


	i = 0;
	m_normals.resize(m_verticesSize);
	// init vertex normals of the six cube faces
	m_normals[i++] = vec3(0.0f, 0.0f, 1.0f);
	m_normals[i++] = vec3(0.0f, 0.0f, 1.0f);
	m_normals[i++] = vec3(0.0f, 0.0f, 1.0f);
	m_normals[i++] = vec3(0.0f, 0.0f, 1.0f);

	m_normals[i++] = vec3(1.0f, 0.0f, 0.0f);
	m_normals[i++] = vec3(1.0f, 0.0f, 0.0f);
	m_normals[i++] = vec3(1.0f, 0.0f, 0.0f);
	m_normals[i++] = vec3(1.0f, 0.0f, 0.0f);

	m_normals[i++] = vec3(0.0f, 0.0f, -1.0f);
	m_normals[i++] = vec3(0.0f, 0.0f, -1.0f);
	m_normals[i++] = vec3(0.0f, 0.0f, -1.0f);
	m_normals[i++] = vec3(0.0f, 0.0f, -1.0f);


	m_normals[i++] = vec3(-1.0f, 0.0f, 0.0f);
	m_normals[i++] = vec3(-1.0f, 0.0f, 0.0f);
	m_normals[i++] = vec3(-1.0f, 0.0f, 0.0f);
	m_normals[i++] = vec3(-1.0f, 0.0f, 0.0f);

	m_normals[i++] = vec3(0.0f, -1.0f, 0.0f);
	m_normals[i++] = vec3(0.0f, -1.0f, 0.0f);
	m_normals[i++] = vec3(0.0f, -1.0f, 0.0f);
	m_normals[i++] = vec3(0.0f, -1.0f, 0.0f);

	m_normals[i++] = vec3(0.0f, 1.0f, 0.0f);
	m_normals[i++] = vec3(0.0f, 1.0f, 0.0f);
	m_normals[i++] = vec3(0.0f, 1.0f, 0.0f);
	m_normals[i++] = vec3(0.0f, 1.0f, 0.0f);

	m_colors.resize(m_verticesSize);
	for(i = 0; i < m_verticesSize; ++i) 
		m_colors[i] = m_color;

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube:initIndices:
//
// Indices for drawing cube faces using triangle strips.
// Triangle strips can be connected by duplicating indices
// between the strips. If connecting strips have opposite
// vertex order then last index of the first strip and first
// index of the second strip needs to be duplicated. If
// connecting strips have same vertex order then only last
// index of the first strip needs to be duplicated.
//
void
Cube::initIndices() {


	GLushort indices[] ={
		0, 1, 2, 3, 3,		// Face 0 - triangle strip ( v0,  v1,  v2,  v3)
		4, 4, 5, 6, 7, 7,	// Face 1 - triangle strip ( v4,  v5,  v6,  v7)
		8, 8, 9, 10, 11, 11,	// Face 2 - triangle strip ( v8,  v9, v10, v11)
		12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
		16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
		20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
	};

	// bind index buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesSize * sizeof(GLushort), indices, GL_STATIC_DRAW);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube:applyTransformation:
//
// Apply scale, rotation, and translation transformations.
//
void
Cube::applyTransformation()
{
	m_t_vertices.clear();
	m_t_normals.clear();

	m_rotationMatrix = setRotationMatrix(m_rotation);
	m_t_vertices.resize(m_verticesSize);
	m_t_normals.resize (m_verticesSize);
	for(uint i = 0; i < m_verticesSize; ++i) {
		vec3 v = m_vertices[i];
		v.setX(v.x()*m_size.x());		// scale
		v.setY(v.y()*m_size.y());
		v.setZ(v.z()*m_size.z());
		vectorRotate(m_rotationMatrix, v);	// rotate
		v = v + m_center;			// translate
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



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube:scale:
//
// Apply scale.
//
void
Cube::scale(vec3 &s)
{
	m_t_vertices.resize(m_verticesSize);
	m_size = s;
	for(uint i = 0; i < m_verticesSize; ++i) {
		vec3 v = m_vertices[i];
		v -= m_center;
		v.setX(v.x()*m_size.x());		// scale
		v.setY(v.y()*m_size.y());
		v.setZ(v.z()*m_size.z());
		v += m_center;
		m_t_vertices[i] = v;
	}
	uploadVertices();
	m_t_vertices.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube:rotate:
//
// Apply 3D rotation.
// input: (alpha, beta, gamma)
//
void
Cube::rotate(vec3 &r)
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
	}
	uploadVertices();
	uploadNormals();
	m_t_vertices.clear();
	m_t_normals.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube:translate:
//
// Apply 3D translation.
// input: (tx, ty, tz)
//
void
Cube::translate(vec3 &t)
{
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
// Cube:uploadVertices:
//
// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
// 
void
Cube::uploadVertices()
{
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_vertices[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube:uploadColors:
//
// bind color buffer to the GPU and copy the color for each vertex from CPU to GPU
// 
//
void
Cube::uploadColors()
{
	// bind color buffer to the GPU and copy colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube:uploadNormals:
//
// bind normals buffer to the GPU and copy the normal for each vertex from CPU to GPU
// 
//
void
Cube::uploadNormals()
{
	// bind normal buffer to the GPU and copy normals from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesSize * sizeof(vec3), &m_t_normals[0], GL_STATIC_DRAW);
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube::createGeometry:
//
// create cube geometry.
//
void
Cube::createGeometry()
{
	initVertices();
	initIndices();
	applyTransformation();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube::updateGeometry:
//
// update cube geometry whenever apply transformation.
//
void
Cube::updateGeometry()
{
	uploadVertices();
	uploadColors();
	uploadNormals();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cube::display:
//
// Display cube.
//
void
Cube::display()
{
	// bind vertex buffer to the GPU; enable buffer to be copied to the
	// attribute vertex variable and specify data format
	glBindBuffer		 (GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer	 (ATTRIB_VERTEX, 3, GL_FLOAT, false, 0, NULL);

	// bind color buffer to the GPU; enable buffer to be copied to the
	// attribute vertex variable and specify data format
	glBindBuffer		 (GL_ARRAY_BUFFER, m_colorBuffer);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer	 (ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	// bind normal buffer to the GPU; enable buffer to be copied to the
	// attribute vertex variable and specify data format
	glBindBuffer		 (GL_ARRAY_BUFFER, m_normalBuffer);
	glEnableVertexAttribArray(ATTRIB_NORMAL);
	glVertexAttribPointer	 (ATTRIB_NORMAL, 3, GL_FLOAT, false, 0, NULL);

	// bind index buffer to the GPU and use it to draw triangle strip
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) m_indicesSize, GL_UNSIGNED_SHORT, 0);
}
