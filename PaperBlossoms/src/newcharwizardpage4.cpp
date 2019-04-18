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

#include "newcharwizardpage4.h"
#include "ui_newcharwizardpage4.h"
#include "QMessageBox"
#include <QDebug>

NewCharWizardPage4::NewCharWizardPage4(DataAccessLayer *dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage4)
{
    ui->setupUi(this);
    this->dal = dal;
    this->setTitle("Part 4: Strengths and Weaknesses");

    ui->nc4_q9_desc_label->setVisible(false);
    ui->nc4_q10_desc_label->setVisible(false);
    ui->nc4_q11_desc_label->setVisible(false);
    ui->nc4_q12_desc_label->setVisible(false);
    ui->nc4_q13_desc_label->setVisible(false);

    ui->nc4_q13_adv_comboBox->setVisible(false);
    ui->nc4_q13_disaadv_comboBox->setVisible(false);
    ui->nc4_q13_skill_comboBox->setVisible(false);

    registerField("q13DisadvChecked", ui->nc4_q13_disadv_radioButton);
    registerField("q13AdvChecked", ui->nc4_q13_adv_radioButton);
    registerField("q13skill", ui->nc4_q13_skill_comboBox, "currentText");
    registerField("q13Advantage",ui->nc4_q13_adv_comboBox, "currentText");
    registerField("q13Disadvantage",ui->nc4_q13_disaadv_comboBox, "currentText");


    registerField("distinction",    ui->nc4_q9_advdisadv_comboBox,  "currentText");
    registerField("adversity",      ui->nc4_q10_advdisadv_comboBox, "currentText");
    registerField("passion",        ui->nc4_q11_advdisadv_comboBox, "currentText");
    registerField("anxiety",        ui->nc4_q12_advdisadv_comboBox, "currentText");


    registerField("q9Text", ui->nc4_q9_lineEdit);
    registerField("q10Text", ui->nc4_q10_lineEdit);
    registerField("q11Text", ui->nc4_q11_lineEdit);
    registerField("q12Text", ui->nc4_q12_lineEdit);
    registerField("q13Text", ui->nc4_q13_lineEdit);
    regenSummary();

}

NewCharWizardPage4::~NewCharWizardPage4()
{
    delete ui;
}

bool NewCharWizardPage4::validatePage(){

    if(!ui->nc4_q13_adv_radioButton->isChecked() && !ui->nc4_q13_disadv_radioButton->isChecked()){
        QMessageBox msg;
        msg.setText("Error: choose an option for Question 13.");
        msg.exec();
        return false;
    }
    return true;
}

void NewCharWizardPage4::initializePage(){
    ui->nc4_q9_advdisadv_comboBox->clear();
    ui->nc4_q10_advdisadv_comboBox->clear();
    ui->nc4_q11_advdisadv_comboBox->clear();
    ui->nc4_q12_advdisadv_comboBox->clear();
    ui->nc4_q13_adv_comboBox->clear();
    ui->nc4_q13_disaadv_comboBox->clear();
    ui->nc4_q13_skill_comboBox->clear();
    ui->nc4_q9_advdisadv_comboBox->addItems(dal->qsl_getadvdisadv("Distinctions"));
    ui->nc4_q10_advdisadv_comboBox->addItems(dal->qsl_getadvdisadv("Adversities"));
    ui->nc4_q11_advdisadv_comboBox->addItems(dal->qsl_getadvdisadv("Passions"));
    ui->nc4_q12_advdisadv_comboBox->addItems(dal->qsl_getadvdisadv("Anxieties"));
    ui->nc4_q13_adv_comboBox->addItems(dal->qsl_getadv());
    ui->nc4_q13_disaadv_comboBox->addItems(dal->qsl_getdisadv());
    ui->nc4_q13_skill_comboBox->addItems(dal->qsl_getskills());
    regenSummary();

}

void NewCharWizardPage4::on_nc4_q13_adv_radioButton_toggled(const bool checked)
{
    ui->nc4_q13_adv_comboBox->setVisible(checked);
    ui->nc4_q13_disaadv_comboBox->setVisible(!checked);
    ui->nc4_q13_skill_comboBox->setVisible(!checked);
    regenSummary();
}

