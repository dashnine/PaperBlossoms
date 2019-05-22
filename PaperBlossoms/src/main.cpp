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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QString defaultLocale;
    defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
    QSettings settings("Paper Blossoms", "Paper Blossoms");
    if(!settings.contains("locale")){
        //initialize the locale to the system locale (which would be the default anyways).
        settings.setValue("locale",defaultLocale);
    }
    else{
        //if there is a setting, and it matches an allowed locale, use it.
        QString settinglocale = settings.value("locale").toString();
        if(settinglocale.toLower()=="en") defaultLocale = "en";
        else if(settinglocale.toLower()=="es") defaultLocale = "es";
        else if(settinglocale.toLower()=="fr") defaultLocale = "fr";
        else if(settinglocale.toLower()=="de") defaultLocale = "de";
        else if(settinglocale.toLower()=="pl") defaultLocale = "pl";
        else if(settinglocale.toLower()=="test") defaultLocale = "test";
        else defaultLocale = "en";

    }

    //support forcing a locale.  NOTE: hardcoded list to avoid injection risks
    //this would override the settings.
    if(argc>1){
        QString arg1(argv[1]);
        if(arg1.toLower()=="en") defaultLocale = "en";
        else if(arg1.toLower()=="es") defaultLocale = "es";
        else if(arg1.toLower()=="fr") defaultLocale = "fr";
        else if(arg1.toLower()=="de") defaultLocale = "de";
        else if(arg1.toLower()=="pl") defaultLocale = "pl";
        else if(arg1.toLower()=="test") defaultLocale = "test";
    }
    qDebug()<< "locale = " + defaultLocale;

    QTranslator tra;
    if(tra.load("paperblossoms_"+defaultLocale,":/translations")){
        a.installTranslator(&tra);
        qDebug()<<"translation loaded";
    }
    else{
        qWarning() << "Translation not loaded.";
    }
    MainWindow w(defaultLocale);
    w.show();

    return a.exec();
}
