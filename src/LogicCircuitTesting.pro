TEMPLATE = app
CONFIG += c++11
QT += widgets
SOURCES += main.cpp \
    logicelement.cpp \
    circuit.cpp \
    circuittest.cpp \
    lexer.cpp \
    parser.cpp \
    codeeditor.cpp \
    highlighter.cpp \
    mainwindow.cpp \
    logicfunc.cpp \
    testbuilder.cpp \
    testwidget.cpp \
    tabbededitor.cpp \
    testtablemodel.cpp

HEADERS += \
    logicelement.h \
    circuit.h \
    input.h \
    component.h \
    circuittest.h \
    lexer.h \
    parseerror.h \
    parser.h \
    token.h \
    codeeditor.h \
    highlighter.h \
    mainwindow.h \
    parsestructures.h \
    logicfunc.h \
    testbuilder.h \
    testwidget.h \
    tabbededitor.h \
    testttablemodel.h
