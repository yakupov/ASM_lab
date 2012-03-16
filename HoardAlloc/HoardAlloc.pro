#-------------------------------------------------
#
# Project created by QtCreator 2012-03-15T20:48:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = HoardAlloc
TEMPLATE = lib

DEFINES += HOARDALLOC_LIBRARY

SOURCES += hoardalloc.cpp \
    heap.cpp \
    superblock.cpp

HEADERS += \
    heap.h \
    superblock.h \
    blockheader.h \
    hardcorelinkedlist.h

QMAKE_CXXFLAGS += -std=c++0x
