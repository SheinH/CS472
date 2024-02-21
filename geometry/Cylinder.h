// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Cylinder.h - Header file for Cylinder class
//
// Written by: George Wolberg 2017
// ======================================================================

#ifndef CYLINDER_H
#define CYLINDER_H

#include "Geometry.h"


class Cylinder : public Geometry {

public:
	Cylinder	();		// default constructor
	Cylinder	(vec3 center, float radius, float height, int nSeg);
	void		setnSegment	(int nSeg)	{ m_nSeg     = nSeg;	}
	void		setHeight	(float h)	{ m_height   = h;	}
	void		setRadius	(float r)	{ m_radius   = r;	}
	void		setColor	(vec3 color)	{ m_color    = color;	}
	void		setCenter	(vec3 center)	{ m_center   = center;	}
	void		setRotation	(vec3 rotation) { m_rotation = rotation;}
	void		display		();
	void		drawTopCap	();
	void		drawBottomCap	();
	void		initVertices	();
	void		initIndices	();
	void		uploadVertices	();
	void		uploadColors	();
	void		uploadNormals	();
	void		applyTransformation();
	void		createGeometry	();
	void		updateGeometry	();


private:

	GLuint			m_vertexBuffer;		// handle to vertex buffer
	GLuint			m_colorBuffer;		// handle to color  buffer
	GLuint			m_normalBuffer;		// handle to vertex buffer
	GLuint			m_indicesBuffer;	// handle to index  buffer

	GLuint			m_indicesSize;		// number of elements in index buffer
	GLuint			m_verticesSize;		// number of vertices

	std::vector<vec3>	m_vertices;		// array of vertices
	std::vector<vec3>	m_normals;		// array of normals
	std::vector<vec3>	m_colors;		// array of colors
	std::vector<GLushort>	m_indices;		// array of indices

	std::vector<vec3>	m_t_vertices;		// array of transformed vertices
	std::vector<vec3>	m_t_normals;		// array of transformed normals

	vec3			m_size;
	vec3			m_color;
	vec3			m_center;
	vec3			m_rotation;
	float			m_radius;
	float			m_height;

	GLushort		m_topCapStart;
	GLushort		m_bottomCapStart;
	int			m_nSeg;
	bool			m_topCap;
	bool			m_bottomCap;
	QMatrix3x3		m_rotationMatrix;
};

#endif // CYLINDER_H
