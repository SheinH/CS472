// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Scene.h - Header file for Scene class
//
// Written by: George Wolberg 2017
// ======================================================================

#ifndef SCENE_H
#define SCENE_H

#include "Geometry.h"

// ----------------------------------------------------------------------
// standard include file for objects in the scene
//
class Scene : protected QGLFunctions 
{

public:
	Scene() {} // defual constructor 

	void	addGeometry(Geometry *obj);
	void	render();
	void	renderShadow();


private:
	std::vector<Geometry *>	m_geometry;

};


#endif // SCENE_H
