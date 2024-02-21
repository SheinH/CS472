// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// MainWindow.h - Header file for MainWindow class
//
// Written by: George Wolberg, 2022
// ======================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// ----------------------------------------------------------------------
// standard include files
//
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include "HW.h"

enum {DUMMY, HW0A, HW0B, HW0C, HW0D, HW1A, HW1B, HW2A, HW2B, HW3A, HW3B, HW4A, HW4B, HW4C};


typedef std::map<QString, HW*> hw_type;

class MainWindow : public QMainWindow 
{
	Q_OBJECT

public:
	// constructor
	MainWindow	(QWidget *parent = 0);


public slots:
	void		createWidgets	();
	void		changeHW	(QAction*);
	void		reset		();
	void		quit		();

protected:
	QFrame*		createGroupView	  ();
	void		createActions	  ();
	void		createMenus	  ();
	QHBoxLayout*	createExitButtons ();

private:
	// homework objects
	QStringList	 m_hwName;
	hw_type		 m_hw;

	// widgets
	QStackedWidget	*m_stackWidgetPanels;
	QStackedWidget	*m_stackWidgetViews;

	// menus
	QMenu		*m_menuHW0;
	QMenu		*m_menuHW1;
	QMenu		*m_menuHW2;
	QMenu		*m_menuHW3;
	QMenu		*m_menuHW4;

	// actions
	QAction		*m_actionHW0a;
	QAction		*m_actionHW0b;
	QAction		*m_actionHW0c;
	QAction		*m_actionHW0d;
	QAction		*m_actionHW1a;
	QAction		*m_actionHW1b;
	QAction		*m_actionHW2a;
	QAction		*m_actionHW2b;
	QAction		*m_actionHW3a;
	QAction		*m_actionHW3b;
	QAction		*m_actionHW4a;
	QAction		*m_actionHW4b;
	QAction		*m_actionHW4c;
};

extern MainWindow	*MainWindowP;

#endif // MAINWINDOW_H
