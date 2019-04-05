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

#ifndef PBOUTPUTDATA_H
#define PBOUTPUTDATA_H

#include <QObject>
#include <QImage>
#include <QStringList>

class PBOutputData : public QObject
{
    Q_OBJECT
public:
    explicit PBOutputData(QObject *parent = nullptr);

    QString name;
    QStringList titles;

    QString clan;
    QString family;
    QString school;

    QString ninjo;
    QString giri;


    QList<QStringList> skills;
    QImage rings;

    QString honor;
    QString glory;
    QString status;
    QString koku;   //wealth
    QString bu;     //wealth
    QString zeni;   //wealth

    QString focus;
    QString vigilance;
    QString endurance;
    QString composure;

    //QString rank;
    //QString curricXP;
    QString curricStatus;
    QString titleStatus;
    QList<QStringList> curriculum;
    QList<QStringList> curTitle;

    QList<QStringList> techniques;

    QList<QStringList> distinctions;
    QList<QStringList> passions;
    QList<QStringList> adversities;
    QList<QStringList> anxieties;

    QList<QStringList> weapons;
    QList<QStringList> armor;
    QList<QStringList> personaleffects;

    QList<QStringList> abilities;

    QString heritage;
    QStringList advanceStack;

    QString notes;

    QImage portrait;

signals:

public slots:
};

#endif // PBOUTPUTDATA_H
