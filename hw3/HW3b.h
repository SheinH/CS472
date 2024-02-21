// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW3b.h - Header file for HW3b class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW3B_H
#define HW3B_H


#include "HW.h"
#include "Camera.h"
#include "Light.h"

#define MAXGRID 64



// ----------------------------------------------------------------------
// standard include files
//

class HW3b : public HW {
	Q_OBJECT
public:
	// constructor
	HW3b		(const QGLFormat &glf, QWidget *parent = 0);		
	QGroupBox*	controlPanel	();		// create control panel
	void		reset		();		// reset parameters

public slots:
	void		changeDisplay	  (int);	// change display mode
	void		changeGeom	  (int);	// change geometry
	void		changeSize	  (int);	// change grid size
	void		changeSpeed	  (int);	// change animation speed
	void		playPauseAnimation();
	void		timeOut		  ();

protected:
	void		initializeGL	();		// init GL state
	void		resizeGL	(int, int);	// resize GL widget
	void		paintGL		();		// render GL scene
	void		initTexture	();		// init texture image
	void		initShaders	();		// init shaders
	void		resetMesh	();		// reset height of surface (z)
	void		getForce	();		// used to compute new height
	void		getVelocity	();		// used to compute new height
	void		getPosition	();		// get surface height
	void		setDT		();		// used to compute new height
	void		initVertices	();		// init XY coords of mesh
	void		initBuffers	();		// init vertices
	void		flatten2D	();		// copy 2D array into 1D array
	void		uploadVertices	();		// copy 3D vertices to GPU
	void		uploadTexCoords	();		// copy 2D texture coords to GPU
	void		uploadIndices	();		// copy vertex indices to GPU
	void		uploadNormals   ();		// copy normals to GPU
	void		uploadColors    ();		// copy colors to GPU
	void		mousePressEvent (QMouseEvent *);
	void		mouseMoveEvent  (QMouseEvent *);
	void		wheelEvent      (QWheelEvent *);
	void		getFaceNorms    ();
	void		getVertNorms    ();


private:
	int		  m_winW;			// window width
	int		  m_winH;			// window height
	int		  m_grid;
	int		  m_displayMode;
	int		  m_geometryMode;
	int		  m_sizeMode;
	int		  m_speed;
	QComboBox	 *m_comboBoxDisplay;		// display mode combobox
	QComboBox	 *m_comboBoxGeom;		// geometry combobox
	QSlider		 *m_sliderSize;
	QSpinBox	 *m_spinBoxSize;
	QSlider		 *m_sliderSpeed;		// speed slider
	QSpinBox	 *m_spinBoxSpeed;		// speed combobox
	QPushButton	 *m_buttonStart;		// play/pause button

	std::vector<vec3> m_points;			
	std::vector<vec2> m_coords;
	std::vector<vec3> m_normals;
	std::vector<vec3> m_colors;
	int		  m_numPoints;			 // number of 2D points
	QMatrix4x4	  m_projection;			 // 4x4 projection matrix
	QImage		  m_image;			 // texture image
	GLuint		  m_texture;			 // shader index to texture unit
	vec3		  m_vertices    [MAXGRID][MAXGRID]; // vector of 3D points (XYZ)
	vec2		  m_texCoords   [MAXGRID][MAXGRID]; // vector of 2D coords (UV)
	vec3		  m_vertNorms   [MAXGRID][MAXGRID]; // vector of normals
	vec3		  m_faceNorms[2][MAXGRID][MAXGRID]; // vector of normals
	float		  m_force       [MAXGRID][MAXGRID];
	float		  m_veloc       [MAXGRID][MAXGRID];
	std::vector<ushort>m_indices_triangles;
	std::vector<ushort>m_indices_wireframe;
	int		  m_resetMode;
	GLuint		  m_vertexBuffer;
	GLuint		  m_texBuffer;
	GLuint		  m_normalBuffer;
	GLuint		  m_colorBuffer;
	GLuint		  m_indicesBuffer[2];
	float		  m_angularSpeed;
	float		  m_dt;
	vec2		  m_mousePressPosition;

	QTimer		 *m_timer;
	bool		  m_wave;

	Light		  *m_light;
	Camera		  *m_camera;


};

#endif // HW3B_H
