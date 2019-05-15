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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QString defaultLocale;

    //support forcing a locale.  NOTE: hardvoded list to avoid injection risks
    if(argc>1){
        QString arg1(argv[1]);
        if(arg1.toLower()=="en") defaultLocale = "en";
        else if(arg1.toLower()=="es") defaultLocale = "es";
        else if(arg1.toLower()=="fr") defaultLocale = "fr";
    }
    else{
        defaultLocale = QLocale::system().name();
        defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
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
    MainWindow w;
    w.show();

    return a.exec();
}
