// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW4a.h - Header file for HW4a class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW4A_H
#define HW4A_H


#include "HW.h"
#include "Camera.h"
#include "Light.h"
#include "Geometry.h"
#include "Scene.h"
#include "Sphere.h"



// ----------------------------------------------------------------------
// standard include files
//

class HW4a : public HW 
{
	Q_OBJECT
public:
	// constructor
	HW4a		(const QGLFormat &glf, QWidget *parent = 0);		
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters
	void		initShaders	();		// init shaders
	void		initScene	();


public slots:
	void		showLight(int);
	void		playPauseAnimation();
	void		animate();
	void		changeDisplay(int val);


protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene
	void		mousePressEvent	(QMouseEvent *);
	void		wheelEvent	(QWheelEvent *);
	void		mouseMoveEvent	(QMouseEvent *);
		

private:
	int		m_winW;			// window width
	int		m_winH;			// window height

	int		m_numPoints;		// number of 2D points
	QImage		m_image;		// texture image
	float		m_angularSpeed;
	vec2		m_mousePressPosition;

	QTimer		*m_timer;

	QMatrix4x4	 m_projection;		// 4x4 projection matrix


	Light		*m_light;
	Camera		*m_camera;
	float		 m_angle;

	GLuint		 m_depthFrameBuffer;
	GLuint		 m_depthTexture;
	GLuint		 m_quad_vertexbuffer;

	Sphere		*m_sun;
	Sphere		*m_earth;
	Sphere		*m_moon;


	QCheckBox	*m_checkBox[3];
	bool		 m_showLight;
	bool		 m_animate;
	QPushButton	*m_buttonStart;
	QComboBox	*m_comboBoxDisplay;		// display mode combobox
	int		 m_displayMode;

};

#endif // HW4A_H
