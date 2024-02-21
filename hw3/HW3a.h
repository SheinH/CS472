// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW3a.h - Header file for HW3a class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW3A_H
#define HW3A_H

#include "HW.h"

// ----------------------------------------------------------------------
// standard include files
//

class HW3a : public HW {
	Q_OBJECT
public:
	// constructor
	HW3a		(const QGLFormat &glf, QWidget *parent = 0);
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters

public slots:
	void		changeTheta	(int);		// respond to rotation slider
	void		changeSubdiv	(int);		// respond to subdivision slider
	void		changeTwist	(int);		// respond to twist checkbox
	void		changeWire	(int);		// respond to wireframe checkbox

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene
	void		divideTriangle	(vec2,vec2,vec2,int);	// subdivide triangle
	void		triangle	(vec2,vec2,vec2);	// process single triangle
	void		initVertexBuffer();		// init vertices
	void		initTexture	();		// init texture image
	void		initShaders	();		// init shaders

private:
	int		  m_winW;			// window width
	int		  m_winH;			// window height
	bool		  m_twist;			// twist flag
	bool		  m_wire;			// wireframe flag
	float		  m_theta;			// rotation angle
	int		  m_subdivisions;		// triangle subdivisions
	QSlider		 *m_sliderTheta;		// rotation slider
	QSlider		 *m_sliderSubdiv;		// subdivision slider
	QSpinBox	 *m_spinBoxTheta;		// rotation spinbox
	QSpinBox	 *m_spinBoxSubdiv;		// subdivision spinbox
	QCheckBox	 *m_checkBoxTwist;		// twist checkbox
	QCheckBox	 *m_checkBoxWire;		// wireframe checkbox
	std::vector<vec2> m_points;			// vector of 2D points (XY)
	std::vector<vec2> m_coords;			// vector of 2D coords (UV)
	int		  m_numPoints;			// number of 2D points
	QMatrix4x4	  m_projection;			// 4x4 projection matrix
	QMatrix4x4	  m_modelview;			// 4x4 modelview  matrix
	QImage		  m_image;			// texture image
	GLuint		  m_texture;			// shader index to texture unit
	GLuint		  m_vertexBuffer;		// vertex buffer id
	GLuint		  m_texBuffer;			// texture coord buffer id
};

#endif // HW3A_H
