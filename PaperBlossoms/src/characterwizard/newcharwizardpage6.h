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

#ifndef NEWCHARWIZARDPAGE6_H
#define NEWCHARWIZARDPAGE6_H

#include <QWizardPage>
#include  "../dependency/databasedependency.h"
#include <QComboBox>

namespace Ui {
class NewCharWizardPage6;
}

class NewCharWizardPage6 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NewCharWizardPage6(DatabaseDependency *deps, QWidget *parent = 0);
    ~NewCharWizardPage6();

private slots:
    void on_nc6_q18_ancestor1_rollButton_clicked();

    void on_nc6_q18_ancestor2_rollButton_clicked();

    void on_nc6_q18_ancestor1_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc6_q18_ancestor2_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc6_q18_ancestor1_radioButton_toggled(bool checked);
    void on_nc6_q18_ancestor2_radioButton_toggled(bool checked);

    void on_nc6_q18_otherComboBox_currentIndexChanged(const QString &arg1);

    void on_nc6_q18_otherrollButton_clicked();

    void on_heritagetable_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc6_q18_special1_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc6_q18_special2_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc6_q17_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::NewCharWizardPage6 *ui;
    DatabaseDependency* deps;
    void initializePage();
    void doPopulateEffects();
    void buildq18UI();
    QComboBox* curAncestorBox;
    QMap<QString, int> calcCurrentRings();
    void regenSummary();
    QMap<QString, int> calcSkills();
    QMap<QString, int> calcSumRings();
};

#endif // NEWCHARWIZARDPAGE6_H
