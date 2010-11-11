#-------------------------------------------------
#
# Project created by QtCreator 2010-10-21T23:40:46
#
#-------------------------------------------------

QT       += core gui

TARGET = qznonstat
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/zchart.cpp \
        src/zframes.cpp \
    src/datakeeper.cpp \
    src/ardataanalysis.cpp \
    src/common_math_tools.cpp \
    src/distanceelement.cpp \
    src/armodeltune.cpp

HEADERS  += src/mainwindow.h \
            src/zchart.h \
            src/zframes.h \
    src/datakeeper.h \
    src/ardataanalysis.h \
    src/common_math_tools.h \
    src/pcg.hpp \
    src/cholesky.hpp \
    src/precond.hpp \
    src/distanceelement.h \
    src/armodeltune.h

FORMS    += src/mainwindow.ui src/armodeltune.ui
