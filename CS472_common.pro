TEMPLATE = app
TARGET = CS472
QT += widgets opengl 
CONFIG += qt debug_and_release


Release:OBJECTS_DIR = release/obj
Release:MOC_DIR     = release/moc
Debug:OBJECTS_DIR   = debug/obj
Debug:MOC_DIR       = debug/moc

INCLUDEPATH     += . $$CS742PATH/ $$CS742PATH/geometry $$CS742PATH/camera $$CS742PATH/lighting 

win32-msvc*{
	RC_FILE        += $${CS742PATH}/CS472.rc
	LIBS           += -lopengl32
	QMAKE_CXXFLAGS += /MP /Zi
	QMAKE_LFLAGS   += /MACHINE:X64
}

macx{
        QMAKE_SONAME_PREFIX = @executable_path/../Frameworks
        QMAKE_LFLAGS += -Wl,-rpath,@executable_path/../Frameworks
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
	CONFIG+=sdk_no_version_check
	ICON = CS472.icns
}

unix:!macx{
	CONFIG += C++11
}

win32-g++{
	RC_FILE        += $${CS742PATH}/CS472.rc
	LIBS += -lopengl32
}


# Input
HEADERS +=	$${CS742PATH}/MainWindow.h		\
		$$CS742PATH/HW.h			\
		$$CS742PATH/dummy.h			\
		$$CS742PATH/geometry/Geometry.h		\
		$$CS742PATH/geometry/Cube.h		\
		$$CS742PATH/geometry/Sphere.h		\
		$$CS742PATH/geometry/Cylinder.h		\
		$$CS742PATH/geometry/Cone.h		\
		$$CS742PATH/geometry/Scene.h		\
		$$CS742PATH/lighting/Light.h		\
		$$CS742PATH/camera/Camera.h
		
SOURCES +=	$$CS742PATH/main.cpp			\ 
		$$CS742PATH/MainWindow.cpp 		\
		$$CS742PATH/HW.cpp			\
		$$CS742PATH/dummy.cpp			\
		$$CS742PATH/geometry/Cube.cpp		\
		$$CS742PATH/geometry/Sphere.cpp		\
		$$CS742PATH/geometry/Cylinder.cpp	\
		$$CS742PATH/geometry/Cone.cpp		\
		$$CS742PATH/geometry/Scene.cpp		\
		$$CS742PATH/lighting/Light.cpp		\
		$$CS742PATH/camera/Camera.cpp
