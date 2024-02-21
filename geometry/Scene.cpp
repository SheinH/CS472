// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2017 by George Wolberg
//
// Scene.cpp - Scene class implementation
// Written by: George Wolberg 2017
// ======================================================================


#include "Scene.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Scene::addGeometry:
//
// add an object to scene list.
//
void
Scene::addGeometry(Geometry *obj) {
	obj->createGeometry();
	m_geometry.push_back(obj);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Scene::renderShadow:
//
// render only objects that cast shodows.
// This is only when we render the scene from light point of view
//
void
Scene::renderShadow()
{
	for(int i = 0; i < (int) m_geometry.size(); ++i) {
		if(m_geometry[i]->drawable() && m_geometry[i]->castShadow())
			m_geometry[i]->display();
	}

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Scene::render:
//
// render only objects that are drawable.
// 
//
void
Scene::render()
{
	for(int i = 0; i < (int)m_geometry.size(); ++i) {
		if(m_geometry[i]->drawable())
			m_geometry[i]->display();
	}

}


