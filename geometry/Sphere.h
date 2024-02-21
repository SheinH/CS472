// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Sphere.h - Header file for Sphere class
//
// Written by: George Wolberg 2017
// ======================================================================

#ifndef SPHERE_H
#define SPHERE_H


#include "Geometry.h"
#define MAXSEG 64

// ----------------------------------------------------------------------
// standard include files
//

class Sphere : public Geometry 
{

public:
			Sphere		(); // default constructor
			Sphere		(vec3 center, float radius, uint nSeg);
	void		setnSegment	(int nSeg)	{ m_nSeg   = nSeg;	}
	void		setRadius	(float r)	{ m_radius = r;		}
	void		setColor	(vec3 color)	{ m_color  = color;	}
	void		setCenter	(vec3 center)	{ m_center = center;	}
	void		setTexture	(QString &name);
	void		initVertices	();
	void		initIndices	();
	void		uploadVertices	();
	void		uploadColors	();
	void		uploadNormals	();
	void		uploadTexCoords ();
	void		display		(int);
	void		createGeometry	();
	void		updateGeometry	();
	void		applyTransformation();
	void		translate	(vec3 &t);

private:
	GLuint		m_vertexBuffer;		// handle to vertex buffer
	GLuint		m_colorBuffer;		// handle to color  buffer
	GLuint		m_normalBuffer;		// handle to normal buffer
	GLuint		m_indicesBuffer[2];	// handle to index  buffer
	GLuint		m_texCoordBuffer;	// handle to texture buffer
	GLuint		m_indicesSize;		// number of elements in index buffer
	GLuint		m_indicesWireFrameSize;
	GLuint		m_verticesSize;		// number of vertices
	GLuint		m_texture;		// shader index to texture unit

	std::vector<vec3>	m_vertices;	// array of vertices
	std::vector<vec3>	m_colors;	// array of colors
	std::vector<vec3>	m_normals;	// array of normals
	std::vector<GLushort>	m_indices;	// array of indices
	std::vector<GLushort>	m_indices_wireframe;
	std::vector<vec2>	m_coords;
	vec2 m_texCoords[MAXSEG][MAXSEG];	// vector of 2D coords (UV)
	std::vector<vec3>	m_t_vertices;	// array of transformed vertices
	std::vector<vec3>	m_t_normals;	// array of transformed normals

	vec3		  m_color;
	vec3		  m_center;
	float		  m_radius;
	uint		  m_nSeg;
	QImage		  m_image;
	int		  m_numPoints;

};

#endif // SPHERE_H
