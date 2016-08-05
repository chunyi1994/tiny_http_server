TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
LIBS += -levent
LIBS += -lpthread
SOURCES += main.cpp \
    tcpserver.cc \
    httpserver.cc \
    httpparser.cc

HEADERS += \
    utils.h \
    tcpserver.h \
    connection.h \
    buffer.h \
    httpserver.h \
    httpparser.h

