// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Light.h - Header file for Light class
//
// Written by: George Wolberg 2017
// ======================================================================
#ifndef LIGHT_H
#define LIGHT_H

#include "HW.h"
#include "Sphere.h"
#include "Cylinder.h"

// ----------------------------------------------------------------------
// standard include files
// Directional light 
class Light : public QGLFunctions 
{

public:
	Light();
	void		set	(vec3 eye, vec3 target);		// constructor
	void		display	();
	QMatrix4x4	view	()		{ return m_view;  }
	vec3		&eye	()		{ return m_eye;   }
	vec3		target	()		{ return m_target;}
	void		rotate	(vec2 &rot);
	void		rotate  (float theta);

private:
	vec3		  m_eye;
	vec3		  m_target;
	QMatrix4x4	  m_view;	// 4x4 view  matrix
	Sphere	         *m_sphere;
	vec2		  m_lightRotation;
	GLuint		  m_vertexBuffer;
	GLuint		  m_colorBuffer;
	std::vector<vec3> m_vertices;
	std::vector<vec3> m_colors;
	float		  m_up;
};

#endif // LIGHT_H
