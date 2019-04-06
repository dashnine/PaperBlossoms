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

#include "newcharwizardpage6.h"
#include "ui_newcharwizardpage6.h"
#include <ctime>
#include <QDebug>

NewCharWizardPage6::NewCharWizardPage6(DataAccessLayer *dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage6)
{

    this->setTitle("Part 6: Ancestry and Family");
    ui->setupUi(this);
    this->dal = dal;
    curAncestorBox = NULL;

    srand(time(NULL));


    registerField("parentSkill", ui->nc6_q17_comboBox,"currentText");
    registerField("ancestor1", ui->nc6_q18_ancestor1_comboBox,"currentText");
    registerField("ancestor2", ui->nc6_q18_ancestor2_comboBox,"currentText");
    registerField("ancestor1index", ui->nc6_q18_ancestor1_comboBox);
    registerField("ancestor2index", ui->nc6_q18_ancestor2_comboBox);
    registerField("ancestor1checked",ui->nc6_q18_ancestor1_radioButton);
    registerField("ancestor2checked",ui->nc6_q18_ancestor2_radioButton);
    registerField("q18OtherEffects",ui->nc6_q18_otherComboBox,"currentText");
    registerField("q18OtherEffectsIndex*",ui->nc6_q18_otherComboBox);
    registerField("q18SecondaryChoice",ui->nc6_q18_secondaryChoice_comboBox,"currentText");
    registerField("q18Spec1",ui->nc6_q18_special1_comboBox,"currentText");
    registerField("q18Spec2",ui->nc6_q18_special2_comboBox,"currentText");

    registerField("q17Text", ui->nc6_q17_lineEdit);
    registerField("personalName",ui->nc6_q19_personalName_lineEdit);
}

NewCharWizardPage6::~NewCharWizardPage6()
{
    delete ui;
}

void NewCharWizardPage6::initializePage()
{
    /////////////////SKILLBOX - Q17
    //TODO - CLEAN UP SKILL MANAGEMENT
    QString clan = field("currentClan").toString();
    QString family = field("currentFamily").toString();

    //qDebug() << field("techChoices").toString();
    QStringList skills;
    skills.append(field("schoolSkills").toString().split("|"));
    skills.append(dal->qsl_getclanskills(clan));
    skills.append(dal->qsl_getfamilyskills(family));
    skills.append(field("q7skill").toString());
    skills.append(field("q8skill").toString());
    if(field("q13DisadvChecked").toBool() == true){
        qDebug() << "Question 13 chose disadvantage.  Adding skill: " << field("q13skill").toString();
        skills.append(field("q13skill").toString());
    }
    QStringList skillList = dal->qsl_getskills();

    foreach (QString string, skills) {
        skillList.removeAll(string);
    }
    ui->nc6_q17_comboBox->clear();
    ui->nc6_q17_comboBox->addItems(skillList);

    ui->nc6_q18_ancestor1_comboBox->clear();
    ui->nc6_q18_ancestor2_comboBox->clear();
    ui->nc6_q18_ancestor1_comboBox->addItems(dal->qsl_getancestors());
    ui->nc6_q18_ancestor2_comboBox->addItems(dal->qsl_getancestors());
    ui->nc6_q18_ancestor1_comboBox->setCurrentIndex(-1);
    ui->nc6_q18_ancestor2_comboBox->setCurrentIndex(-1);

   buildq18UI();

}

void NewCharWizardPage6::on_nc6_q18_ancestor1_rollButton_clicked()
{
   ui->nc6_q18_ancestor1_comboBox->setCurrentIndex(rand()%10);
}

void NewCharWizardPage6::on_nc6_q18_ancestor2_rollButton_clicked()
{
   ui->nc6_q18_ancestor2_comboBox->setCurrentIndex(rand()%10);

}

void NewCharWizardPage6::on_nc6_q18_ancestor1_comboBox_currentIndexChanged(const QString &arg1)
{
   if(arg1.isEmpty()) {
       ui->nc6_q18_ancestor1_modLabel->setText("");
       return;
   }
   QStringList mods = dal->qsl_getancestormods(arg1);
   QString modstr = "";
   if(mods.at(0).toInt() != 0){
       modstr+= "H:"+mods.at(0) + " ";
   }
   if(mods.at(1).toInt() != 0){
       modstr+= "G:"+mods.at(1)+ " ";
   }
   if(mods.at(2).toInt() != 0){
       modstr+= "S:"+mods.at(2);
   }


   ui->nc6_q18_ancestor1_modLabel->setText(modstr);

    if(ui->nc6_q18_ancestor1_radioButton->isChecked()){
        doPopulateEffects(); //refresh the effects if you changed this while focusing on this ancestor.
    }
}

