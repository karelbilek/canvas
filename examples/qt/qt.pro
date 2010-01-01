# -------------------------------------------------
# Project created by QtCreator 2009-12-30T22:31:01
# -------------------------------------------------
TARGET = qt
TEMPLATE = app
INCLUDEPATH += ../../headers/
LIBS += -L../../ \
    -lcan
SOURCES += main.cpp \
    mainwindow.cpp \
    qimage_paint.cpp
HEADERS += mainwindow.h \
    qimage_paint.h
FORMS += mainwindow.ui
