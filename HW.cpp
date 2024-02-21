// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW.cpp - HW class. Base class of homework solutions.
//
// Written by: George Wolberg, 2022
// ===============================================================

#ifdef SOLN
#include "../CS472.skel/HW.h"
#else
#include "HW.h"
#endif

QString GroupBoxStyle = "QGroupBox {				\
			border: 1px solid gray;			\
			border-radius: 9px;			\
			font-weight: bold;			\
			margin-top: 0.5em;}			\
			QGroupBox::title {			\
			subcontrol-origin: margin;		\
			left: 10px;				\
			padding: 0 3px 0 3px;			\
			}";


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::HW:
//
// HW constructor.
// This is base class for homework solutions that will replace
// the control panel, reset function, and add homework solution. 
//
HW::HW(const QGLFormat &glf, QWidget *parent) : QGLWidget (glf, parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::controlPanel:
//
// Create a control panel of widgets for homework solution.
//
QGroupBox*
HW::controlPanel()
{
	return NULL;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::reset:
//
// Reset parameters in control panel.
//
void
HW::reset() {}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW::initShader:
//
// Initialize vertex and fragment shaders.
//
void
HW::initShader(int shaderID, QString vshaderName, QString fshaderName, UniformMap uniforms)
{


	// due to bug in Qt, in order to use higher OpenGL version (>2.1), we need to add lines
	// up to initShader() to render properly
	uint vao;

	typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
	typedef void (APIENTRY *_glBindVertexArray) (GLuint);

	_glGenVertexArrays glGenVertexArrays;
	_glBindVertexArray glBindVertexArray;

	glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
	glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// compile vertex shader
	bool flag = m_program[shaderID].addShaderFromSourceFile(QGLShader::Vertex, vshaderName);
	if(!flag) {
		QMessageBox::critical(0, "Error", "Vertex shader error: " + vshaderName + "\n" +
					m_program[shaderID].log(), QMessageBox::Ok);
		exit(-1);
	}

	// compile fragment shader
	if(!m_program[shaderID].addShaderFromSourceFile(QGLShader::Fragment, fshaderName)) {
		QMessageBox::critical(0, "Error", "Fragment shader error: " + fshaderName + "\n" +
					m_program[shaderID].log(), QMessageBox::Ok);
		exit(-1);
	}

	// bind the attribute variable in the glsl program with a generic vertex attribute index;
	// values provided via ATTRIB_VERTEX will modify the value of "a_position")
	glBindAttribLocation(m_program[shaderID].programId(), ATTRIB_VERTEX,	"a_Position");
	glBindAttribLocation(m_program[shaderID].programId(), ATTRIB_COLOR,	"a_Color"   );
	glBindAttribLocation(m_program[shaderID].programId(), ATTRIB_TEXCOORD,	"a_TexCoord");
	glBindAttribLocation(m_program[shaderID].programId(), ATTRIB_NORMAL,	"a_Normal"  );

	// link shader pipeline; attribute bindings go into effect at this point
	if(!m_program[shaderID].link()) {
		QMessageBox::critical(0, "Error", "Could not link shader: " + vshaderName + "\n" +
					m_program[shaderID].log(), QMessageBox::Ok);
		qDebug() << m_program[shaderID].log();
		exit(-1);
	}


	// iterate over all uniform variables; map each uniform name to shader location ID
	for(std::map<QString, GLuint>::iterator iter = uniforms.begin(); iter != uniforms.end(); ++iter) {
		QString uniformName = iter->first;
		GLuint  uniformID   = iter->second;

		// get storage location
		m_uniform[shaderID][uniformID]=glGetUniformLocation(m_program[shaderID].programId(),
			  uniformName.toStdString().c_str());
		if(m_uniform[shaderID][uniformID] < 0) {
			qDebug() << m_program[shaderID].log();
			qDebug() << "Failed to get the storage location of " + uniformName;
		}
	}
}