void NewCharWizardPage4::on_nc4_q13_disadv_radioButton_toggled(const bool checked)
{
    ui->nc4_q13_adv_comboBox->setVisible(!checked);
    ui->nc4_q13_disaadv_comboBox->setVisible(checked);
    ui->nc4_q13_skill_comboBox->setVisible(checked);
    regenSummary();
}

void NewCharWizardPage4::regenSummary(){
   QString skills = "";
   QString rings = "";

   const QMap<QString, int> ringmap = calcCurrentRings();

   QMapIterator<QString, int> i(ringmap);
   while (i.hasNext()) {
       i.next();
       if(!i.key().isEmpty()){
        rings+="  "+i.key()+": "+QString::number(i.value())+ "\n";
       }
   }

   const QMap<QString, int> skillmap = calcSkills();

   QMapIterator<QString, int> si(skillmap);
   while (si.hasNext()) {
       si.next();
       if(!si.key().isEmpty()){
        skills+="  "+si.key()+": "+QString::number(si.value())+ "\n";
       }
   }

    ui->summary_label->setText("Rings:\n"+rings+"\n\nSkills:\n"+skills);

}

QMap<QString, int> NewCharWizardPage4::calcCurrentRings(){
    QMap<QString, int> ringmap;
    QStringList ringlist = dal->qsl_getrings();
    foreach (const QString ring, ringlist) {
        ringmap[ring] = 1;
    }

    //NOW - CALCULATE EXISTING RINGS
    //clan
    ringmap[dal->qs_getclanring(field("currentClan").toString())]++;
    //family
    ringmap[field("familyRing").toString()]++;
    //school
    //QStringList schoolrings = dal->qsl_getschoolrings(field("currentSchool").toString());
    QStringList schoolrings = field("ringChoices").toString().split("|");
    schoolrings.removeAll("");

    foreach (const QString r, schoolrings){
        ringmap[r]++;
    }
    //standout
    ringmap[field("schoolSpecialRing").toString()]++;
    qDebug() << ringmap;
    return ringmap;
}

QMap<QString, int> NewCharWizardPage4::calcSkills(){


    QStringList skills;
    skills.append(dal->qsl_getclanskills(field("currentClan").toString()));
    skills.append(dal->qsl_getfamilyskills(field("currentFamily").toString()));
    skills.append(field("schoolSkills").toString().split("|"));
    skills.append(field("q7skill").toString());
    skills.append(field("q8skill").toString());
    if(field("q13DisadvChecked").toBool() == true){
        qDebug() << "Question 13 chose disadvantage.  Adding skill: " << field("q13skill").toString();
        skills.append(field("q13skill").toString());
    }
    skills.append(field("parentSkill").toString());
    //get q18 skill


    int ancestorIndex = -1;
    QString heritage = "";
    if(field("ancestor1checked").toBool()){
        ancestorIndex = field("ancestor1index").toInt()+1;
        heritage = field("ancestor1").toString();
    }
    else if (field("ancestor2checked").toBool()){
        ancestorIndex = field("ancestor2index").toInt()+1;
        heritage = field("ancestor2").toString();
    }

    if(    //core
           heritage == "Wondrous Work" ||
           heritage ==  "Dynasty Builder" ||
           heritage ==  "Discovery" ||
           heritage ==  "Ruthless Victor" ||
           heritage ==  "Elevated for Service" ||
           //shadowlands
           heritage ==   "Infamous Builder" ||
           heritage ==   "Lost in the Darkness" ||
           heritage ==   "Vengeance for the Fallen" ||
           heritage ==   "Tewnty Goblin Thief"
            ){
        skills.append(field("q18OtherEffects").toString());

    }

    skills.removeAll("");
    //initialize the skill map
    QMap<QString, int> skillmap;
    //skills start at 0.

    //clan
    foreach(const QString skill, skills){
        skillmap[skill]++;
    }

    return skillmap;
}

void NewCharWizardPage4::on_nc4_q13_skill_comboBox_currentIndexChanged(const QString &arg1)
{
    regenSummary();

}
