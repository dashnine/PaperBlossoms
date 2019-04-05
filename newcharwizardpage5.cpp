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

#include "newcharwizardpage5.h"
#include "ui_newcharwizardpage5.h"

NewCharWizardPage5::NewCharWizardPage5(DataAccessLayer *dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage5)
{
    this->setTitle("Part 5: Personality and Behavior");
    ui->setupUi(this);
    this->dal = dal;

    registerField("q16ItemIndex*",ui->nc5_q16_item_comboBox);
    registerField("q16Item",ui->nc5_q16_item_comboBox,"currentText");
    registerField("q14Text", ui->nc5_q14_textEdit, "plainText");
    registerField("q15Text", ui->nc5_q15_textEdit, "plainText");
    registerField("q16Text", ui->nc5_q16_textEdit, "plainText");
}

NewCharWizardPage5::~NewCharWizardPage5()
{
    delete ui;
}

void NewCharWizardPage5::initializePage()
{
    ui->nc5_q16_item_comboBox->addItems(dal->qsl_getitemsunderrarity(7));
    ui->nc5_q16_item_comboBox->setCurrentIndex(-1);

}
