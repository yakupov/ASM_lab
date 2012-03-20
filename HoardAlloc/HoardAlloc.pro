#-------------------------------------------------
#
# Project created by QtCreator 2012-03-15T20:48:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = HoardAlloc
TEMPLATE = lib

DEFINES += HOARDALLOC_LIBRARY

SOURCES += \
    src/HoardAlloc.cpp \
    src/Heap.cpp \
    src/SuperBlock.cpp

HEADERS += \
    src/Heap.h \
    src/SuperBlock.h \
    src/BlockHeader.h \
    src/InternalAlloc.h

QMAKE_CXXFLAGS += -std=c++0x
