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

#include "adddisadvdialog.h"
#include "ui_adddisadvdialog.h"

AddDisAdvDialog::AddDisAdvDialog(DataAccessLayer* dal, Character* character, QString type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDisAdvDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/resources/sakura.png"));
    this->character = character;
    this->dal = dal;

    ui->traitComboBox->addItems(dal->qsl_getadvdisadv(type));
}

AddDisAdvDialog::~AddDisAdvDialog()
{
    delete ui;
}

QString AddDisAdvDialog::getResult() const {
    return ui->traitComboBox->currentText();
}
