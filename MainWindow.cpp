// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// MainWindow.cpp - MainWindow class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "MainWindow.h"
#include "dummy.h"
#include "hw0/HW0a.h"
#include "hw0/HW0b.h"
#include "hw0/HW0c.h"
#include "hw0/HW0d.h"
#include "hw1/HW1a.h"
#include "hw1/HW1b.h"
#include "hw2/HW2a.h"
#include "hw2/HW2b.h"
#include "hw3/HW3a.h"
#include "hw3/HW3b.h"
#include "hw4/HW4a.h"
#include "hw4/HW4b.h"
#include "hw4/HW4c.h"

QString frameStyle   = "QFrame#frame {				\
			border: 2px solid gray;			\
			border-radius: 9px;			\
			background-color: rgb(230, 232, 232);	\
			}";

MainWindow *MainWindowP = NULL;

QString glslPath;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::MainWindow:
//
// MainWindow constructor.
//
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

	setWindowTitle("Computer Graphics Homework");
	setWindowIcon(QIcon(":/CS472.png")); 
	// reasonable minimum size
	setMinimumSize(QSize(800, 600));	
	// set global variable for main window pointer
	MainWindowP = this;

	// create stacked widgets to hold multiple control panels and image displays
	m_stackWidgetPanels= new QStackedWidget;
	m_stackWidgetViews = new QStackedWidget;

	// create widgets, actions, and menus
	createWidgets();	// insert your widgets here
	createActions();	// insert your actions here
	createMenus();		// insert your menus   here

	// add stacked widget to vertical box layout 
	QFrame *frame = new QFrame;
	frame->setObjectName("frame");
	frame->setStyleSheet(frameStyle);
	frame->setMinimumWidth(300);
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_stackWidgetPanels);
	vbox->addStretch(1);
	vbox->addLayout(createExitButtons());
	frame->setLayout(vbox);

	// add all widgets to grid layout
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(createGroupView());
	hbox->setStretch(0, 1);
	hbox->addWidget(frame);

	// create container widget and set its layout
	QWidget *w = new QWidget;
	w->setLayout(hbox);
	setCentralWidget(w);

	// set stacked widget to latest solution
	m_stackWidgetViews->setCurrentIndex(DUMMY);
	m_stackWidgetPanels->setCurrentIndex(DUMMY);
	m_hw[m_hwName[DUMMY]]->setMaximumSize(1, 1); // nothing to show
	m_hw[m_hwName[DUMMY]]->setMinimumSize(1, 1); // nothing to show

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createWidgets:
//
// Create user-defined widgets for display and control panel.
//
void
MainWindow::createWidgets()
{
	// create list of hw names; m_hwName name will be used for
	// menu name and as key for class in m_hw container
	m_hwName << "Blank page"
		<< "0a: Square"
		<< "0b: Aspect ratio"
		<< "0c: GLSL"
		<< "0d: Transformations"
		<< "1a: P"
		<< "1b: Triangle"
		<< "2a: P (GLSL)"
		<< "2b: Triangle (GLSL)"
		<< "3a: Triangle (Texture/Wire)"
		<< "3b: Wave"
		<< "4a: Transformation stack"
		<< "4b: Bounce"
		<< "4c: Shadow";

	// format for legacy OpenGL with older GLSL (supporting attribute/varying qualifiers)
	QGLFormat glfLegacy = QGLFormat::defaultFormat();	// base format
	glfLegacy.setProfile(QGLFormat::CompatibilityProfile);	// also support legacy version
	glfLegacy.setSampleBuffers(true);			// multisample buffer support for antialiasing (AA)
	glfLegacy.setSamples(16);				// number of samples per fragment for AA
	glfLegacy.setSwapInterval(0);
	glfLegacy.setDefaultFormat(glfLegacy);			// use modified parameters

	// format for modern OpenGL (3.3+) with newer GLSL (supporting in/out/layout)
	QGLFormat glfModern = QGLFormat::defaultFormat();	// base format
	glfModern.setVersion(3, 3);				// Mac requires 3.3+ for core profile
	glfModern.setProfile(QGLFormat::CoreProfile);		// don't use deprecated functions
	glfModern.setSampleBuffers(true);			// multisample buffer support for antialiasing (AA)
	glfModern.setSamples(16);				// number of samples per fragment (for AA)
	glfModern.setSwapInterval(0);
	glfModern.setDefaultFormat(glfModern);			// use modified parameters

	// instantiate homework solution classes
	m_hw[m_hwName[DUMMY]] = new Dummy(glfLegacy);
	m_hw[m_hwName[HW0A ]] = new HW0a (glfLegacy);
	m_hw[m_hwName[HW0B ]] = new HW0b (glfLegacy);
	m_hw[m_hwName[HW0C ]] = new HW0c (glfModern);
	m_hw[m_hwName[HW0D ]] = new HW0d (glfLegacy);
	m_hw[m_hwName[HW1A ]] = new HW1a (glfLegacy);
	m_hw[m_hwName[HW1B ]] = new HW1b (glfLegacy);
	m_hw[m_hwName[HW2A ]] = new HW2a (glfModern);
	m_hw[m_hwName[HW2B ]] = new HW2b (glfModern);
	m_hw[m_hwName[HW3A ]] = new HW3a (glfModern);
	m_hw[m_hwName[HW3B ]] = new HW3b (glfModern);
	m_hw[m_hwName[HW4A ]] = new HW4a (glfModern);
	m_hw[m_hwName[HW4B ]] = new HW4b (glfLegacy);
	m_hw[m_hwName[HW4C ]] = new HW4c (glfModern);

	// add control panels to stacked widget
	for(int i = 0; i < (int) m_hwName.size(); ++i)
		m_stackWidgetPanels->addWidget(m_hw[m_hwName[i]]->controlPanel());
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createActions:
//
// Create actions to associate with menu and toolbar selection.
//
void
MainWindow::createActions() 
{
	//////////////////////////////
	// HW 0 actions
	//////////////////////////////

	m_actionHW0a = new QAction(m_hwName[HW0A], this);
	m_actionHW0a->setData(HW0A);

	m_actionHW0b = new QAction(m_hwName[HW0B], this);
	m_actionHW0b->setData(HW0B);

	m_actionHW0c = new QAction(m_hwName[HW0C], this);
	m_actionHW0c->setData(HW0C);

	m_actionHW0d = new QAction(m_hwName[HW0D], this);
	m_actionHW0d->setData(HW0D);

	//////////////////////////////
	// HW 1 actions
	//////////////////////////////

	m_actionHW1a = new QAction(m_hwName[HW1A], this);
	m_actionHW1a->setData(HW1A);

	m_actionHW1b = new QAction(m_hwName[HW1B], this);
	m_actionHW1b->setData(HW1B);

	//////////////////////////////
	// HW 2 actions
	//////////////////////////////
	m_actionHW2a = new QAction(m_hwName[HW2A], this);
	m_actionHW2a->setData(HW2A);

	m_actionHW2b = new QAction(m_hwName[HW2B], this);
	m_actionHW2b->setData(HW2B);

	//////////////////////////////
	// HW 3 actions
	//////////////////////////////
	m_actionHW3a = new QAction(m_hwName[HW3A], this);
	m_actionHW3a->setData(HW3A);

	m_actionHW3b = new QAction(m_hwName[HW3B], this);
	m_actionHW3b->setData(HW3B);

	//////////////////////////////
	// HW 4 actions
	//////////////////////////////
	m_actionHW4a = new QAction(m_hwName[HW4A], this);
	m_actionHW4a->setData(HW4A);
	m_actionHW4b = new QAction(m_hwName[HW4B], this);
	m_actionHW4b->setData(HW4B);
	m_actionHW4c = new QAction(m_hwName[HW4C], this);
	m_actionHW4c->setData(HW4C);

	// one signal-slot connection for all actions;
	// execute() will resolve which action was triggered
	connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(changeHW(QAction*)));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createMenus:
//
// Create menus and install in menubar.
//
void
MainWindow::createMenus() 
{
	// hw0 menu
	m_menuHW0 = menuBar()->addMenu("HW0");
	m_menuHW0->addAction(m_actionHW0a);
	m_menuHW0->addAction(m_actionHW0b);
	m_menuHW0->addAction(m_actionHW0c);
	m_menuHW0->addAction(m_actionHW0d);

	// hw1 menu
	m_menuHW1 = menuBar()->addMenu("HW1");
	m_menuHW1->addAction(m_actionHW1a);
	m_menuHW1->addAction(m_actionHW1b);

	// hw2 menu
	m_menuHW2 = menuBar()->addMenu("HW2");
	m_menuHW2->addAction(m_actionHW2a);
	m_menuHW2->addAction(m_actionHW2b);

	// hw3 menu
	m_menuHW3 = menuBar()->addMenu("HW3");
	m_menuHW3->addAction(m_actionHW3a);
	m_menuHW3->addAction(m_actionHW3b);

	// hw4 menu
	m_menuHW4 = menuBar()->addMenu("HW4");
	m_menuHW4->addAction(m_actionHW4a);
	m_menuHW4->addAction(m_actionHW4b);
	m_menuHW4->addAction(m_actionHW4c);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createGroupView:
//
// Create preview window groupbox.
//
QFrame*
MainWindow::createGroupView()
{
	// init group box
	QFrame *frame = new QFrame();
	frame->setObjectName("frame");
	frame->setStyleSheet(frameStyle);

	// create a stack widget to handle multiple displays
	// one view per homework problem
	for(int i = 0; i < (int) m_hwName.size(); ++i)
		m_stackWidgetViews->addWidget(m_hw[m_hwName[i]]);

	// assemble stacked widget in vertical layout
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(m_stackWidgetViews);
	frame->setLayout(vbox);

	return frame;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::changeHW:
//
// Slot function to change OpenGL canvas and control panel in stacked widget.
//
void
MainWindow::changeHW(QAction* action)
{
	// get code from action
	int index = action->data().toInt();
	m_stackWidgetViews ->setCurrentIndex(index);	// change OpenGL widget 
	m_stackWidgetPanels->setCurrentIndex(index);	// change control panel
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createExitButtons:
//
// Create save/quit buttons.
//
QHBoxLayout *
MainWindow::createExitButtons()
{
	// create pushbuttons
	QPushButton *buttonReset = new QPushButton("Reset");
	QPushButton *buttonQuit  = new QPushButton("Quit");

	// init signal/slot connections
	connect(buttonReset, SIGNAL(clicked()), this, SLOT(reset()));
	connect(buttonQuit , SIGNAL(clicked()), this, SLOT(quit ()));

	// assemble pushbuttons in horizontal layout
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(buttonReset);
	buttonLayout->addWidget(buttonQuit );

	return buttonLayout;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::reset:
//
// Reset application.
//
void
MainWindow::reset()
{
	int index = m_stackWidgetViews->currentIndex();	// index to current OpenGL widget
	m_hw[ m_hwName[index] ]->reset();		// invoke respective reset function
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::quit:
//
// Quit application.
//
void
MainWindow::quit()
{
	// close the dialog window
	close();
}
