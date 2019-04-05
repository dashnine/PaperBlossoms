#-------------------------------------------------
#
# Project created by QtCreator 2019-02-12T22:39:35
#
#-------------------------------------------------

QT       += core gui sql webenginewidgets widgets printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PaperBlossoms
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    newcharacterwizard.cpp \
    newcharwizardpage1.cpp \
    dataaccesslayer.cpp \
    newcharwizardpage2.cpp \
    newcharwizardpage3.cpp \
    dynamicchoicewidget.cpp \
    newcharwizardpage4.cpp \
    newcharwizardpage5.cpp \
    newcharwizardpage6.cpp \
    newcharwizardpage7.cpp \
    character.cpp \
    ringviewer.cpp \
    addadvancedialog.cpp \
    addtitledialog.cpp \
    additemdialog.cpp \
    clicklabel.cpp \
    adddisadvdialog.cpp \
    renderdialog.cpp \
    pboutputdata.cpp \
    aboutdialog.cpp

HEADERS += \
        mainwindow.h \
    newcharacterwizard.h \
    newcharwizardpage1.h \
    dataaccesslayer.h \
    newcharwizardpage2.h \
    newcharwizardpage3.h \
    dynamicchoicewidget.h \
    newcharwizardpage4.h \
    newcharwizardpage5.h \
    newcharwizardpage6.h \
    newcharwizardpage7.h \
    character.h \
    ringviewer.h \
    addadvancedialog.h \
    addtitledialog.h \
    additemdialog.h \
    enums.h \
    clicklabel.h \
    adddisadvdialog.h \
    renderdialog.h \
    pboutputdata.h \
    aboutdialog.h

FORMS += \
        mainwindow.ui \
    newcharwizardpage1.ui \
    newcharwizardpage2.ui \
    newcharwizardpage3.ui \
    dynamicchoicewidget.ui \
    newcharwizardpage4.ui \
    newcharwizardpage5.ui \
    newcharwizardpage6.ui \
    newcharwizardpage7.ui \
    ringviewer.ui \
    addadvancedialog.ui \
    addtitledialog.ui \
    additemdialog.ui \
    adddisadvdialog.ui \
    renderdialog.ui \
    aboutdialog.ui

# Copy the DB file into the build whenever build is run.
# using shell_path() to correct path depending on platform
# escaping quotes and backslashes for file paths
# This excellent little gem comes from https://stackoverflow.com/a/36438513
#copydata.commands = $(COPY_FILE) \"$$shell_path($$PWD\\data\\testdb.db)\" \"$$shell_path($$OUT_PWD)\"
#copydata.commands = $(COPY_FILE) \"$$shell_path($$PWD\\data\\paperblossoms.db)\" \"$$shell_path($$OUT_PWD)\"
#first.depends = $(first) copydata
#export(first.depends)
#export(copydata.commands)
#QMAKE_EXTRA_TARGETS += first copydata

RESOURCES += \
    resources.qrc

DISTFILES +=

win32:RC_ICONS += resources/pink-sakura-01-hi.ico
macx: ICON = resources/pink-sakura-01-hi.ico
