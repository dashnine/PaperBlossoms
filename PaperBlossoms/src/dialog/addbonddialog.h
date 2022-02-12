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

#ifndef ADDBONDDIALOG_H
#define ADDBONDDIALOG_H

#include <QDialog>
#include "../dependency/databasedependency.h"
#include "../character.h"

namespace Ui {
class AddBondDialog;
}

class AddBondDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBondDialog(DatabaseDependency* deps, Character *character, QString type, QWidget *parent = 0);
    ~AddBondDialog();

    QStringList getResult() const;
private:
    Ui::AddBondDialog *ui;
    DatabaseDependency* deps;
    Character* character;
};

#endif // ADDDBONDDIALOG_H
