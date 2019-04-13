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

#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include "dataaccesslayer.h"
#include "character.h"
#include <QStringListModel>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(DataAccessLayer *dal, Character *character, QString type, QWidget *parent = 0);
    ~AddItemDialog();

    QList<QStringList> getResult() const;
private slots:
    void on_itemtemplate_combobox_currentIndexChanged(const QString &arg1);

    void on_qual_add_pushButton_clicked();

    void on_qual_rem_pushButton_clicked();

    void on_pattern_add_pushButton_clicked();

private:
    Ui::AddItemDialog *ui;
    DataAccessLayer* dal;
    Character* character;
    QString type;
    QStringListModel qualities;
    void clearFields();
};

#endif // ADDITEMDIALOG_H
