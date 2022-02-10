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
#include  "../dependency/databasedependency.h"

NewCharWizardPage1::NewCharWizardPage1(DatabaseDependency* deps, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage1)
{
    ui->setupUi(this);
    this->deps = deps;
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
    upbringingSkillModel3 = new QStringListModel;



    //set models
    ui->nc1_clan_ComboBox->setModel(clanModel);
    ui->nc1_family_ComboBox->setModel(familyModel);
    ui->nc1_ring_ComboBox->setModel(famRingModel);

    ui->nc1_region_ComboBox->setModel(regionModel);
    ui->nc1_upbringing_ComboBox->setModel(upbringingModel);
    ui->nc1_upbringing_ring_ComboBox->setModel(upbringingRingModel);
    ui->nc1_skill1_ComboBox->setModel(upbringingSkillModel1);
    ui->nc1_skill2_ComboBox->setModel(upbringingSkillModel2);
    ui->nc1_skill3_ComboBox->setModel(upbringingSkillModel3);

    //third skill is never visible at first
    ui->nc1_skill3_ComboBox->setVisible(false);

    //set models - PoW
    //ui->nc

    //populate models from data
    clanModel->setStringList(deps->clansRepository->qsl_getclans());
    familyModel->setStringList(deps->familiesRepository->qsl_getfamilies(ui->nc1_clan_ComboBox->currentText()));

    regionModel->setStringList(deps->regionsRepository->qsl_getregions(ui->characterType_comboBox->currentText()));
    upbringingModel->setStringList(deps->upbringingsRepository->qsl_getupbringings());




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
    registerField("upbringingSkill3", ui->nc1_skill3_ComboBox,"currentText");

    setSamuraiVisibilty(true);


    ui->characterType_comboBox->setCurrentIndex(-1);
    ui->characterType_comboBox->setCurrentIndex(0);
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
        if(ui->nc1_skill3_ComboBox->currentText().isEmpty() && deps->upbringingsRepository->qsl_getupbringingskillsbyset(ui->nc1_upbringing_ComboBox->currentText(),2).count()>0){
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
    familyModel->setStringList(deps->familiesRepository->qsl_getfamilies(arg1));

    QString clanring = "+1 " + deps->clansRepository->qs_getclanring(arg1);
    QString clanskills = "";

    foreach (QString skill, deps->clansRepository->qsl_getclanskills(arg1)){
        clanskills += "+1 " + skill + "\n";
    }

    if(!arg1.isEmpty()){

        ui->nc1_clan_TextEdit->setText(deps->clansRepository->qs_getclandesc(arg1) + " " + deps->clansRepository->qs_getclanref(arg1)+"\n"+clanring+"\n"+clanskills);
    }
    else {
        ui->nc1_clan_TextEdit->setText("");
        ui->nc1_family_ComboBox->setCurrentIndex(-1);
        ui->nc1_family_TextEdit->setText("");
    }


    regenSummary();
}

void NewCharWizardPage1::on_nc1_family_ComboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Selecting Rings for family = " + arg1;
    famRingModel->setStringList(deps->familiesRepository->qsl_getfamilyrings( arg1 ));
    QString famskills = "";

    foreach (QString skill, deps->familiesRepository->qsl_getfamilyskills(arg1)){
        famskills += "+1 " + skill + "\n";
    }

    if(!arg1.isEmpty()){
        ui->nc1_family_TextEdit->setText(deps->familiesRepository->qs_getfamilydesc(arg1) + " " + deps->familiesRepository->qs_getfamilyref(arg1) +
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
    QStringList ringlist = deps->regionsRepository->qsl_getrings();

    foreach (const QString ring, ringlist) {
        ringmap[ring] = 1;
    }


    if(field("characterType").toString()=="Samurai"){
        //NOW - CALCULATE EXISTING RINGS
        //clan
        ringmap[deps->regionsRepository->qs_getclanring(field("currentClan").toString())]++;
        //family
        ringmap[field("familyRing").toString()]++;
    }


    ///////////PoW
    ///
    ///
    else{
        //region
        ringmap[deps->regionsRepository->qs_getregionring(field("currentRegion").toString())]++;
        //upbringing
        ringmap[field("upbringingRing").toString()]++;
    }
    /////////////////

/*  //For now, let's not calc school since that comes later...
    //school
    //QStringList schoolrings = dal->qsl_getschoolrings(field("currentSchool").toString());
    QStringList schoolrings = field("ringChoices").toString().split("|");
    schoolrings.removeAll("");

    foreach (const QString r, schoolrings){
        ringmap[r]++;
    }
    //standout
    ringmap[field("schoolSpecialRing").toString()]++;

*/
    qDebug() << ringmap;
    return ringmap;
}

QMap<QString, int> NewCharWizardPage1::calcSkills(){


    QStringList skills;
    skills.append(deps->clansRepository->qsl_getclanskills(field("currentClan").toString()));
    skills.append(deps->familiesRepository->qsl_getfamilyskills(field("currentFamily").toString()));


    skills.append(deps->regionsRepository->qsl_getregionskills(field("currentRegion").toString()));
    skills.append(field("upbringingSkill1").toString());
    skills.append(field("upbringingSkill2").toString());
    skills.append(field("upbringingSkill3").toString());



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
            heritage = deps->dal->translate(field("ancestor1").toString());
        }
        else if (field("ancestor2checked").toBool()){
            ancestorIndex = field("ancestor2index").toInt()+1;
            heritage = deps->dal->translate(field("ancestor2").toString());
        }

        if(    //core
               heritage ==  deps->dal->translate("Wondrous Work") ||
               heritage ==  deps->dal->translate("Dynasty Builder") ||
               heritage ==  deps->dal->translate("Discovery") ||
               heritage ==  deps->dal->translate("Ruthless Victor") ||
               heritage ==  deps->dal->translate("Elevated for Service") ||
               //shadowlands
               heritage ==   deps->dal->translate("Infamous Builder") ||
               heritage ==   deps->dal->translate("Lost in the Darkness") ||
               heritage ==   deps->dal->translate("Vengeance for the Fallen") ||
               heritage ==   deps->dal->translate("Tewnty Goblin Thief") ||
               //Courts
               heritage ==   deps->dal->translate("Dishonorable Cheat") ||
               heritage ==   deps->dal->translate("Unforgivable Performance") ||
               heritage ==   deps->dal->translate("A Little Too Close To Heaven")||
               //Celestial realms
               heritage ==   deps->dal->translate("Great Treatise")||
               heritage ==   deps->dal->translate("Guardian of Forbidden Knowledge")

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
    setSamuraiVisibilty(ui->characterType_comboBox->currentText()=="Samurai");
    QString type = arg1;
    if(type == "Peasant") type = "Rōnin"; //for regions, Peasants are treated as Ronin

    regionModel->setStringList(deps->regionsRepository->qsl_getregions(type));//adjust in case we swapped to gaijin or ronin


    ui->nc1_clan_ComboBox->setCurrentIndex(-1);
    ui->nc1_family_ComboBox->setCurrentIndex(-1);
    ui->nc1_ring_ComboBox->setCurrentIndex(-1);
    ui->nc1_upbringing_ComboBox->setCurrentIndex(-1);
    ui->nc1_region_ComboBox->setCurrentIndex(-1);
    ui->nc1_upbringing_ring_ComboBox->setCurrentIndex(-1);
    ui->nc1_skill1_ComboBox->setCurrentIndex(-1);
    ui->nc1_skill2_ComboBox->setCurrentIndex(-1);
    ui->nc1_skill3_ComboBox->setCurrentIndex(-1);

    regenSummary();
}

void NewCharWizardPage1::on_nc1_region_ComboBox_currentIndexChanged(const QString &arg1)
{

    QString regionring = "+1 " + deps->regionsRepository->qs_getregionring(arg1);
    QString regionskills = "";

    foreach (QString skill, deps->regionsRepository->qsl_getregionskills(arg1)){
        regionskills += "+1 " + skill + "\n";
    }

    if(!arg1.isEmpty())
        ui->nc1_region_TextEdit->setText(deps->regionsRepository->qs_getregiondesc(arg1) + " \n" + deps->regionsRepository->qs_getregionref(arg1)+"\n"+regionring+"\n"+regionskills);
    else {
        ui->nc1_region_TextEdit->setText("");
    }


    regenSummary();
}

void NewCharWizardPage1::on_nc1_upbringing_ComboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Selecting Rings for upbringing = " + arg1;
    upbringingRingModel->setStringList(deps->upbringingsRepository->qsl_getupbringingrings( arg1 ));
    upbringingSkillModel1->setStringList(deps->upbringingsRepository->qsl_getupbringingskillsbyset(arg1,0));
    upbringingSkillModel2->setStringList(deps->upbringingsRepository->qsl_getupbringingskillsbyset(arg1,1));
    upbringingSkillModel3->setStringList(deps->upbringingsRepository->qsl_getupbringingskillsbyset(arg1,2));

    if(!arg1.isEmpty()){
        ui->nc1_upbringing_TextEdit->setText(deps->upbringingsRepository->qs_getupbringingdesc(arg1) + " \n" + deps->upbringingsRepository->qs_getupbringingref(arg1) );
    }
    else{
        ui->nc1_upbringing_TextEdit->setText("");
    }

    ui->nc1_skill3_ComboBox->setVisible(deps->upbringingsRepository->qsl_getupbringingskillsbyset(arg1,2).count()>0);

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

void NewCharWizardPage1::on_nc1_skill3_ComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();
}

void NewCharWizardPage1::on_nc1_upbringing_ring_ComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();
}
