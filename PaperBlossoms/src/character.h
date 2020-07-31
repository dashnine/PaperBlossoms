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

#ifndef CHARACTER_H
#define CHARACTER_H

#include <QMap>
#include <QStringList>
#include <QStandardItemModel>
#include <QList>
#include <QImage>

class Character
{
public:
    Character();
    ~Character();

    QString name;
    QStringList titles;

    QString clan;
    QString family;
    QString school;

    QString ninjo;
    QString giri;

    QMap<QString, int> baseskills;
    QMap<QString, int> skillranks;
    QMap<QString, int> baserings;
    QMap<QString, int> ringranks;

    int honor;
    int glory;
    int status;
    int koku;   //wealth
    int bu;     //wealth
    int zeni;   //wealth
    int rank;

    QStringList techniques;

    QStringList adv_disadv;

    QList<QStringList> abilities;
    QList<QStringList> equipment;
    QList<QStringList> bonds;

    QString heritage;
    QStringList advanceStack;

    QString notes;

    int totalXP;

    void clear();

    QImage portrait;
};

#endif // CHARACTER_H
