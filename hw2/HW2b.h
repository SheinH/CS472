// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW2b.h - Header file for HW2b class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW2B_H
#define HW2B_H


#include "HW.h"


// ----------------------------------------------------------------------
// standard include files
//

class HW2b : public HW 
{
	Q_OBJECT
public:
	// constructor
	HW2b		(const QGLFormat &glf, QWidget *parent = 0);
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters
	void		initVertexBuffer();		// init vertices
	void		initShaders	();		// init shaders

public slots:
	void		changeTheta	(int);		// respond to rotation slider
	void		changeSubdiv	(int);		// respond to subdivision slider
	void		changeTwist	(int);		// respond to twist checkbox

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene
	void		divideTriangle(vec2, vec2, vec2, int);	// subdivide triangle
	void		triangle(vec2, vec2, vec2);	// process single triangle

private:
	int		  m_winW;			// window width
	int		  m_winH;			// window height
	bool		  m_twist;			// twist flag
	float		  m_theta;			// rotation angle
	int		  m_subdivisions;		// triangle subdivisions
	QSlider		 *m_sliderTheta;		// rotation slider
	QSlider		 *m_sliderSubdiv;		// subdivision slider
	QSpinBox	 *m_spinBoxTheta;		// rotation spinbox
	QSpinBox	 *m_spinBoxSubdiv;		// subdivision spinbox
	QCheckBox	 *m_checkBoxTwist;		// twist checkbox
	std::vector<vec2> m_points;			// vector of 2D points (XY)
	std::vector<vec3> m_colors;			// vector of 3D colors (RGB)
	int		  m_numPoints;			// number of 2D points
	QMatrix4x4	  m_modelview;			// 4x4 modelview  matrix
	QMatrix4x4	  m_projection;			// 4x4 projection matrix
	GLuint		  m_vertexBuffer;		// vertex buffer id
	GLuint		  m_colorBuffer;		// color  buffer id
};

#endif // HW2B_H
