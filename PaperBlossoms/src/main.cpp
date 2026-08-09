/*
 * *******************************************************************
 * This file is part of the Paper Blossoms application
 * (https://github.com/dashnine/PaperBlossoms).
 * Copyright (c) 2019 Kyle Hankins (dashnine)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The Legend of the Five Rings Roleplaying Game is the creation
 * and property of Fantasy Flight Games.
 * *******************************************************************
 */

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QLocale>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtCore/QTextCodec>
#else
    #include <QtCore5Compat/QTextCodec>
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QString defaultLocaleDB, defaultLocaleUI;
    defaultLocaleDB = defaultLocaleUI = QLocale::system().name();
    defaultLocaleDB.truncate(defaultLocaleDB.lastIndexOf('_'));
    defaultLocaleUI.truncate(defaultLocaleUI.lastIndexOf('_'));
    QString settingfile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/settings.ini";
    qDebug() << "Settings file: " << settingfile;
    QSettings settings(settingfile, QSettings::IniFormat);

    if(!settings.contains("localeDB")){
        //initialize the locale to the system locale (which would be the default anyways).
        settings.setValue("localeDB",defaultLocaleDB);
    }
    else{
        //if there is a setting, and it matches an allowed locale, use it.
        QString settinglocale = settings.value("localeDB").toString();
        if(settinglocale.toLower()=="en") defaultLocaleDB = "en";
        else if(settinglocale.toLower()=="es") defaultLocaleDB = "es";
        else if(settinglocale.toLower()=="fr") defaultLocaleDB = "fr";
        else if(settinglocale.toLower()=="de") defaultLocaleDB = "de";
        else if(settinglocale.toLower()=="test") defaultLocaleDB = "test";
        else defaultLocaleDB = "en";

    }

    if(!settings.contains("localeUI")){
        //initialize the locale to the system locale (which would be the default anyways).
        settings.setValue("localeUI",defaultLocaleUI);
    }
    else{
        //if there is a setting, and it matches an allowed locale, use it.
        QString settinglocale = settings.value("localeUI").toString();
        if(settinglocale.toLower()=="en") defaultLocaleUI = "en";
        else if(settinglocale.toLower()=="es") defaultLocaleUI = "es";
        else if(settinglocale.toLower()=="fr") defaultLocaleUI = "fr";
        else if(settinglocale.toLower()=="de") defaultLocaleUI = "de";
        else if(settinglocale.toLower()=="test") defaultLocaleUI = "test";
        else defaultLocaleUI = "en";

    }

    //support forcing a locale.  NOTE: hardcoded list to avoid injection risks
    //this would override the settings.
    if(argc>1){
        QString arg1(argv[1]);
        if(arg1.toLower()=="en") defaultLocaleDB = defaultLocaleUI = "en";
        else if(arg1.toLower()=="es") defaultLocaleDB = defaultLocaleUI = "es";
        else if(arg1.toLower()=="fr") defaultLocaleDB = defaultLocaleUI = "fr";
        else if(arg1.toLower()=="de") defaultLocaleDB = defaultLocaleUI = "de";
        else if(arg1.toLower()=="test") defaultLocaleDB = defaultLocaleUI = "test";
    }
    qDebug()<< "localeDB = " + defaultLocaleDB;
    qDebug()<< "localeUI = " + defaultLocaleUI;

    QTranslator tra;
    if(tra.load("paperblossoms_"+defaultLocaleUI,":/translations")){
        a.installTranslator(&tra);
        qDebug()<<"translation loaded";
    }
    else{
        qWarning() << "Translation not loaded.";
    }
    MainWindow w(defaultLocaleDB);
    w.show();

    return a.exec();
}
