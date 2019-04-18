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

#ifndef NEWCHARWIZARDPAGE4_H
#define NEWCHARWIZARDPAGE4_H

#include <QWizardPage>
#include "dataaccesslayer.h"
namespace Ui {
class NewCharWizardPage4;
}

class NewCharWizardPage4 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NewCharWizardPage4(DataAccessLayer *dal, QWidget *parent = 0);
    ~NewCharWizardPage4();

private slots:
    void on_nc4_q13_adv_radioButton_toggled(const bool checked);

    void on_nc4_q13_disadv_radioButton_toggled(const bool checked);

    void on_nc4_q13_skill_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::NewCharWizardPage4 *ui;
    DataAccessLayer* dal;
    void initializePage();
    bool validatePage();
    void regenSummary();
    QMap<QString, int> calcCurrentRings();
    QMap<QString, int> calcSkills();
};

#endif // NEWCHARWIZARDPAGE4_H
