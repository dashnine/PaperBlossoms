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

#ifndef NEWCHARWIZARDPAGE7_H
#define NEWCHARWIZARDPAGE7_H

#include <QWizardPage>
#include "dataaccesslayer.h"
#include "character.h"

namespace Ui {
class NewCharWizardPage7;
}

class NewCharWizardPage7 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NewCharWizardPage7(DataAccessLayer *dal, Character* character, QWidget *parent = 0);
    ~NewCharWizardPage7();

private slots:
    void ringsChanged(QString dynamicRingString);
    void on_nc7_repRing_1_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc7_repRing_2_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc7_repSkill_1_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc7_repSkill_2_comboBox_currentIndexChanged(const QString &arg1);

    void on_nc7_repSkill_3_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::NewCharWizardPage7 *ui;
    DataAccessLayer* dal;
    Character* character;
    void initializePage();
    QMap<QString, int> calcSkills();
    QMap<QString, int> calcRings();
    bool validatePage();
    int ring_overflow;
    int skill_overflow;
    void setVis();
    QList<QStringList> populateItemFields(QString name, QString type, QString cust_qual_1="", QString cust_qual_2="");
};

#endif // NEWCHARWIZARDPAGE7_H
