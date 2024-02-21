// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Camera.h - Header file for Camera class
//
// Written by: George Wolberg 2017
// ======================================================================
#ifndef CAMERA_H
#define CAMERA_H

#include "HW.h"


class Camera : public QGLFunctions 
{
public:
	Camera();	// default constructor
	void		set	(vec3 eye, vec3 target);		// constructor
	QMatrix4x4	view	()		{ return m_view;  }
	vec3		&eye	()		{ return m_eye;   }
	vec3		target	()		{ return m_target;}
	void		rotate	(vec2 &rot);
	void		zoom	(float distance);

private:
	vec3		  m_eye;
	vec3		  m_target;
	QMatrix4x4	  m_view;	// 4x4 view  matrix
	vec2		  m_cameraRotation;
	float		  m_up;
};

#endif // CAMERA_H
