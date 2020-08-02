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
#include <QMessageBox>
#include "dataaccesslayer.h"

NewCharWizardPage1::NewCharWizardPage1(DataAccessLayer* dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage1)
{
    ui->setupUi(this);
    this->dal = dal;
    this->setTitle(tr("Part 1: Clan and Family"));

    //initialize models
    clanModel = new QStringListModel;
    familyModel = new QStringListModel;
    famRingModel = new QStringListModel;

    //initialize models - Path of Waves Support
    regionModel = new QStringListModel;
    upbringingModel = new QStringListModel;
    upbringingRingModel = new QStringListModel;
    upbringingSkillModel1 = new QStringListModel;
    upbringingSkillModel2 = new QStringListModel;



    //set models
    ui->nc1_clan_ComboBox->setModel(clanModel);
    ui->nc1_family_ComboBox->setModel(familyModel);
    ui->nc1_ring_ComboBox->setModel(famRingModel);

    ui->nc1_region_ComboBox->setModel(regionModel);
    ui->nc1_upbringing_ComboBox->setModel(upbringingModel);
    ui->nc1_upbringing_ring_ComboBox->setModel(upbringingRingModel);
    ui->nc1_skill1_ComboBox->setModel(upbringingSkillModel1);
    ui->nc1_skill2_ComboBox->setModel(upbringingSkillModel2);

    //set models - PoW
    //ui->nc

    //populate models from data
    clanModel->setStringList(dal->qsl_getclans());
    familyModel->setStringList(dal->qsl_getfamilies(ui->nc1_clan_ComboBox->currentText()));

    regionModel->setStringList(dal->qsl_getregions(ui->characterType_comboBox->currentText()));
    upbringingModel->setStringList(dal->qsl_getupbringings());




    registerField("currentClan", ui->nc1_clan_ComboBox, "currentText");
    registerField("currentFamily",ui->nc1_family_ComboBox, "currentText");
    registerField("familyRing", ui->nc1_ring_ComboBox, "currentText");
    registerField("currentClanIndex", ui->nc1_clan_ComboBox);
    registerField("currentFamilyIndex",ui->nc1_family_ComboBox);
    registerField("familyRingIndex", ui->nc1_ring_ComboBox);

    //PoW
    registerField("characterType", ui->characterType_comboBox,"currentText");
    registerField("currentRegion", ui->nc1_region_ComboBox,"currentText");
    registerField("currentUpbringing", ui->nc1_upbringing_ComboBox,"currentText");
    registerField("upbringingRing", ui->nc1_upbringing_ring_ComboBox,"currentText");
    registerField("upbringingSkill1", ui->nc1_skill1_ComboBox,"currentText");
    registerField("upbringingSkill2", ui->nc1_skill2_ComboBox,"currentText");

    setSamuraiVisibilty(true);

    regenSummary();
}

NewCharWizardPage1::~NewCharWizardPage1()
{
    delete ui;
}

void NewCharWizardPage1::setSamuraiVisibilty(bool isSamurai){
    //ui->->setVisible(isSamurai);
    ui->clanGroupBox->setVisible(isSamurai);
    ui->famGroupBox->setVisible(isSamurai);
    ui->regionGroupBox->setVisible(!isSamurai);
    ui->upbringingGroupBox->setVisible(!isSamurai);
}

bool NewCharWizardPage1::validatePage(){
    if(ui->characterType_comboBox->currentText() == "Samurai"){
        if (    ui->nc1_clan_ComboBox->currentText().isEmpty()      ||
                ui->nc1_family_ComboBox->currentText().isEmpty()    ||
                ui->nc1_ring_ComboBox->currentText().isEmpty()
                ) {
            QMessageBox msg;
            msg.setText("Error: please answer all questions to advance.");
            msg.exec();
                    return false;
        }
    }
    else {
        if(
                ui->nc1_region_ComboBox->currentText().isEmpty() ||
                ui->nc1_upbringing_ComboBox->currentText().isEmpty() ||
                ui->nc1_upbringing_ring_ComboBox->currentText().isEmpty() ||
                ui->nc1_skill1_ComboBox->currentText().isEmpty() ||
                ui->nc1_skill2_ComboBox->currentText().isEmpty() )
{
            QMessageBox msg;
            msg.setText("Error: please answer all questions to advance.");
            msg.exec();
                 return false;
        }
    }
    return true;
}

void NewCharWizardPage1::on_nc1_clan_ComboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Selecting Families for clan = " + arg1;
    //populate model from data
    familyModel->setStringList(dal->qsl_getfamilies(arg1));

    QString clanring = "+1 " + dal->qs_getclanring(arg1);
    QString clanskills = "";

    foreach (QString skill, dal->qsl_getclanskills(arg1)){
        clanskills += "+1 " + skill + "\n";
    }

    if(!arg1.isEmpty()){

    ui->nc1_clan_TextEdit->setText(dal->qs_getclandesc(arg1) + " " + dal->qs_getclanref(arg1)+"\n"+clanring+"\n"+clanskills);
    }
    else {
        ui->nc1_clan_TextEdit->setText("");
    }


    regenSummary();
}

