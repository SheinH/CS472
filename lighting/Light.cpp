// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Light.cpp -  file for Light class
//
// Written by: George Wolberg, 2017
// ======================================================================


#include "Light.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Light::Light:
//
// Light constructor function.
//
Light::Light()
{
	initializeGLFunctions();
	m_eye    = vec3(1.0f, 2.0f, 1.0f);
	m_target = vec3(0.0f, 0.0f, 0.0f);
	set(m_eye, m_target);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Light::set:
//
// set light position and direction.
//
void
Light::set(vec3 eye, vec3 target)
{
	m_eye       = eye;
	m_target    = target;
	
	m_lightRotation = cartesianToSpherical(m_eye);
	float phi   = m_lightRotation.y();

	// keep phi within -2PI to +2PI for easy 'up' comparison;
	// if phi is between 0 to PI or -PI to -2PI, 
	// make 'up' be positive Y, otherwise make it negative Y
	checkPhi(phi, m_up);
	m_lightRotation.setY(phi);

	// init camera orientation
	vec3 dir     = (m_target - m_eye).normalized();
	vec3 worldUp = vec3(0.0f, m_up, 0.0f);
	vec3 right   = vec3::crossProduct(dir, worldUp).normalized();
	vec3 up      = vec3::crossProduct(right, dir).normalized();
	m_view.setToIdentity();
	m_view.lookAt(m_eye, m_target, up);

	// create a sphere at the light position
	m_sphere = new Sphere(m_eye, 0.05f, 16);
	m_sphere->setColor(vec3(1.0f, 1.0f, 1.0f));
	m_sphere->createGeometry();

	m_vertices.resize(2);
	m_vertices[0] = m_eye;
	m_vertices[1] = m_target;
	m_colors.resize(2);
	m_colors[0] = vec3(1.0f, 1.0f, 1.0f);
	m_colors[1] = vec3(1.0f, 1.0f, 1.0f);

	// init vertex buffer for line drawing to show light direction
	glGenBuffers(1, &m_vertexBuffer);
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), &m_vertices[0], GL_STATIC_DRAW);

	// init color buffer for line drawing
	glGenBuffers(1, &m_colorBuffer);

	// bind color buffer to the GPU and copy the colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Light::display:
//
// display light (draw a sphere and line for direction).
//
void
Light::display()
{
	// display light bulb as a sphere 
	m_sphere->display(1);
	
	// bind the buffer to the GPU; all future drawing calls gets data from this buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	// enable the assignment of attribute vertex variable
	glEnableVertexAttribArray(ATTRIB_VERTEX);

	// assign the buffer object to the attribute vertex variable
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, false, 0, NULL);
	
	// bind the buffer to the GPU; all future drawing calls gets data from this buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);

	// enable the assignment of attribute vertex variable
	glEnableVertexAttribArray(ATTRIB_COLOR);

	// assign the buffer object to the attribute vertex variable
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	// draw line: direction of light
	glDrawArrays(GL_LINES, 0, 2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Light::rotate:
//
// move light on a sphere
//
void
Light::rotate(vec2 &rot)
{
	float theta = m_lightRotation.x();	
	float phi   = m_lightRotation.y();
	if(m_up > 0.0f) theta -= rot.x();
	else            theta += rot.x();
	phi += rot.y();

	// keep phi within -2PI to +2PI for easy 'up' comparison;
	// if phi is between 0 to PI or -PI to -2PI, 
	// make 'up' be positive Y, other wise make it negative Y
	checkPhi(phi, m_up);
	m_lightRotation.setX(theta);
	m_lightRotation.setY(phi);
	sphericalToCartesian(theta, phi, m_eye);
	vec3 dir = (m_target - m_eye).normalized();
	vec3 worldUp = vec3(0.0f, m_up, 0.0f);
	vec3 right   = vec3::crossProduct(dir, worldUp).normalized();
	vec3 up      = vec3::crossProduct(dir, right).normalized();
	m_view.setToIdentity();
	m_view.lookAt(m_eye, m_target, up);

	m_sphere->translate(m_eye);
	m_vertices[0] = m_eye;
	m_vertices[1] = m_target;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), &m_vertices[0], GL_STATIC_DRAW);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Light::rotate:
//
// move light on a circle
//
void
Light::rotate(float dtheta) 
{
	float theta = m_lightRotation.x();
	if(m_up > 0.0f) theta -= dtheta;
	else            theta += dtheta;

	double r = sqrt(m_eye.x()*m_eye.x()+m_eye.z()*m_eye.z());
	double dx = r*cos(theta);
	double dz = r*sin(theta);
	m_eye.setX(dx);
	m_eye.setZ(dz);
	m_lightRotation = cartesianToSpherical(m_eye);

	vec3 dir = (m_target - m_eye).normalized();
	vec3 worldUp = vec3(0.0f, m_up, 0.0f);
	vec3 right   = vec3::crossProduct(dir, worldUp).normalized();
	vec3 up      = vec3::crossProduct(dir, right).normalized();
	m_view.setToIdentity();
	m_view.lookAt(m_eye, m_target, up);

	m_sphere->translate(m_eye);
	m_vertices[0] = m_eye;
	m_vertices[1] = m_target;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), &m_vertices[0], GL_STATIC_DRAW);
}
