// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW4c.h - Header file for HW4c class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW4C_H
#define HW4C_H


#include "HW.h"
#include "Camera.h"
#include "Light.h"
#include "Geometry.h"
#include "Scene.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Cone.h"


// ----------------------------------------------------------------------
// standard include files
//

class HW4c : public HW 
{
	Q_OBJECT
public:
	// constructor
	HW4c		(const QGLFormat &glf, QWidget *parent = 0);		
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters
	void		initShaders	();		// init shaders
	void		initScene	();


public slots:
	void		showLight(int);
	void		showDepthMap(int);
	void		playPauseAnimation();
	void		moveLight();


protected:
	void		initializeGL	   ();		// init GL state
	void		resizeGL	   (int, int);	// resize GL widget
	void		paintGL		   ();		// render GL scene
	void		mousePressEvent    (QMouseEvent *);
	void		wheelEvent	   (QWheelEvent *);
	void		mouseMoveEvent	   (QMouseEvent *);
	bool		initDepthBuffer	   ();
	void		computeDepthMap	   ();
	void		GetShadowMapExtends(vec3 &extends);
	void		drawScene	   (bool depthMap);
	void		renderScene	   ();
	void		displayDepthMap	   ();
	
		

private:
	int		 m_winW;			// window width
	int		 m_winH;			// window height

	int		 m_numPoints;			 // number of 2D points
	QImage		 m_image;			 // texture image
	GLuint		 m_texture;			 // shader index to texture unit
	float		 m_angularSpeed;
	vec2		 m_mousePressPosition;

	QTimer		*m_timer;

	QMatrix4x4	 m_projection;			 // 4x4 projection matrix
	QMatrix4x4	 m_depthMVP;			 // 4x4 depth view  matrix
	QMatrix4x4	 m_biasMatrix;			 // normalize vertices from [-1, 1] to [0, 1] for depth map uv sampling



	Light		*m_light;
	Camera		*m_camera;
	Scene		 m_scene;

	GLuint		 m_depthFrameBuffer;
	GLuint		 m_depthTexture;
	GLuint		 m_quad_vertexbuffer;

	Cube		*m_cube[5];
	Sphere		*m_sphere[3];
	Cylinder	*m_cylinder[3];
	Cone		*m_cone[2];

	QCheckBox	*m_checkBox[3];
	bool		 m_showDepthMap;
	bool		 m_showLight;
	bool		 m_computeDepth;
	bool		 m_rotateLigh;
	QPushButton	*m_buttonStart;


};

#endif // HW4C_H
