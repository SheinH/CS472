// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Camera.cpp -  file for Camera class
//
// Written by: George Wolberg, 2017
// ======================================================================


#include "Camera.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Camera::Camera:
//
// Camera constructor function.
//
Camera::Camera()
{
	initializeGLFunctions();
	m_eye    = vec3(1.0f, 2.0f, 1.0f);
	m_target = vec3(0.0f, 0.0f, 0.0f);
	set(m_eye, m_target);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Camera::set:
//
// set Camera position and direction.
//
void
Camera::set(vec3 eye, vec3 target)
{
	m_eye       = eye;
	m_target    = target;
	m_cameraRotation = cartesianToSpherical(m_eye);
	
	// keep phi within -2PI to +2PI for easy 'up' comparison;
	// if phi is between 0 to PI or -PI to -2PI, 
	// make 'up' be positive Y, other wise make it negative Y
	float phi   = m_cameraRotation.y();
	checkPhi(phi, m_up);
	m_cameraRotation.setY(phi);

	// init camera orientation
	vec3  dir    = (m_target - m_eye).normalized();
	vec3 worldUp = vec3(0.0f, m_up, 0.0f);
	vec3 right   = vec3::crossProduct(dir, worldUp).normalized();
	vec3 up      = vec3::crossProduct(right, dir).normalized();
	m_view.setToIdentity();
	m_view.lookAt(m_eye, m_target, up);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Camera::rotate:
//
// move Camera on a sphere 
// spherical coordinate system (theta, phi)
//
void
Camera::rotate(vec2 &rot)
{
	float theta = m_cameraRotation.x();	
	if(m_up > 0.0f) theta += rot.x();
	else            theta -= rot.x();
	float phi   = m_cameraRotation.y();
	phi -= rot.y();

	// keep phi within -2PI to +2PI for easy 'up' comparison;
	// if phi is between 0 to PI or -PI to -2PI, 
	// make 'up' be positive Y, otherwise make it negative Y
	checkPhi(phi, m_up);
	m_cameraRotation.setX(theta);
	m_cameraRotation.setY(phi);
	sphericalToCartesian(theta, phi, m_eye);
	vec3 dir     = (m_target - m_eye).normalized();
	vec3 worldUp = vec3(0.0f, m_up, 0.0f);
	vec3 right   = vec3::crossProduct(dir, worldUp).normalized();
	vec3 up      = vec3::crossProduct(right, dir).normalized();
	m_view.setToIdentity();
	m_view.lookAt(m_eye, m_target, up);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Camera::zoom:
//
// zoom in/out Camera 
// 
//
void
Camera::zoom(float dist) 
{
	float theta = m_cameraRotation.x();
	float phi   = m_cameraRotation.y();
	vec3  dir   = m_target - m_eye;
	float cameraDistance = dir.length();
	cameraDistance -= dist * 0.001f;
	float x = cameraDistance * cos(theta) * sin(phi);
	float z = cameraDistance * sin(theta) * sin(phi);
	float y = cameraDistance * cos(phi);

	m_eye.setX(x);
	m_eye.setY(y);
	m_eye.setZ(z);

	dir.normalize();
	vec3 worldUp = vec3(0.0f, m_up, 0.0f);
	vec3 right = vec3::crossProduct(dir, worldUp).normalized();
	vec3 up = vec3::crossProduct(right, dir).normalized();
	m_view.setToIdentity();
	m_view.lookAt(m_eye, m_target, up);
}
