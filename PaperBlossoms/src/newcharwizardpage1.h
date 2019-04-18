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

#ifndef NEWCHARWIZARDPAGE1_H
#define NEWCHARWIZARDPAGE1_H

#include <QWizardPage>
#include <QStringListModel>
#include "dataaccesslayer.h"

namespace Ui {
class NewCharWizardPage1;
}

class NewCharWizardPage1 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NewCharWizardPage1(DataAccessLayer* dal, QWidget *parent = 0);
    ~NewCharWizardPage1();
    QStringListModel* clanModel;
    QStringListModel* familyModel;
    QStringListModel* famRingModel;


private slots:
    void on_nc1_clan_ComboBox_currentIndexChanged(const QString &arg1);

    void on_nc1_family_ComboBox_currentIndexChanged(const QString &arg1);

    void on_nc1_ring_ComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::NewCharWizardPage1 *ui;
    DataAccessLayer* dal;
    void regenSummary();
    QMap<QString, int> calcCurrentRings();
    QMap<QString, int> calcSkills();
};

#endif // NEWCHARWIZARDPAGE1_H
