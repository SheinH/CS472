// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW0D.h - Header file for HW0D class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW0D_H
#define HW0D_H

#include "HW.h"

// ----------------------------------------------------------------------
// standard include files
//

class HW0d : public HW {
	Q_OBJECT
public:
	HW0d(const QGLFormat &glf, QWidget *parent=0);	// constructor
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters
	void		initBuffers	();		// init buffers for vertices and colors
	void		initShaders	();		// init shaders

public slots:
	void		reverseOn	();		// respond to transformation reversal (on)
	void		reverseOff	();		// respond to transformation reversal (off)
	void		shaderOn	();		// respond to shader mode (on)
	void		shaderOff	();		// respond to shader mode (off)
	void		timeOut		();

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene
	void		renderGLSL	();		// render using shaders
	void		renderGL	();		// render using standard opengl
	void		setCodes	();

private:
	float		  m_xmax;			// ortho x range
	float		  m_ymax;			// ortho y range
	bool		  m_rst;			// reverse order of transformations flag
	bool		  m_glsl;			// GLSL mode or gl mode flag
	QSlider		 *m_sliderTheta;		// rotation slider
	QSpinBox	 *m_spinBoxTheta;		// rotation spinbox
	QRadioButton	 *m_radio[4];			// radio buttons
	QTextEdit	 *m_codes;
	std::vector<vec2> m_points;			// vector of 2D points (XY)
	std::vector<vec3> m_colors;			// vector of 3D colors (RGB)
	int		  m_numPoints;			// number of 2D points
	QMatrix4x4	  m_projection;			// 4x4 projection matrix
	QMatrix4x4	  m_ModelMatrix;		// 4x4 transformation matrix
	QPoint		  m_mousePosition;		// latest mouse position
	GLuint		  m_vertexBuffer;		// vertex buffer ID
	GLuint		  m_colorBuffer;		// color buffer  ID

	QTimer		 *m_timer;
	float		  m_angle;
};

#endif // HW0C_H
