TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        graph.cpp \
        main.cpp

HEADERS += \
    dijkstra.h \
    graph.h \
    heap.h \
    matching_preparation.h \
    pupil.h \
    student.h
