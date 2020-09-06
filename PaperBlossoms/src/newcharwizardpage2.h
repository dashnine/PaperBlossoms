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

#ifndef NEWCHARWIZARDPAGE2_H
#define NEWCHARWIZARDPAGE2_H

#include <QWizardPage>
#include "dataaccesslayer.h"
#include <QStringListModel>
#include <QFrame>
#include <QVBoxLayout>
#include <QList>
#include <QComboBox>

namespace Ui {
class NewCharWizardPage2;
}

class NewCharWizardPage2 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NewCharWizardPage2(DataAccessLayer *dal, QWidget *parent = 0);
    ~NewCharWizardPage2();
    QStringListModel* schoolModel;
    QStringListModel* skillOptModel;
    QStringListModel* skillSelModel;
    QStringListModel* ringModel;

private slots:
    void on_nc2_unrestrictedSchool_checkBox_toggled(const bool checked);
    void on_nc2_school_ComboBox_currentIndexChanged(const QString &arg1);
    void on_nc2_skillAdd_pushButton_clicked();
    void on_nc2_skillRem_pushButton_clicked();



    void equipSelectionChanged(const QString newText);
    void schoolRingSelectionChanged(const QString newText);
    void on_nc2_kitsune_comboBox_currentTextChanged(const QString &arg1);

    void on_nc2_skillOpt_listview_doubleClicked(const QModelIndex &index);

    void on_nc2_skillSel_listview_doubleClicked(const QModelIndex &index);


    void on_nc2_schoolSpecialtRing_ComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::NewCharWizardPage2 *ui;
    DataAccessLayer* dal;
    void initializePage();
    bool validatePage();
    bool settingupequip;
    void handleSpecCases(const QString speccase);
    void doEquip(const QString school);
    void doAddSkill();
    void doRemSkill();
    QMap<QString, int> calcSkills();
    QMap<QString, int> calcCurrentRings();
    void regenSummary();
};

#endif // NEWCHARWIZARDPAGE2_H