void NewCharWizardPage6::on_nc6_q18_ancestor2_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1.isEmpty()) {
        ui->nc6_q18_ancestor2_modLabel->setText("");
        return;
    }
    QStringList mods = dal->qsl_getancestormods(arg1);
    QString modstr = "";
    if(mods.at(0).toInt() != 0){
        modstr+= "H:"+mods.at(0) + " ";
    }
    if(mods.at(1).toInt() != 0){
        modstr+= "G:"+mods.at(1)+ " ";
    }
    if(mods.at(2).toInt() != 0){
        modstr+= "S:"+mods.at(2);
    }


    ui->nc6_q18_ancestor2_modLabel->setText(modstr);

    if(ui->nc6_q18_ancestor2_radioButton->isChecked()){
        doPopulateEffects(); //refresh the effects if you changed this while focusing on this ancestor.
    }
}

void NewCharWizardPage6::on_nc6_q18_ancestor1_radioButton_toggled(bool checked)
{
    //TODO: clean this up
    if(ui->nc6_q18_ancestor1_radioButton->isChecked())
        curAncestorBox = ui->nc6_q18_ancestor1_comboBox;
    else if(ui->nc6_q18_ancestor2_radioButton->isChecked()){
        curAncestorBox = ui->nc6_q18_ancestor2_comboBox;
    }
    else{
        curAncestorBox = NULL;
    }
    doPopulateEffects();
}


void NewCharWizardPage6::on_nc6_q18_ancestor2_radioButton_toggled(bool checked)
{
    if(ui->nc6_q18_ancestor1_radioButton->isChecked())
        curAncestorBox = ui->nc6_q18_ancestor1_comboBox;
    else if(ui->nc6_q18_ancestor2_radioButton->isChecked()){
        curAncestorBox = ui->nc6_q18_ancestor2_comboBox;
    }
    else{
        curAncestorBox = NULL;
    }
    doPopulateEffects();
}

void NewCharWizardPage6::doPopulateEffects(){
    if(curAncestorBox == NULL) return; //TODO: error handling here?
    ui->nc6_q18_otherComboBox->clear();
    QString ancestor = curAncestorBox->currentText();


    ui->nc6_q18_otherComboBox->addItems(dal->qsl_getancestorseffects(ancestor));
    ui->nc6_q18_otherComboBox->setCurrentIndex(-1);
   buildq18UI();

}

