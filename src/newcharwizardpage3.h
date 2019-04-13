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

#ifndef NEWCHARWIZARDPAGE3_H
#define NEWCHARWIZARDPAGE3_H

#include <QWizardPage>
#include "dataaccesslayer.h"
#include <QButtonGroup>

namespace Ui {
class NewCharWizardPage3;
}

class NewCharWizardPage3 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NewCharWizardPage3(DataAccessLayer *dal, QWidget *parent = 0);
    ~NewCharWizardPage3();

private slots:
    void on_nc3_q7_neg_radioButton_toggled(const bool checked);

    void on_nc3_q8_neg_radioButton_toggled(const bool checked);

private:
    Ui::NewCharWizardPage3 *ui;
    void initializePage();
    QButtonGroup q7group;
    QButtonGroup q8group;

    DataAccessLayer* dal;

    bool validatePage();
};

#endif // NEWCHARWIZARDPAGE3_H
