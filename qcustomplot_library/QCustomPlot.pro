
QT +=core
QT +=gui
QT +=widgets

TARGET = qcustomplost_library
TEMPLATE = lib

DEFINES += QCUSTOMPLOT_COMPILE_LIBRARY

SOURCES += \
    $$PWD/qcustomplot/qcustomplot.cpp

HEADERS += \
    $$PWD/qcustomplot/qcustomplot.h

include($$PWD/BaseQCustomPlot.pri)

include($$PWD/../OpenCVBase.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR
