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

#ifndef NEWCHARACTERWIZARD_H
#define NEWCHARACTERWIZARD_H

#include <QWidget>
#include <QWizard>
#include  "../dependency/databasedependency.h"
#include  "../dependency/databasedependency.h"
#include "../character.h"
#include <QComboBox>

class NewCharacterWizard : public QWizard
{
    Q_OBJECT
public:
    explicit NewCharacterWizard(DatabaseDependency* deps, QWizard *parent = nullptr);

    Character getCharacter();
private:
   QList<QComboBox*> techBoxes; //link to technique boxes, since they're dynamic.
   Character character;

signals:

public slots:
};

#endif // NEWCHARACTERWIZARD_H
