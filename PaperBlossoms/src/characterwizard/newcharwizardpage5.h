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

#ifndef NEWCHARWIZARDPAGE5_H
#define NEWCHARWIZARDPAGE5_H
#include  "../dependency/databasedependency.h"

#include <QWizardPage>

namespace Ui {
class NewCharWizardPage5;
}

class NewCharWizardPage5 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NewCharWizardPage5(DatabaseDependency *deps, QWidget *parent = 0);
    ~NewCharWizardPage5();

private:
    Ui::NewCharWizardPage5 *ui;
    DatabaseDependency* deps;
    void initializePage();
    void regenSummary();
    QMap<QString, int> calcCurrentRings();
    QMap<QString, int> calcSkills();
};

#endif // NEWCHARWIZARDPAGE5_H
