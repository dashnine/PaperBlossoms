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

#include "newcharwizardpage3.h"
#include "ui_newcharwizardpage3.h"
#include "dataaccesslayer.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>

NewCharWizardPage3::NewCharWizardPage3(DataAccessLayer *dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage3)
{
    ui->setupUi(this);
    this->dal = dal;
    this->setTitle("Part 3: Honor and Glory");

    //Add radio buttons to buttongroup to set exclusivity properly
    q7group.addButton(ui->nc3_q7_pos_radioButton);
    q7group.addButton(ui->nc3_q7_neg_radioButton);
    q8group.addButton(ui->nc3_q8_pos_radioButton);
    q8group.addButton(ui->nc3_q8_neg_radioButton);

    ui->nc3_q7_comboBox->setEnabled(false);
    ui->nc3_q8_comboBox->setEnabled(false);

    registerField("q7skill", ui->nc3_q7_comboBox, "currentText");
    registerField("q8skill", ui->nc3_q8_comboBox, "currentText");

    registerField("q7posradio_glory", ui->nc3_q7_pos_radioButton);
    registerField("q7negradio_skill", ui->nc3_q7_neg_radioButton);
    registerField("q8posradio_honor", ui->nc3_q8_pos_radioButton);
    registerField("q8negradio_skill", ui->nc3_q8_neg_radioButton);

    registerField("q5Text",ui->nc3_q5_textEdit, "plainText");
    registerField("q6Text",ui->nc3_q6_textEdit, "plainText");
    registerField("q7Text",ui->nc3_q7_textEdit, "plainText");
    registerField("q8Text",ui->nc3_q8_textEdit, "plainText");
}

NewCharWizardPage3::~NewCharWizardPage3()
{
    delete ui;
}

bool NewCharWizardPage3::validatePage(){

    if(!ui->nc3_q7_neg_radioButton->isChecked() && !ui->nc3_q7_pos_radioButton->isChecked()){
        QMessageBox msg;
        msg.setText("Error: choose an option for Question 7.");
        msg.exec();
        return false;
    }
    if(!ui->nc3_q8_neg_radioButton->isChecked() && !ui->nc3_q8_pos_radioButton->isChecked()){
        QMessageBox msg;
        msg.setText("Error: choose an option for Question 8.");
        msg.exec();
        return false;
    }
    return true;
}

void NewCharWizardPage3::initializePage()
{
    //TODO - CLEAN UP SKILL MANAGEMENT
    QString clan = field("currentClan").toString();
    QString family = field("currentFamily").toString();

    //qDebug() << field("techChoices").toString();
    QStringList skills;
    skills.append(field("schoolSkills").toString().split("|"));
    skills.append(dal->qsl_getclanskills(clan));
    skills.append(dal->qsl_getfamilyskills(family));
    qDebug()<<skills;
    QStringList skillList = dal->qsl_getskills();
    QStringList q8skills = {"Commerce", "Labor", "Medicine", "Seafaring", "Skulduggery", "Survival"};
    ui->nc3_q7_comboBox->clear();
    ui->nc3_q8_comboBox->clear();
    foreach (QString string, skills) {
        skillList.removeAll(string);
    }


    //ui->nc3_q7_neg_radioButton->setChecked(false);
    //ui->nc3_q7_pos_radioButton->setChecked(false);
    //ui->nc3_q8_neg_radioButton->setChecked(false);
    //ui->nc3_q8_pos_radioButton->setChecked(false);
    ui->nc3_q7_comboBox->addItems(skillList);
    ui->nc3_q8_comboBox->addItems(q8skills);
    ui->nc3_q7_comboBox->setCurrentIndex(-1);
    ui->nc3_q8_comboBox->setCurrentIndex(-1);


}


void NewCharWizardPage3::on_nc3_q7_neg_radioButton_toggled(bool checked)
{
    ui->nc3_q7_comboBox->setEnabled(checked);
    if(!checked) ui->nc3_q7_comboBox->setCurrentIndex(-1);
}

void NewCharWizardPage3::on_nc3_q8_neg_radioButton_toggled(bool checked)
{
    ui->nc3_q8_comboBox->setEnabled(checked);
    if(!checked) ui->nc3_q8_comboBox->setCurrentIndex(-1);

}
