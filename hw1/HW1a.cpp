// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW1a.cpp - HW1a class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW1a.h"

// init array of 16 vertices for letter 'P'
float Vertices[] = {
        -0.5f, -0.75f,
        -0.5f, -0.5f,
        -0.5f, -0.25f,
        -0.5f, 0.0f,
        -0.5f, 0.25f,
        -0.5f, 0.5f,
        -0.25f, 0.75f,
        0.0f, 0.75f,
        0.25f, 0.75f,
        0.5f, 0.75f,
        0.75f, 0.5f,
        0.75f, 0.25f,
        0.5f, 0.0f,
        0.25f, 0.0f,
        0.0f, 0.0f,
        -0.25f, 0.0f
};

static int DrawModes[] = {
        GL_POINTS,
        GL_LINES,
        GL_LINE_STRIP,
        GL_LINE_LOOP,
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,
        GL_QUADS,
        GL_POLYGON
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::HW1a:
//
// HW1a constructor.
//
HW1a::HW1a(const QGLFormat &glf, QWidget *parent)
        : HW(glf, parent) {
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW1a::initializeGL() {
    // PUT YOUR CODE HERE
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW1a::resizeGL(int w, int h) {
    // PUT YOUR CODE HERE
    m_winW = w;
    m_winH = h;
    float ar = (float) w / h;
    float   x_max;
    float   y_max;
    if(ar > 1.0) {		// wide screen
        x_max = ar;
        y_max = 1.;
    } else {		// tall screen
        x_max = 1.;
        y_max = 1/ar;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-x_max, x_max, -y_max,  y_max, -1.0, 1.0);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::paintGL:
//
// Update GL scene.
//
void
HW1a::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

    int boxWidth = qRound(m_winW / 3.0);
    int boxHeight = qRound(m_winH / 3.0);

    int drawModeNum = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            glViewport(col * boxWidth,row * boxHeight,boxWidth,boxHeight);
            glBegin(DrawModes[drawModeNum]);
            for(int i = 0; i < 16; i++) {
                auto x = Vertices[i * 2];
                auto y = Vertices[i * 2 + 1];
                glVertex2f(x,y);
            }
            glEnd();
            drawModeNum++;
        }
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox *
HW1a::controlPanel() {
    // init group box
    QGroupBox *groupBox = new QGroupBox("Homework 1a");
    groupBox->setStyleSheet(GroupBoxStyle);

    return (groupBox);
}
