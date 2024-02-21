// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW.h - Header file for HW class. Base class of homework solutions.
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef HW_H
#define HW_H

#define MXPROGRAMS	32
#define MXUNIFORMS	32

#define PII		3.1415926535897931160E0
#define PI2		6.2831853071795862320E0
#define PI_2		1.5707963267948965580E0
#define DEGtoRAD	0.0174532927777777777E0
#define RADtoDEG	57.295778666661658617E0

#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>

typedef QVector2D vec2;
typedef QVector3D vec3;

enum {
	ATTRIB_VERTEX,
	ATTRIB_COLOR,
	ATTRIB_TEXCOORD,
	ATTRIB_NORMAL
};

typedef std::map<QString, GLuint> UniformMap;

extern QString GroupBoxStyle;


// ----------------------------------------------------------------------
// standard include files
//
class HW : public QGLWidget, protected QGLFunctions {

public:
	HW(const QGLFormat &glf, QWidget *parent = 0);
	virtual QGroupBox*	controlPanel	();		// create control panel
	virtual void		reset		();		// reset parameters
	void			initShader	(int, QString, QString, UniformMap);

protected:
	QGLShaderProgram  m_program[MXPROGRAMS];		// GLSL programs
	GLint		  m_uniform[MXPROGRAMS][MXUNIFORMS];	// uniform vars for each program
};




inline QMatrix3x3
setRotationMatrix(vec3 rotation) 
{
	QMatrix3x3 R;
	R.setToIdentity();
	float alpha = qDegreesToRadians(rotation.x());
	float beta  = qDegreesToRadians(rotation.y());
	float gamma = qDegreesToRadians(rotation.z());

	R(0, 0) =  cos(beta)*cos(gamma);
	R(0, 1) = -cos(beta)*sin(gamma);
	R(0, 2) =  sin(beta);

	R(1, 0) =  sin(alpha)*sin(beta)*cos(gamma) + cos(alpha)*sin(gamma);
	R(1, 1) = -sin(alpha)*sin(beta)*sin(gamma) + cos(alpha)*cos(gamma);
	R(1, 2) = -sin(alpha)*cos(beta);

	R(2, 0) = -cos(alpha)*sin(beta)*cos(gamma) + sin(alpha)*sin(gamma);
	R(2, 1) =  cos(alpha)*sin(beta)*sin(gamma) + sin(alpha)*cos(gamma);
	R(2, 2) =  cos(alpha)*cos(beta);
	return R;
}


inline void
vectorRotate(QMatrix3x3 &rot, vec3 &v) 
{
	vec3 t(v);
	v.setX(rot(0, 0)*t.x() + rot(0, 1)*t.y() + rot(0, 2)*t.z());
	v.setY(rot(1, 0)*t.x() + rot(1, 1)*t.y() + rot(1, 2)*t.z());
	v.setZ(rot(2, 0)*t.x() + rot(2, 1)*t.y() + rot(2, 2)*t.z());
}


inline vec3
rot_v(vec3 &v) 
{
	v.normalize();
	float a = acos(vec3::dotProduct(v, vec3(1.0f, 0.0f, 0.0f)));
	float b = acos(vec3::dotProduct(v, vec3(0.0f, 1.0f, 0.0f)));
	float c = acos(vec3::dotProduct(v, vec3(0.0f, 0.0f, 1.0f)));
	vec3 r;
	r.setX(90-qRadiansToDegrees(a));
	r.setY(90-qRadiansToDegrees(c));
	r.setZ(qRadiansToDegrees(b));
	return r;

}

inline vec3
rot_v1tov2(vec3 &v2, vec3 &v1) 
{
	v1.normalize();
	v2.normalize();
	float a = acos(vec3::dotProduct(v1, v2));
	vec3 axis = vec3::crossProduct(v1, v2);
	axis.normalize();
	QQuaternion q = QQuaternion::fromAxisAndAngle(axis, qRadiansToDegrees(a));
	vec3 r = q.toEulerAngles();
	vec3 rr;
	rr.setX(r.x());
	rr.setY(r.y());
	rr.setZ(-r.z());
	return rr;
}

inline vec2
cartesianToSpherical(vec3 &p) 
{
	vec2 angles;
	double dist = sqrt(p.x()*p.x() + p.z()*p.z());
	angles.setX(atan2(p.z(), p.x()));
	angles.setY(atan2(dist, p.y()));
	return angles;
}

inline void
sphericalToCartesian(float theta, float phi, vec3 &eye) 
{
	vec3 dir = -eye;
	float r = dir.length();

	float x = r * cos(theta) * sin(phi);
	float z = r * sin(theta) * sin(phi);
	float y = r * cos(phi);

	eye.setX(x);
	eye.setY(y);
	eye.setZ(z);
}

inline void
sphericalToCartesian(float theta, float phi, float r, vec3 &eye) 
{

	float x = r * cos(theta) * sin(phi);
	float z = r * sin(theta) * sin(phi);
	float y = r * cos(phi);

	eye.setX(x);
	eye.setY(y);
	eye.setZ(z);
}

inline void
checkPhi(float &phi, float &up) 
{
	// Keep phi within -2PI to +2PI for easy 'up' comparison
	if(phi > PI2)       phi -= (float) PI2;
	else if(phi < -PI2) phi += (float) PI2;

	if((phi > 0     && phi < M_PI) ||
	   (phi < -M_PI && phi > -PI2))
		up =  1.0f;
	else    up = -1.0f;
}

#endif // HW_H
