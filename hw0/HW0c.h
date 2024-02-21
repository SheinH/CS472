// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW0c.h - Header file for HW0c class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW0C_H
#define HW0C_H

#include "HW.h"

// ----------------------------------------------------------------------
// standard include files
//

class HW0c : public HW {
	Q_OBJECT
public:
	HW0c(const QGLFormat &glf, QWidget *parent=0);	// constructor
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters
	void		initBuffers	();		// init buffers for vertices and colors
	void		initShaders	();		// init shaders

public slots:
	void		changeTheta(int);		// respond to rotation slider
	void		reverseOn();			// respond to color reversal (on)
	void		reverseOff();			// respond to color reversal (off)

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene

	void		mousePressEvent	 (QMouseEvent *);
	void		mouseMoveEvent	 (QMouseEvent *);
	void		mouseReleaseEvent(QMouseEvent *);

private:
	int		  m_winW;			// window width
	int		  m_winH;			// window height
	bool		  m_reverseColor;		// reverse color flag
	QSlider		 *m_sliderTheta;		// rotation slider
	QSpinBox	 *m_spinBoxTheta;		// rotation spinbox
	QRadioButton	 *m_radio[2];			// radio button for reversing color
	std::vector<vec2> m_points;			// vector of 2D points (XY)
	std::vector<vec3> m_colors;			// vector of 3D colors (RGB)
	int		  m_numPoints;			// number of 2D points
	QMatrix4x4	  m_projection;			// 4x4 projection matrix
	QMatrix4x4	  m_ModelMatrix;		// 4x4 transformation matrix
	QPoint		  m_mousePosition;		// latest mouse position
	GLuint		  m_vertexBuffer;		// vertex buffer ID
	GLuint		  m_colorBuffer;		// color buffer  ID
};

#endif // HW0C_H
