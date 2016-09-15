
QT -= core gui
QMAKE_CXXFLAGS += -ftemplate-depth=4100

TARGET = cplusplus_base_library
TEMPLATE = lib

DEFINES += CPLUSPLUS_BASE_LIBRARY_LIBRARY

SOURCES += Cplusplus_base_library.cpp \
    memory/source/Memory.cpp

HEADERS += $$PWD/Cplusplus_base_library.hpp \
        $$PWD/cplusplus_base_library_global.hpp \
    $$PWD/memory/Allocator.hpp \
    $$PWD/memory/MakeFunction.hpp \
    $$PWD/memory/MakeShared.hpp \
    $$PWD/memory/Memory.hpp \
    $$PWD/memory/MemoryStatic.hpp

include($$PWD/base_cplusplus_base_library.pri)

include($$PWD/../OpenCVBase.pri)
DESTDIR=$$THIS_PROJECT_DESTDIR

