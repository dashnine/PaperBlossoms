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
#include <QDebug>

NewCharWizardPage5::NewCharWizardPage5(DataAccessLayer *dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage5)
{
    this->setTitle(tr("Part 5: Personality and Behavior"));
    ui->setupUi(this);
    this->dal = dal;

    registerField("q16ItemIndex*",ui->nc5_q16_item_comboBox);
    registerField("q16Item",ui->nc5_q16_item_comboBox,"currentText");
    registerField("q14Text", ui->nc5_q14_textEdit, "plainText");
    registerField("q15Text", ui->nc5_q15_textEdit, "plainText");
    registerField("q16Text", ui->nc5_q16_textEdit, "plainText");
    registerField("q14Item", ui->nc5_q14_item_comboBox,"currentText");
    regenSummary();
}

NewCharWizardPage5::~NewCharWizardPage5()
{
    delete ui;
}

void NewCharWizardPage5::initializePage()
{
    ui->nc5_q16_item_comboBox->addItems(dal->qsl_getitemsunderrarity(7));
    ui->nc5_q16_item_comboBox->setCurrentIndex(-1);

    ////////PoW
    ui->nc5_q14_item_comboBox->addItems(dal->qsl_getitemsunderrarity(5));
    ui->nc5_q14_item_comboBox->setCurrentIndex(-1);

    ////////PoW
    if(field("characterType").toString() == "Samurai"){
        ui->q14_groupBox->setTitle("14. What do people notice furst upon encountering you? (Choose a distinctive style/detail.)");
        ui->q15_groupBox->setTitle("15. How do you react to stressful situations?");
        ui->q16_groupBox->setTitle("16. What are your preexisting relationships with other clans, families, organizations, and traditions?");

        ui->nc5_q14_item_comboBox->setVisible(false);
        ui->q14_warninglabel->setVisible(false);
        ui->q14_itemlabel->setVisible(false);

    }
    else{
        ui->q14_groupBox->setTitle("14. What is your character's most prized possession?");
        ui->q15_groupBox->setTitle("15. How do you react to stressful situations?");
        ui->q16_groupBox->setTitle("16. What are your relationships to your family, the clans, peasants, and others?");
        ui->nc5_q14_item_comboBox->setVisible(true);
        ui->q14_warninglabel->setVisible(true);
        ui->q14_itemlabel->setVisible(true);
    }

    regenSummary();

}


////////////////TODO - Unify this across all wiz pages


void NewCharWizardPage5::regenSummary(){
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

QMap<QString, int> NewCharWizardPage5::calcCurrentRings(){
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

QMap<QString, int> NewCharWizardPage5::calcSkills(){


    QStringList skills;
    skills.append(dal->qsl_getclanskills(field("currentClan").toString()));
    skills.append(dal->qsl_getfamilyskills(field("currentFamily").toString()));


    skills.append(dal->qsl_getregionskills(field("currentRegion").toString()));
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





