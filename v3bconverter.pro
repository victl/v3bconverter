TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../ugv-share/config.cpp \
    ../ugv-share/defines.cpp

LIBS += `pkg-config opencv --cflags --libs` \
        -lglog \
       -lboost_system\
       -lboost_thread\
       -lboost_program_options

INCLUDEPATH += /usr/include/opencv

HEADERS += \
    ../ugv-share/config.h \
    ../ugv-share/defines.h
