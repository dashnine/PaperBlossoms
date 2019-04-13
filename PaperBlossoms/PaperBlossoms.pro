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
    src/aboutdialog.cpp \
    src/mainwindow.cpp \
    src/addadvancedialog.cpp \
    src/adddisadvdialog.cpp \
    src/additemdialog.cpp \
    src/addtitledialog.cpp \
    src/character.cpp \
    src/clicklabel.cpp \
    src/dataaccesslayer.cpp \
    src/dynamicchoicewidget.cpp \
    src/main.cpp \
    src/newcharacterwizard.cpp \
    src/newcharwizardpage1.cpp \
    src/newcharwizardpage2.cpp \
    src/newcharwizardpage3.cpp \
    src/newcharwizardpage4.cpp \
    src/newcharwizardpage5.cpp \
    src/newcharwizardpage6.cpp \
    src/newcharwizardpage7.cpp \
    src/pboutputdata.cpp \
    src/renderdialog.cpp \
    src/ringviewer.cpp

HEADERS += \
    src/aboutdialog.h \
    src/mainwindow.h \
    src/addadvancedialog.h \
    src/adddisadvdialog.h \
    src/additemdialog.h \
    src/addtitledialog.h \
    src/character.h \
    src/clicklabel.h \
    src/dataaccesslayer.h \
    src/dynamicchoicewidget.h \
    src/enums.h \
    src/newcharacterwizard.h \
    src/newcharwizardpage1.h \
    src/newcharwizardpage2.h \
    src/newcharwizardpage3.h \
    src/newcharwizardpage4.h \
    src/newcharwizardpage5.h \
    src/newcharwizardpage6.h \
    src/newcharwizardpage7.h \
    src/pboutputdata.h \
    src/renderdialog.h \
    src/ringviewer.h

FORMS += \
    ui/aboutdialog.ui \
    ui/addadvancedialog.ui \
    ui/adddisadvdialog.ui \
    ui/additemdialog.ui \
    ui/addtitledialog.ui \
    ui/dynamicchoicewidget.ui \
    ui/mainwindow.ui \
    ui/newcharwizardpage1.ui \
    ui/newcharwizardpage2.ui \
    ui/newcharwizardpage3.ui \
    ui/newcharwizardpage4.ui \
    ui/newcharwizardpage5.ui \
    ui/newcharwizardpage6.ui \
    ui/newcharwizardpage7.ui \
    ui/renderdialog.ui \
    ui/ringviewer.ui

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

#DISTFILES +=

TRANSLATIONS += paperblossoms_en.ts \
                paperblossoms_fr.ts \
                paperblossoms_es.ts

win32:RC_ICONS += resources/pink-sakura-01-hi.ico
macx: ICON =

#QMAKE_RESOURCE_FLAGS += -no-compress
