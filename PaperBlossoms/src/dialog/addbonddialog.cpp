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

#include "addbonddialog.h"
#include "ui_addbonddialog.h"

AddBondDialog::AddBondDialog(DatabaseDependency* deps, Character* character, QString type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBondDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/resources/sakura.png"));
    this->character = character;
    this->deps = deps;
    ui->passionwarning->setVisible(type=="Bonds");

    ui->traitComboBox->addItems(deps->bondsRepository->qsl_getbonds());
}

AddBondDialog::~AddBondDialog()
{
    delete ui;
}

QStringList AddBondDialog::getResult() const {
    QStringList resultlist;
    resultlist.append(deps->bondsRepository->qsl_getbond(ui->traitComboBox->currentText()));
    if(resultlist.count()>0) resultlist.insert(1,"1"); //a new bond is always rank 1
    return resultlist;
}
