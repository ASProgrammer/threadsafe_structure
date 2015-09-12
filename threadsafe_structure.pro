TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread -lboost_system -lboost_thread
LIBS += -pthread -lboost_system -lboost_thread

SOURCES += main.cpp \
    threadsafequeue.cpp \
    stack.cpp

HEADERS += \
    threadsafequeue.h \
    stack.h

