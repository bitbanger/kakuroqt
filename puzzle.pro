QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = puzzlewindow

SOURCES += main.cpp \
    PuzzleWindow.cpp \
    KakuroConfig.cpp \
    Cell.cpp

HEADERS  += \
    PuzzleWindow.h \
    KakuroConfig.h \
    Cell.h \
    Partitioner.h \
    Solver.h

CONFIG += c++11
