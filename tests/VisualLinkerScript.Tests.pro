QT       += core testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++17

# The 'sources' tag is auto-generated from template
SOURCES += \
    DrcEngine\Rules\CEntryIsDefinedRuleTests.cpp

INCLUDEPATH += ../src
DEPENDPATH += ../src

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE += -O0

QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE -= -O3
QMAKE_CFLAGS_RELEASE += -O0