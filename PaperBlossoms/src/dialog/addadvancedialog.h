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

#ifndef ADDADVANCEDIALOG_H
#define ADDADVANCEDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "../dependency/databasedependency.h"
#include "../character.h"
#include <QStandardItemModel>

namespace Ui {
class AddAdvanceDialog;
}

class AddAdvanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAdvanceDialog(DatabaseDependency* deps, Character *character, QString sel = "", QString option = "", QWidget *parent = 0);
    ~AddAdvanceDialog();

    QString getResult() const;
private slots:
    void on_advtype_currentIndexChanged(const QString &arg1);

    void on_advchooser_combobox_currentIndexChanged(const QString &arg1);

    void on_curriculum_radioButton_clicked();

    void on_title_radioButton_clicked();

    void on_detailTableView_clicked(const QModelIndex &index);

    void on_free_radioButton_toggled(const bool checked);

    void on_halfxp_checkBox_toggled(bool checked);

    void on_restrictioncheckBox_toggled(bool checked);

private:
    Ui::AddAdvanceDialog *ui;
    DatabaseDependency* deps;
    Character* character;

    bool removerestrictions;

    void validatePage();
    QStandardItemModel techModel;
    QSortFilterProxyModel proxyModel;
    QSqlQueryModel curriculumModel;
    void populateTechModel();
    void addTechRow(QStringList tech);
};

#endif // ADDADVANCEDIALOG_H