void NewCharWizardPage6::buildq18UI(){ //this is all still in initializePage
    QString curAncestor;
    if(curAncestorBox == NULL) curAncestor = ""; //TODO: error handling here?
    else  curAncestor = curAncestorBox->currentText();
    if(curAncestor == ""){
        //not picked yet.  Set all to false.
        ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
        ui->nc6_q18_secondaryChoice_label->setVisible(false);
        ui->nc6_q18_otherLabel->setVisible(false);
        ui->nc6_q18_otherComboBox->setVisible(false);
        ui->nc6_q18_otherrollButton->setVisible(false);
        ui->nc6_q18_special1_comboBox->setVisible(false);
        ui->nc6_q18_special2_comboBox->setVisible(false);
        ui->nc6_q18_specialInstruction_label->setVisible(false);
        ui->nc6_q18_specialInstruction_label->setText("");
        ui->nc6_q18_spec1_label->setVisible(false);
        ui->nc6_q18_spec2_label->setVisible(false);
        ui->nc6_q18_spec1_label->setText("");
        ui->nc6_q18_spec2_label->setText("");

    }
    else if(curAncestor == "Famous Deed" || curAncestor == "Glorious Sacrifice"){
        //Item Selection
        ui->nc6_q18_secondaryChoice_comboBox->setVisible(true);
        ui->nc6_q18_secondaryChoice_label->setVisible(true);
        ui->nc6_q18_otherLabel->setVisible(true);
        ui->nc6_q18_otherComboBox->setVisible(true);
        ui->nc6_q18_otherrollButton->setVisible(true);
        ui->nc6_q18_special1_comboBox->setVisible(true);
        ui->nc6_q18_special2_comboBox->setVisible(true);
        ui->nc6_q18_specialInstruction_label->setVisible(true);
        ui->nc6_q18_specialInstruction_label->setText("Choose an item quality. The GM chooses another item quality.");
        ui->nc6_q18_spec1_label->setVisible(true);
        ui->nc6_q18_spec2_label->setVisible(true);
        ui->nc6_q18_spec1_label->setText("Player-chosen Quality:");
        ui->nc6_q18_spec2_label->setText("GM-chosen Quality:");
    }
    else if (curAncestor == "Stolen Knowledge"){
        //Technique
        ui->nc6_q18_secondaryChoice_comboBox->setVisible(true);
        ui->nc6_q18_secondaryChoice_label->setVisible(true);
        ui->nc6_q18_otherLabel->setVisible(true);
        ui->nc6_q18_otherComboBox->setVisible(true);
        ui->nc6_q18_otherrollButton->setVisible(true);
        ui->nc6_q18_special1_comboBox->setVisible(false);
        ui->nc6_q18_special2_comboBox->setVisible(false);
        ui->nc6_q18_specialInstruction_label->setVisible(false);
        ui->nc6_q18_specialInstruction_label->setText("");
        ui->nc6_q18_spec1_label->setVisible(false);
        ui->nc6_q18_spec2_label->setVisible(false);
        ui->nc6_q18_spec1_label->setText("");
        ui->nc6_q18_spec2_label->setText("");

    }
    else if (curAncestor == "Imperial Heritage"){
        //Advantage - Blessed Lineage
        ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
        ui->nc6_q18_secondaryChoice_label->setVisible(false);
        ui->nc6_q18_otherLabel->setVisible(true);
        ui->nc6_q18_otherComboBox->setVisible(true);
        ui->nc6_q18_otherrollButton->setVisible(false);
        ui->nc6_q18_special1_comboBox->setVisible(false);
        ui->nc6_q18_special2_comboBox->setVisible(false);
        ui->nc6_q18_specialInstruction_label->setVisible(false);
        ui->nc6_q18_specialInstruction_label->setText("");
        ui->nc6_q18_spec1_label->setVisible(false);
        ui->nc6_q18_spec2_label->setVisible(false);
        ui->nc6_q18_spec1_label->setText("");
        ui->nc6_q18_spec2_label->setText("");
    }
    else if (curAncestor == "Unusual Name Origin"){
        ui->nc6_q18_otherLabel->setVisible(true);
        ui->nc6_q18_otherComboBox->setVisible(true);
        ui->nc6_q18_otherrollButton->setVisible(false);
        if(ui->nc6_q18_otherComboBox->currentText() == ""){
            //unselected
            ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
            ui->nc6_q18_secondaryChoice_label->setVisible(false);
            ui->nc6_q18_special1_comboBox->setVisible(false);
            ui->nc6_q18_special2_comboBox->setVisible(false);
            ui->nc6_q18_specialInstruction_label->setVisible(false);
            ui->nc6_q18_specialInstruction_label->setText("");
            ui->nc6_q18_spec1_label->setVisible(false);
            ui->nc6_q18_spec2_label->setVisible(false);
            ui->nc6_q18_spec1_label->setText("");
            ui->nc6_q18_spec2_label->setText("");
        }
        if(ui->nc6_q18_otherComboBox->currentText() == "Ring Exchange"){
            ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
            ui->nc6_q18_secondaryChoice_label->setVisible(false);
            ui->nc6_q18_special1_comboBox->setVisible(true);
            ui->nc6_q18_special2_comboBox->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setText("Choose a ring to raise, and a ring to lower.");
            ui->nc6_q18_spec1_label->setVisible(true);
            ui->nc6_q18_spec2_label->setVisible(true);
            ui->nc6_q18_spec1_label->setText("Raise:");
            ui->nc6_q18_spec2_label->setText("Lower:");
        }
        else{ //equipment
            ui->nc6_q18_secondaryChoice_comboBox->setVisible(true);
            ui->nc6_q18_secondaryChoice_label->setVisible(true);

            ui->nc6_q18_special1_comboBox->setVisible(false);
            ui->nc6_q18_special2_comboBox->setVisible(false);
            ui->nc6_q18_specialInstruction_label->setVisible(false);
            ui->nc6_q18_specialInstruction_label->setText("");
            ui->nc6_q18_spec1_label->setVisible(false);
            ui->nc6_q18_spec2_label->setVisible(false);
            ui->nc6_q18_spec1_label->setText("");
            ui->nc6_q18_spec2_label->setText("");

        }
    }
    else {
        //skill
        ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
        ui->nc6_q18_secondaryChoice_label->setVisible(false);
        ui->nc6_q18_otherLabel->setVisible(true);
        ui->nc6_q18_otherComboBox->setVisible(true);
        ui->nc6_q18_otherrollButton->setVisible(true);
        ui->nc6_q18_special1_comboBox->setVisible(false);
        ui->nc6_q18_special2_comboBox->setVisible(false);
        ui->nc6_q18_specialInstruction_label->setVisible(false);
        ui->nc6_q18_spec1_label->setVisible(false);
        ui->nc6_q18_spec2_label->setVisible(false);
        ui->nc6_q18_spec1_label->setText("");
        ui->nc6_q18_spec2_label->setText("");
    }
}

