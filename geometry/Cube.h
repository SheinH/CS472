// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Cube.h - Header file for Cube class
//
// Written by: George Wolberg 2017
// ======================================================================

#ifndef CUBE_H
#define CUBE_H

#include "Geometry.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//
// HW4 : header for Cube object
//

class Cube : public Geometry 
{

public:
	Cube					();
	void		setColor		(vec3 color)    { m_color    = color;    }
	void		setSize			(vec3 size)     { m_size     = size;     }
	void		setCenter		(vec3 center)   { m_center   = center;   }
	void		setRotation		(vec3 rotation) { m_rotation = rotation; }
	void		initVertices		();
	void		uploadVertices		();
	void		uploadColors		();
	void		uploadNormals		();
	void		initIndices		();
	void		display			();
	void		applyTransformation	();
	void		createGeometry		();
	void		updateGeometry		();
	void		translate		(vec3 &t);
	void		rotate			(vec3 &rot);
	void		scale			(vec3 &scale);


private:
	GLuint		  m_vertexBuffer;	// handle to vertex buffer
	GLuint		  m_colorBuffer;	// handle to color  buffer
	GLuint		  m_normalBuffer;	// handle to normal buffer
	GLuint		  m_indicesBuffer;	// handle to index  buffer
	GLuint		  m_verticesSize;	// number of vertices
	GLuint		  m_indicesSize;	// number of elements in index buffer


	std::vector<vec3> m_vertices;		// array of vertices
	std::vector<vec3> m_normals;		// array of normals
	std::vector<vec3> m_colors;		// array of colors

	std::vector<vec3> m_t_vertices;		// array of transformed vertices
	std::vector<vec3> m_t_normals;		// array of transformed normals

	QMatrix3x3	  m_rotationMatrix;
	vec3		  m_size;
	vec3		  m_color;
	vec3		  m_center;
	vec3		  m_rotation;

		

};

#endif // CUBE_H
