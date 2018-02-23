TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lboost_system \
        -pthread

SOURCES += main.cpp \
    reliablebroadcast.cpp \
    message.cpp \
    node.cpp \
    externalmessage.cpp \
    internalmessage.cpp \
    echomessage.cpp \
    sendmessage.cpp \
    readymessage.cpp \
    socketcontroller.cpp \
    session.cpp

HEADERS += \
    reliablebroadcast.h \
    message.h \
    node.h \
    externalmessage.h \
    internalmessage.h \
    echomessage.h \
    sendmessage.h \
    readymessage.h \
    socketcontroller.h \
    session.h \
    threadsafequeue.h