void NewCharWizardPage1::on_nc1_family_ComboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Selecting Rings for family = " + arg1;
    famRingModel->setStringList(dal->qsl_getfamilyrings( arg1 ));
    QString famskills = "";

    foreach (QString skill, dal->qsl_getfamilyskills(arg1)){
        famskills += "+1 " + skill + "\n";
    }

    if(!arg1.isEmpty()){
        ui->nc1_family_TextEdit->setText(dal->qs_getfamilydesc(arg1) + " " + dal->qs_getfamilyref(arg1) +
                                         "\n"+famskills  );
    }
    else {
        ui->nc1_family_TextEdit->setText("");
    }
    regenSummary();
}

void NewCharWizardPage1::regenSummary(){
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

QMap<QString, int> NewCharWizardPage1::calcCurrentRings(){
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



    ///////////PoW
    ///
    ///

    //region
    ringmap[dal->qs_getregionring(field("currentRegion").toString())]++;
    //upbringing
    ringmap[field("upbringingRing").toString()]++;

    /////////////////


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

QMap<QString, int> NewCharWizardPage1::calcSkills(){


    QStringList skills;
    skills.append(dal->qsl_getclanskills(field("currentClan").toString()));
    skills.append(dal->qsl_getfamilyskills(field("currentFamily").toString()));


    skills.append(dal->qsl_getregionskills(field("currentRegion").toString()));
    skills.append(field("upbringingSkill1").toString());
    skills.append(field("upbringingSkill2").toString());



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
            heritage = dal->translate(field("ancestor1").toString());
        }
        else if (field("ancestor2checked").toBool()){
            ancestorIndex = field("ancestor2index").toInt()+1;
            heritage = dal->translate(field("ancestor2").toString());
        }

        if(    //core
               heritage ==  dal->translate("Wondrous Work") ||
               heritage ==  dal->translate("Dynasty Builder") ||
               heritage ==  dal->translate("Discovery") ||
               heritage ==  dal->translate("Ruthless Victor") ||
               heritage ==  dal->translate("Elevated for Service") ||
               //shadowlands
               heritage ==   dal->translate("Infamous Builder") ||
               heritage ==   dal->translate("Lost in the Darkness") ||
               heritage ==   dal->translate("Vengeance for the Fallen") ||
               heritage ==   dal->translate("Tewnty Goblin Thief") ||
               //Courts
               heritage ==   dal->translate("Dishonorable Cheat") ||
               heritage ==   dal->translate("Unforgivable Performance") ||
               heritage ==   dal->translate("A Little Too Close To Heaven")
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

void NewCharWizardPage1::on_nc1_ring_ComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();
}

void NewCharWizardPage1::on_characterType_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    setSamuraiVisibilty(ui->characterType_comboBox->currentText()=="Samurai");

    regionModel->setStringList(dal->qsl_getregions(ui->characterType_comboBox->currentText()));//adjust in case we swapped to gaijin or ronin


    ui->nc1_clan_ComboBox->setCurrentIndex(-1);
    ui->nc1_family_ComboBox->setCurrentIndex(-1);
    ui->nc1_ring_ComboBox->setCurrentIndex(-1);
    ui->nc1_upbringing_ComboBox->setCurrentIndex(-1);
    ui->nc1_region_ComboBox->setCurrentIndex(-1);
    ui->nc1_upbringing_ring_ComboBox->setCurrentIndex(-1);
    ui->nc1_skill1_ComboBox->setCurrentIndex(-1);
    ui->nc1_skill2_ComboBox->setCurrentIndex(-1);

    regenSummary();
}

void NewCharWizardPage1::on_nc1_region_ComboBox_currentIndexChanged(const QString &arg1)
{

    QString regionring = "+1 " + dal->qs_getregionring(arg1);
    QString regionskills = "";

    foreach (QString skill, dal->qsl_getregionskills(arg1)){
        regionskills += "+1 " + skill + "\n";
    }

    if(!arg1.isEmpty())
        ui->nc1_region_TextEdit->setText(dal->qs_getregiondesc(arg1) + " \n" + dal->qs_getregionref(arg1)+"\n"+regionring+"\n"+regionskills);
    else {
        ui->nc1_region_TextEdit->setText("");
    }


    regenSummary();
}

void NewCharWizardPage1::on_nc1_upbringing_ComboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Selecting Rings for upbringing = " + arg1;
    upbringingRingModel->setStringList(dal->qsl_getfamilyrings( arg1 ));
    upbringingSkillModel1->setStringList(dal->qsl_getupbringingskills1(arg1));
    upbringingSkillModel2->setStringList(dal->qsl_getupbringingskills2(arg1));

    QString famskills = "";

    foreach (QString skill, dal->qsl_getupbringingskills1(arg1)){
        famskills += "+1 " + skill + "\n";
    }
    foreach (QString skill, dal->qsl_getupbringingskills2(arg1)){
        famskills += "+1 " + skill + "\n";
    }

    if(!arg1.isEmpty()){
        ui->nc1_upbringing_TextEdit->setText(dal->qs_getupbringingdesc(arg1) + " \n" + dal->qs_getupbringingref(arg1) );
    }
    else{
        ui->nc1_upbringing_TextEdit->setText("");
    }
    regenSummary();
}

void NewCharWizardPage1::on_nc1_skill1_ComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();
}

void NewCharWizardPage1::on_nc1_skill2_ComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();
}

void NewCharWizardPage1::on_nc1_upbringing_ring_ComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();
}
