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

#include "newcharwizardpage1.h"
#include "ui_newcharwizardpage1.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include "dataaccesslayer.h"

NewCharWizardPage1::NewCharWizardPage1(DataAccessLayer* dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage1)
{
    ui->setupUi(this);
    this->dal = dal;
    this->setTitle("Part 1: Clan and Family");

    //initialize models
    clanModel = new QStringListModel;
    familyModel = new QStringListModel;
    famRingModel = new QStringListModel;

    //set models
    ui->nc1_clan_ComboBox->setModel(clanModel);
    ui->nc1_family_ComboBox->setModel(familyModel);
    ui->nc1_ring_ComboBox->setModel(famRingModel);

    //populate models from data
    clanModel->setStringList(dal->qsl_getclans());
    familyModel->setStringList(dal->qsl_getfamilies(ui->nc1_clan_ComboBox->currentText()));

    registerField("currentClan", ui->nc1_clan_ComboBox, "currentText");
    registerField("currentFamily",ui->nc1_family_ComboBox, "currentText");
    registerField("familyRing", ui->nc1_ring_ComboBox, "currentText");
    registerField("currentClanIndex*", ui->nc1_clan_ComboBox);
    registerField("currentFamilyIndex*",ui->nc1_family_ComboBox);
    registerField("familyRingIndex*", ui->nc1_ring_ComboBox);


    //ui->nc1_clan_TextEdit->setVisible(false);
    //ui->nc1_family_TextEdit->setVisible(false);
}

NewCharWizardPage1::~NewCharWizardPage1()
{
    delete ui;
}

void NewCharWizardPage1::on_nc1_clan_ComboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Selecting Families for clan = " + arg1;
    //populate model from data
    familyModel->setStringList(dal->qsl_getfamilies(arg1));
    ui->nc1_clan_TextEdit->setText(dal->qs_getclandesc(arg1) + " " + dal->qs_getclanref(arg1));
}

void NewCharWizardPage1::on_nc1_family_ComboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Selecting Rings for family = " + arg1;
    famRingModel->setStringList(dal->qsl_getfamilyrings( arg1 ));
    ui->nc1_family_TextEdit->setText(dal->qs_getfamilydesc(arg1) + " " + dal->qs_getfamilyref(arg1));
}
