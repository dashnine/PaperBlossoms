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
#include  "../dependency/databasedependency.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>

NewCharWizardPage3::NewCharWizardPage3(DatabaseDependency *deps, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage3)
{
    ui->setupUi(this);
    this->deps = deps;
    this->setTitle(tr("Part 3: Honor and Glory"));

    //Add radio buttons to buttongroup to set exclusivity properly
    q7group.addButton(ui->nc3_q7_pos_radioButton);
    q7group.addButton(ui->nc3_q7_neg_radioButton);
    q8group.addButton(ui->nc3_q8_pos_radioButton);
    q8group.addButton(ui->nc3_q8_mid_radioButton);
    q8group.addButton(ui->nc3_q8_neg_radioButton);

    ui->nc3_q7_comboBox->setEnabled(false);
    ui->nc3_q8_comboBox->setEnabled(false);
    ui->nc3_q8_item_comboBox->setEnabled(false);

    registerField("q7skill", ui->nc3_q7_comboBox, "currentText");
    registerField("q8skill", ui->nc3_q8_comboBox, "currentText");
    registerField("q8item", ui->nc3_q8_item_comboBox, "currentText");

    registerField("q7posradio_glory", ui->nc3_q7_pos_radioButton);
    registerField("q7negradio_skill", ui->nc3_q7_neg_radioButton);
    registerField("q8posradio_honor", ui->nc3_q8_pos_radioButton);
    registerField("q8midradio_skill", ui->nc3_q8_mid_radioButton);
    registerField("q8negradio_skill", ui->nc3_q8_neg_radioButton);

    registerField("q5Text",ui->nc3_q5_textEdit, "plainText");
    registerField("q6Text",ui->nc3_q6_textEdit, "plainText");
    registerField("q7Text",ui->nc3_q7_textEdit, "plainText");
    registerField("q8Text",ui->nc3_q8_textEdit, "plainText");
    regenSummary();

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
    if(!ui->nc3_q8_neg_radioButton->isChecked() && !ui->nc3_q8_pos_radioButton->isChecked()&& !ui->nc3_q8_mid_radioButton->isChecked()){
        QMessageBox msg;
        msg.setText("Error: choose an option for Question 8.");
        msg.exec();
        return false;
    }
        if(field("characterType").toString() == "Samurai" && ui->nc3_q8_mid_radioButton->isChecked()){
            QMessageBox msg;
            msg.setText("Error: choose an option for Question 8.");
            msg.exec();
            return false;
        }
    return true;
}

void NewCharWizardPage3::initializePage()
{

    ///////////////////////PoW: Set Ronin Questions if needed:
    //populate model
    if(field("characterType").toString() == "Samurai"){
        ui->q5_groupBox->setTitle("5. Who is your lord, and what is your duty to them?");
        ui->q6_groupBox->setTitle("6. What do you long for, and how might this impede your duty?");
        ui->q7_groupBox->setTitle("7. What is your relationship with your clan?");
        ui->q8_groupBox->setTitle("8. What do you think of Bushido?");

        ui->nc3_q8_item_comboBox->setVisible(false);
        ui->nc3_q8_mid_radioButton->setVisible(false);
        ui->nc3_q8_mid_radioButton->setChecked(false);
    }
    else{
        ui->q5_groupBox->setTitle("5. What is your past, and how does it affect you?");
        ui->q6_groupBox->setTitle("6. What do you long for, and how might your past impact your Ninjo?");
        ui->q7_groupBox->setTitle("7. What are you known for?");
        ui->q8_groupBox->setTitle("8. What do you think of Bushido?");

        ui->nc3_q8_item_comboBox->setVisible(true);
        ui->nc3_q8_mid_radioButton->setVisible(true);
    }

    ui->nc3_q8_item_comboBox->addItems(deps->inventoryRepository->qsl_getitemsunderrarity(5));

    ////////////////////////////////



    //TODO - CLEAN UP SKILL MANAGEMENT
    const QString clan = field("currentClan").toString();
    const QString family = field("currentFamily").toString();

    //qDebug() << field("techChoices").toString();
    QStringList skills;
    skills.append(field("schoolSkills").toString().split("|"));
    skills.append(deps->clansRepository->qsl_getclanskills(clan));
    skills.append(deps->familiesRepository->qsl_getfamilyskills(family));
    qDebug()<<skills;
    QStringList skillList = deps->skillsRepository->qsl_getskills();
    const QStringList q8skills = {deps->dal->translate("Commerce"),
                                  deps->dal->translate( "Labor"),
                                  deps->dal->translate( "Medicine"),
                                  deps->dal->translate( "Seafaring"),
                                  deps->dal->translate( "Skulduggery"),
                                  deps->dal->translate( "Survival")};
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
    ui->nc3_q8_item_comboBox->setCurrentIndex(-1);



    regenSummary();


}


void NewCharWizardPage3::on_nc3_q7_neg_radioButton_toggled(const bool checked)
{
    ui->nc3_q7_comboBox->setEnabled(checked);
    if(!checked) ui->nc3_q7_comboBox->setCurrentIndex(-1);
}

void NewCharWizardPage3::on_nc3_q8_neg_radioButton_toggled(const bool checked)
{
    ui->nc3_q8_comboBox->setEnabled(checked);
    if(!checked) ui->nc3_q8_comboBox->setCurrentIndex(-1);

}

//PoW
void NewCharWizardPage3::on_nc3_q8_mid_radioButton_toggled(bool checked)
{
    ui->nc3_q8_item_comboBox->setEnabled(checked);
    if(!checked) ui->nc3_q8_item_comboBox->setCurrentIndex(-1);
}
///////

void NewCharWizardPage3::on_nc3_q7_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();

}

void NewCharWizardPage3::on_nc3_q8_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();

}




void NewCharWizardPage3::regenSummary(){
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

QMap<QString, int> NewCharWizardPage3::calcCurrentRings(){
    QMap<QString, int> ringmap;
    QStringList ringlist = deps->ringsRepository->qsl_getrings();
    foreach (const QString ring, ringlist) {
        ringmap[ring] = 1;
    }

    //NOW - CALCULATE EXISTING RINGS
    //clan
    ringmap[deps->clansRepository->qs_getclanring(field("currentClan").toString())]++;
    //family
    ringmap[field("familyRing").toString()]++;



    ///////////PoW
    ///
    ///

    //region
    ringmap[deps->regionsRepository->qs_getregionring(field("currentRegion").toString())]++;
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

QMap<QString, int> NewCharWizardPage3::calcSkills(){


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
               heritage ==   deps->dal->translate("Guardian of Forbidden Knowledge")||
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



