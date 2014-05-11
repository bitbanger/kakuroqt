QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kakuro

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

ICON = kakuro.icns

CONFIG -= app_bundle

CONFIG += c++11
