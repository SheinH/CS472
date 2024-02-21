// ======================================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// main.c - main() function.
//
// Written by: George Wolberg, 2022
// ======================================================================

#include "MainWindow.h"

int 
main(int argc, char **argv)
{
	// optional code to handle high-density (e.g. retina) display
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication::setStyle(QStyleFactory::create("fusion"));

	// create application and window; display maximized window
	QApplication app(argc, argv);		// create application
	MainWindow window;			// create UI window
	window.showMaximized();			// display maximized window
	return app.exec();			// infinite processing loop
}
