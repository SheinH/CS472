// ======================================================================
// Nail Point Art
// Copyright (C) 2015 by George Wolberg
//
// Cone.h - Header file for Cone class
//
// Written by: George Wolberg, Siavash Zokai 2015
// ======================================================================

#ifndef CONE_H
#define CONE_H


// ----------------------------------------------------------------------
// standard include files
//
#include "Geometry.h"


class Cone: public Geometry {

public:
	Cone();
	Cone				(vec3 center, float radius, float height, int nSeg);
	void		setnSegment	(int nSeg)	{ m_nSeg     = nSeg;	}
	void		setHeight	(float h)	{ m_height   = h;	}
	void		setRadius	(float r)	{ m_radius   = r;	}
	void		setColor	(vec3 color)	{ m_color    = color;	}
	void		setCenter	(vec3 center)	{ m_center   = center;	}
	void		setRotation	(vec3 rotation) { m_rotation = rotation;}
	void		display		();
	void		drawTopCap	();
	void		initVertices	();
	void		initIndices	();
	void		uploadVertices	();
	void		uploadColors	();
	void		uploadNormals	();
	void		applyTransformation();
	void		createGeometry	();
	void		updateGeometry	();
	void		translate	(vec3 &t);
	void		rotate		(vec3 &rot);


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
	int			m_nSeg;
	bool			m_topCap;
	bool			m_bottomCap;

	QMatrix3x3		m_rotationMatrix;
};

#endif // CONE_H