void NewCharWizardPage6::on_nc6_q18_otherComboBox_currentIndexChanged(const QString &arg1)
{
    if(curAncestorBox == NULL) return; //TODO: error handling here?
    int heritageRow = curAncestorBox->currentIndex()+1;
    ui->nc6_q18_secondaryChoice_comboBox->clear();
    ui->nc6_q18_special1_comboBox->clear();
    ui->nc6_q18_special2_comboBox->clear();
    QString effectresult = arg1;

    if(effectresult == ""){
        //nothing selected
    }
    switch(heritageRow){
    case 1:
    case 2:
        //heritage 1-2//////////////////////////
        if (effectresult == "weapon"){
            ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Weapon"));
        }
        else if (effectresult == "set of armor"){
            ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Armor"));

        }
        else if (effectresult == "another item"){
            ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Item"));

        }
        else if (effectresult == "horse or other animal"){
            ui->nc6_q18_secondaryChoice_comboBox->addItem("Horse");
            ui->nc6_q18_secondaryChoice_comboBox->addItem("Animal");

        }
        else if (effectresult == "boat or estate"){
            ui->nc6_q18_secondaryChoice_comboBox->addItem("Estate");
            ui->nc6_q18_secondaryChoice_comboBox->addItem("Boat");

        }
        ui->nc6_q18_special1_comboBox->addItems(dal->qsl_getqualities());
        ui->nc6_q18_special2_comboBox->addItems(dal->qsl_getqualities());

        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        /////////heritage 3-7 ///////////////////////////
        //else if (skillist.contains effectresult ){
        //  addSKill(effectResult);
        //}
        break;
    case 8:
        /// heritage 8
        // in tech?
        if(effectresult == "Mahō or Ninjutsu")
            ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getmahoninjutsu(1));
        else
            ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_gettechbytyperank(effectresult,1));
        break;
    case 9:
        // heritage 9
        if (effectresult == "Blessed Lineage"){
            //add blessed lineage -- no secondary pick.  Set in data?
        }
        break;
    case 10:
        ////////////////// Heritage 10
        if (effectresult == "Item (Rank 6 or lower)"){
            //do item adds
            ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsunderrarity(6));
        }
        else{ //rings
            QMap<QString, int> ringmap = calcCurrentRings();

            QMapIterator<QString, int> i(ringmap);
            while (i.hasNext()) {
                i.next();
                if(i.value() < 3)
                   ui->nc6_q18_special1_comboBox->addItem (i.key());
                if (i.value() > 1)
                    ui->nc6_q18_special2_comboBox->addItem (i.key());
            }

        }
        break;
    }
    buildq18UI();
}

QMap<QString, int> NewCharWizardPage6::calcCurrentRings(){
    QMap<QString, int> ringmap;
    QStringList ringlist = dal->qsl_getrings();
    foreach (QString ring, ringlist) {
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

    foreach (QString r, schoolrings){
        ringmap[r]++;
    }
    //standout
    ringmap[field("schoolSpecialRing").toString()]++;
    qDebug() << ringmap;
    return ringmap;
}

void NewCharWizardPage6::on_nc6_q18_otherrollButton_clicked()
{
   if (curAncestorBox->currentText() == "") return;
   QStringList rangestrings = dal->qsl_getheritageranges(curAncestorBox->currentText());
   int roll = rand()%10+1;
   int index = 0;
   foreach (QString str, rangestrings) {
       qDebug() << "Rangestring = " << str;
       qDebug() << "Roll = " << roll;
       if(roll >= str.split(", ")[0].toInt() && roll <= str.split(", ")[1].toInt() ) {// min == [0], max == [1]
            ui->nc6_q18_otherComboBox->setCurrentIndex(index);
            return;
       }
       else{
           ++index;
       }
   }

}
