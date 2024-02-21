// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Geometry.h - Header file for Geometry class.
//
// Written by: George Wolberg  2017
// ======================================================================

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "HW.h"

// ----------------------------------------------------------------------
// standard include files
// Base class for Cube, Cylinder, Cone, and Shpere
class Geometry : protected QGLFunctions 
{

public:
	Geometry() {}	// default constructor
	virtual	void	display		() {};
	virtual void	createGeometry	() {};
	virtual	void	updateGeometry	() {};
	// set the geometry to cast shadow
	void		castShadow	(bool flag) { m_castShadow = flag; }
	bool		castShadow	()	    { return m_castShadow; }
	// set the geometry to be drawable in scene
	void		setDrawable	(bool flag) { m_drawable = flag; }
	bool		drawable	()	    { return m_drawable; }

private:
	bool m_castShadow;
	bool m_drawable;

};



#endif // GEOMETRY_H
