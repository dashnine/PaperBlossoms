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

#include "addtitledialog.h"
#include "ui_addtitledialog.h"

AddTitleDialog::AddTitleDialog(DataAccessLayer* dal, Character* character, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTitleDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/resources/pink-sakura-01-hi.png"));
    this->dal = dal;
    this->character = character;
    ui->title_combobox->addItems(dal->qsl_gettitles());
    title = "";
    ui->title_combobox->setCurrentIndex(-1);
}

AddTitleDialog::~AddTitleDialog()
{
    delete ui;
}

void AddTitleDialog::on_title_combobox_currentIndexChanged(const QString &arg1)
{

    title = arg1;

    ui->reference_label->setText(dal->qs_gettitleref(arg1));
    ui->xp_label->setText(dal->qs_gettitlexp(arg1));
    ui->ability_label->setText(dal->qs_gettitleability(arg1));
}

QString AddTitleDialog::getResult(){
    return title;
}
