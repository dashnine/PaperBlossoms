QT += testlib
QT += core gui sql webenginewidgets widgets printsupport
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_testmain.cpp

RESOURCES += \
    ../PaperBlossoms/resources.qrc
