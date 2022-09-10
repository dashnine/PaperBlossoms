#-------------------------------------------------
#
# Project created by QtCreator 2019-02-12T22:39:35
#
#-------------------------------------------------

QT       += core gui sql webenginewidgets widgets printsupport xml


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
    src/dependency/databasedependency.cpp \
    src/dependency/dependencybuilder.cpp \
    src/dialog/aboutdialog.cpp \
    src/dialog/addadvancedialog.cpp \
    src/dialog/addbonddialog.cpp \
    src/dialog/adddisadvdialog.cpp \
    src/dialog/additemdialog.cpp \
    src/dialog/addtitledialog.cpp \
    src/dialog/dblocalisationeditordialog.cpp \
    src/dialog/edituserdescriptionsdialog.cpp \
    src/dialog/renderdialog.cpp \
    src/mainwindow.cpp \
    src/character.cpp \
    src/clicklabel.cpp \
    src/repository/abstractrepository.cpp \
    src/repository/advantagesrepository.cpp \
    src/repository/bondsrepository.cpp \
    src/repository/clansrepository.cpp \
    src/repository/dataaccesslayer.cpp \
    src/dynamicchoicewidget.cpp \
    src/main.cpp \
    src/characterwizard/newcharacterwizard.cpp \
    src/characterwizard/newcharwizardpage1.cpp \
    src/characterwizard/newcharwizardpage2.cpp \
    src/characterwizard/newcharwizardpage3.cpp \
    src/characterwizard/newcharwizardpage4.cpp \
    src/characterwizard/newcharwizardpage5.cpp \
    src/characterwizard/newcharwizardpage6.cpp \
    src/characterwizard/newcharwizardpage7.cpp \
    src/pboutputdata.cpp \
    src/repository/familiesrepository.cpp \
    src/repository/hertiagesrepository.cpp \
    src/repository/inventoryrepository.cpp \
    src/repository/regionsrepository.cpp \
    src/repository/ringsrepository.cpp \
    src/repository/schoolsrepository.cpp \
    src/repository/skillsrepository.cpp \
    src/repository/techniquesrepository.cpp \
    src/repository/titlesrepository.cpp \
    src/repository/upbringingsrepository.cpp \
    src/ringviewer.cpp \
    src/tabs/advancementpage.cpp \
    src/tabs/backgroundpage.cpp \
    src/tabs/bondspage.cpp \
    src/tabs/characterdatapage.cpp \
    src/tabs/equipmentpage.cpp \
    src/tabs/personaltraitspage.cpp \
    src/tools/common.cpp \
    src/tools/developer.cpp \
    src/tools/export.cpp \
    src/tools/file.cpp \
    src/tools/import.cpp

HEADERS += \
    src/dependency/databasedependency.h \
    src/dependency/dependencybuilder.h \
    src/dialog/aboutdialog.h \
    src/dialog/addadvancedialog.h \
    src/dialog/addbonddialog.h \
    src/dialog/adddisadvdialog.h \
    src/dialog/additemdialog.h \
    src/dialog/addtitledialog.h \
    src/dialog/dblocalisationeditordialog.h \
    src/dialog/edituserdescriptionsdialog.h \
    src/dialog/renderdialog.h \
    src/mainwindow.h \
    src/character.h \
    src/clicklabel.h \
    src/repository/abstractrepository.h \
    src/repository/advantagesrepository.h \
    src/repository/bondsrepository.h \
    src/repository/clansrepository.h \
    src/repository/dataaccesslayer.h \
    src/dynamicchoicewidget.h \
    src/enums.h \
    src/characterwizard/newcharacterwizard.h \
    src/characterwizard/newcharwizardpage1.h \
    src/characterwizard/newcharwizardpage2.h \
    src/characterwizard/newcharwizardpage3.h \
    src/characterwizard/newcharwizardpage4.h \
    src/characterwizard/newcharwizardpage5.h \
    src/characterwizard/newcharwizardpage6.h \
    src/characterwizard/newcharwizardpage7.h \
    src/pboutputdata.h \
    src/repository/familiesRepository.h \
    src/repository/hertiagesrepository.h \
    src/repository/inventoryrepository.h \
    src/repository/regionsrepository.h \
    src/repository/ringsrepository.h \
    src/repository/schoolsrepository.h \
    src/repository/skillsrepository.h \
    src/repository/techniquesrepository.h \
    src/repository/titlesrepository.h \
    src/repository/upbringingsrepository.h \
    src/ringviewer.h

FORMS += \
    ui/aboutdialog.ui \
    ui/addadvancedialog.ui \
    ui/addbonddialog.ui \
    ui/adddisadvdialog.ui \
    ui/additemdialog.ui \
    ui/addtitledialog.ui \
    ui/dblocalisationeditordialog.ui \
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
    ui/ringviewer.ui \
    ui/edituserdescriptionsdialog.ui

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
                paperblossoms_es.ts \
                paperblossoms_de.ts

win32:RC_ICONS += resources/sakura.ico
macx: ICON = resources/sakura.icns

#QMAKE_RESOURCE_FLAGS += -no-compress
