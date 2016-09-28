
QT += core gui
QT += widgets
QT += network

TARGET = test_network
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    XTextEdit.cpp

HEADERS  += MainWindow.hpp \
    XTextEdit.hpp

FORMS    += MainWindow.ui

include($$PWD/../../OpenCVBase.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR

include($$PWD/../../qcustomplot_library/QCustomPlot.pri)
LIBS += -L$$THIS_PROJECT_DESTDIR -lqcustomplost_library

include($$PWD/../../cplusplus_base_library/cplusplus_base_library.pri)
LIBS += -L$$THIS_PROJECT_DESTDIR -lcplusplus_base_library

