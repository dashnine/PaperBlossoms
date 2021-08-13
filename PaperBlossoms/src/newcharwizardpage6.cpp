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

    this->setTitle(tr("Part 6: Ancestry and Family"));
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
    registerField("q18OtherEffectsIndex",ui->nc6_q18_otherComboBox);
    registerField("q18SecondaryChoice",ui->nc6_q18_secondaryChoice_comboBox,"currentText");
    registerField("q18Spec1",ui->nc6_q18_special1_comboBox,"currentText");
    registerField("q18Spec2",ui->nc6_q18_special2_comboBox,"currentText");


    registerField("q17Text", ui->nc6_q17_lineEdit);
    registerField("personalName",ui->nc6_q19_personalName_lineEdit);

    registerField("q18SourceTable",ui->heritagetable_comboBox,"currentText");

    //POW
    registerField("q17roninBond",ui->nc6_bond_comboBox,"currentText");
    registerField("q17roninText",ui->nc6_q17r_lineEdit);

}

NewCharWizardPage6::~NewCharWizardPage6()
{
    delete ui;
}

void NewCharWizardPage6::initializePage()
{
    ui->heritagetable_comboBox->clear();
    ui->heritagetable_comboBox->addItem("Core");
    ui->heritagetable_comboBox->addItem("SL");
    ui->heritagetable_comboBox->addItem("CoS");
    ui->heritagetable_comboBox->addItem("CR");
    ui->heritagetable_comboBox->addItem("FoV");
    ui->heritagetable_comboBox->setCurrentIndex(0);

    /////////////////SKILLBOX - Q17
    //TODO - CLEAN UP SKILL MANAGEMENT
    const QString clan = field("currentClan").toString();
    const QString family = field("currentFamily").toString();

    //qDebug() << field("techChoices").toString();
    QStringList skills;
    skills.append(field("schoolSkills").toString().split("|"));
    skills.append(dal->qsl_getclanskills(clan));
    skills.append(dal->qsl_getfamilyskills(family));
    skills.append(field("q7skill").toString());
    skills.append(field("q8skill").toString());
    skills.append(field("upbringingSkill1").toString());
    skills.append(field("upbringingSkill2").toString());
    skills.append(field("upbringingSkill3").toString());
    if(field("q13DisadvChecked").toBool() == true){
        qDebug() << "Question 13 chose disadvantage.  Adding skill: " << field("q13skill").toString();
        skills.append(field("q13skill").toString());
    }
    QStringList skillList = dal->qsl_getskills();

    foreach (const QString string, skills) {
        skillList.removeAll(string);
    }
    ui->nc6_q17_comboBox->clear();
    ui->nc6_q17_comboBox->addItems(skillList);

    ui->nc6_q18_ancestor1_comboBox->clear();
    ui->nc6_q18_ancestor2_comboBox->clear();
    ui->nc6_q18_ancestor1_comboBox->addItems(dal->qsl_getancestors("Core"));
    ui->nc6_q18_ancestor2_comboBox->addItems(dal->qsl_getancestors("Core"));
    ui->nc6_q18_ancestor1_comboBox->setCurrentIndex(-1);
    ui->nc6_q18_ancestor2_comboBox->setCurrentIndex(-1);

   buildq18UI();


   //POW

   ////////PoW
   ///
   ///

   ui->nc6_bond_comboBox->clear();
   QStringList bonds = dal->qsl_getbonds();
   ui->nc6_bond_comboBox->addItems(bonds);

   if(field("characterType").toString() == "Samurai"){
       ui->q17ronin_groupbox->setVisible(false);
       ui->q17s_q18r_groupBox->setTitle("17. How would your parents describe you?");
       ui->q18s_groupbox->setVisible(true);

   }
   else{
       ui->q17ronin_groupbox->setVisible(true);
       ui->q17s_q18r_groupBox->setTitle("18. Who raised you?");
       ui->q18s_groupbox->setVisible(false);
   }




}

void NewCharWizardPage6::on_nc6_q18_ancestor1_rollButton_clicked()
{
    QStringList rangestrings = dal->qsl_getancestorranges(ui->heritagetable_comboBox->currentText());
    int roll = rand()%10+1;
    int index = 0;
    foreach (QString str, rangestrings) {
        qDebug() << "Rangestring = " << str;
        qDebug() << "Roll = " << roll;
        if(roll >= str.split(", ")[0].toInt() && roll <= str.split(", ")[1].toInt() ) {// min == [0], max == [1]
             ui->nc6_q18_ancestor1_comboBox->setCurrentIndex(index);
             return;
        }
        else{
            ++index;
        }
    }


}

void NewCharWizardPage6::on_nc6_q18_ancestor2_rollButton_clicked()
{
    QStringList rangestrings = dal->qsl_getancestorranges(ui->heritagetable_comboBox->currentText());
    int roll = rand()%10+1;
    int index = 0;
    foreach (QString str, rangestrings) {
        qDebug() << "Rangestring = " << str;
        qDebug() << "Roll = " << roll;
        if(roll >= str.split(", ")[0].toInt() && roll <= str.split(", ")[1].toInt() ) {// min == [0], max == [1]
             ui->nc6_q18_ancestor2_comboBox->setCurrentIndex(index);
             return;
        }
        else{
            ++index;
        }
    }


}

void NewCharWizardPage6::on_nc6_q18_ancestor1_comboBox_currentIndexChanged(const QString &arg1)
{
   if(arg1.isEmpty()) {
       ui->nc6_q18_ancestor1_modLabel->setText("");
       return;
   }
   const QStringList mods = dal->qsl_getancestormods(arg1);
   QString modstr = "";
   if(mods.at(0).toInt() != 0){
       modstr+= tr("H:")+mods.at(0) + " ";
   }
   if(mods.at(1).toInt() != 0){
       modstr+= tr("G:")+mods.at(1)+ " ";
   }
   if(mods.at(2).toInt() != 0){
       modstr+= tr("S:")+mods.at(2);
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
    const QStringList mods = dal->qsl_getancestormods(arg1);
    QString modstr = "";
    if(mods.at(0).toInt() != 0){
        modstr+= tr("H:")+mods.at(0) + " ";
    }
    if(mods.at(1).toInt() != 0){
        modstr+= tr("G:")+mods.at(1)+ " ";
    }
    if(mods.at(2).toInt() != 0){
        modstr+= tr("S:")+mods.at(2);
    }


    ui->nc6_q18_ancestor2_modLabel->setText(modstr);

    if(ui->nc6_q18_ancestor2_radioButton->isChecked()){
        doPopulateEffects(); //refresh the effects if you changed this while focusing on this ancestor.
    }
}

void NewCharWizardPage6::on_nc6_q18_ancestor1_radioButton_toggled(const bool checked)
{
    Q_UNUSED(checked)

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


void NewCharWizardPage6::on_nc6_q18_ancestor2_radioButton_toggled(const bool checked)
{
    Q_UNUSED(checked)
    
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
    const QString ancestor = curAncestorBox->currentText();



    QStringList effects = dal->qsl_getancestorseffects(ancestor);
    //start by removing effect placeholders -- these will get added in on page 7 automatically.
    ////////
    ///Shadowlands
    if(effects.contains(dal->translate("Fallen Ancestor"))){
        effects.removeAll(dal->translate("Fallen Ancestor"));
        effects.append(tr("No Change"));
    }
    effects.removeAll(dal->translate("Haunting"));
    //Court
    effects.removeAll(dal->translate("Isolation"));
    effects.removeAll(dal->translate("Benten's Curse"));
    if(effects.contains(dal->translate("Fallen Ancestor"))){
        effects.removeAll(dal->translate("Fallen Ancestor"));
        effects.append(tr("No Change"));
    }
    //Celestial realms - CR
    if(effects.contains(dal->translate("Whispers of Failure"))){
        effects.removeAll(dal->translate("Whispers of Failure"));
        effects.append(tr("No Change"));
    }
    //end supplement custom code

    ui->nc6_q18_otherComboBox->addItems(effects);
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
    else if(curAncestor == dal->translate("Famous Deed") ||
            curAncestor == dal->translate("Glorious Sacrifice") ||
            curAncestor == dal->translate("Triumph Over the Lion") ||
            curAncestor == dal->translate("Sacrifice")

            ){
        //Item Selection
        ui->nc6_q18_secondaryChoice_comboBox->setVisible(true);
        ui->nc6_q18_secondaryChoice_label->setVisible(true);
        ui->nc6_q18_otherLabel->setVisible(true);
        ui->nc6_q18_otherComboBox->setVisible(true);
        ui->nc6_q18_otherrollButton->setVisible(true);
        ui->nc6_q18_special1_comboBox->setVisible(true);
        ui->nc6_q18_special2_comboBox->setVisible(true);
        ui->nc6_q18_specialInstruction_label->setVisible(true);
        ui->nc6_q18_specialInstruction_label->setText(tr("Choose an item quality. The GM chooses another item quality."));
        ui->nc6_q18_spec1_label->setVisible(true);
        ui->nc6_q18_spec2_label->setVisible(true);
        ui->nc6_q18_spec1_label->setText(tr("Player-chosen Quality:"));
        ui->nc6_q18_spec2_label->setText(tr("GM-chosen Quality:"));
    }
    else if (curAncestor == dal->translate("Stolen Knowledge")){
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
    else if (curAncestor == dal->translate("Imperial Heritage")||
             curAncestor == dal->translate("Triumph During Gempuku")||
             curAncestor == dal->translate("Touched by the Fortunes")

             ){
        //Advantage
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
    else if (curAncestor == dal->translate("Unusual Name Origin")){
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
        if(ui->nc6_q18_otherComboBox->currentText() == dal->translate("Ring Exchange")){
            ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
            ui->nc6_q18_secondaryChoice_label->setVisible(false);
            ui->nc6_q18_special1_comboBox->setVisible(true);
            ui->nc6_q18_special2_comboBox->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setText(tr("Choose a ring to raise, and a ring to lower."));
            ui->nc6_q18_spec1_label->setVisible(true);
            ui->nc6_q18_spec2_label->setVisible(true);
            ui->nc6_q18_spec1_label->setText(tr("Raise:"));
            ui->nc6_q18_spec2_label->setText(tr("Lower:"));
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
    /////////////////////////////////////////////
    //// Shadowlands and Courts
    ///
    else if (curAncestor == dal->translate("Blood and Mortar")){
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
    else if (curAncestor == dal->translate("Tainted Blood") ||
             curAncestor == dal->translate("Elegant Craftsman") ||
             curAncestor == dal->translate("Associated with a Natural Disaster") ||
             curAncestor == dal->translate("Mark of the Elements")
             ){
        ui->nc6_q18_otherLabel->setVisible(true);
        ui->nc6_q18_otherComboBox->setVisible(true);
        ui->nc6_q18_otherrollButton->setVisible(false);
        if(ui->nc6_q18_otherComboBox->currentText() == dal->translate("Void ring exchange")){
            ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
            ui->nc6_q18_secondaryChoice_label->setVisible(false);
            ui->nc6_q18_special1_comboBox->setVisible(true);
            ui->nc6_q18_special2_comboBox->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setText(tr("You may raise the Void Ring and choose a ring to lower."));
            ui->nc6_q18_spec1_label->setVisible(true);
            ui->nc6_q18_spec2_label->setVisible(true);
            ui->nc6_q18_spec1_label->setText(tr("Raise:"));
            ui->nc6_q18_spec2_label->setText(tr("Lower:"));
        }
        else if(ui->nc6_q18_otherComboBox->currentText() == dal->translate("Air/Fire ring exchange")){
            ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
            ui->nc6_q18_secondaryChoice_label->setVisible(false);
            ui->nc6_q18_special1_comboBox->setVisible(true);
            ui->nc6_q18_special2_comboBox->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setText(tr("You may choose a ring to lower and raise the Fire or Air Ring."));
            ui->nc6_q18_spec1_label->setVisible(true);
            ui->nc6_q18_spec2_label->setVisible(true);
            ui->nc6_q18_spec1_label->setText(tr("Raise:"));
            ui->nc6_q18_spec2_label->setText(tr("Lower:"));
        }
        else if(ui->nc6_q18_otherComboBox->currentText() == dal->translate("Ring Exchange")){
            ui->nc6_q18_secondaryChoice_comboBox->setVisible(false);
            ui->nc6_q18_secondaryChoice_label->setVisible(false);
            ui->nc6_q18_special1_comboBox->setVisible(true);
            ui->nc6_q18_special2_comboBox->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setVisible(true);
            ui->nc6_q18_specialInstruction_label->setText(tr("You may choose a ring to raise and a ring to lower."));
            ui->nc6_q18_spec1_label->setVisible(true);
            ui->nc6_q18_spec2_label->setVisible(true);
            ui->nc6_q18_spec1_label->setText(tr("Raise:"));
            ui->nc6_q18_spec2_label->setText(tr("Lower:"));
        }

        else{ //no change - hide it all
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
    regenSummary();
}

void NewCharWizardPage6::on_nc6_q18_otherComboBox_currentIndexChanged(const QString &arg1)
{
    if(curAncestorBox == NULL) return; //TODO: error handling here?
    const int heritageRow = curAncestorBox->currentIndex()+1;
    ui->nc6_q18_secondaryChoice_comboBox->clear();
    ui->nc6_q18_special1_comboBox->clear();
    ui->nc6_q18_special2_comboBox->clear();
    const QString effectresult = arg1;

    if(effectresult == ""){
        //nothing selected
    }

    if(ui->heritagetable_comboBox->currentText()=="Core"){
        switch(heritageRow){
        case 1:
        case 2:
            //heritage 1-2//////////////////////////
            if (effectresult == dal->translate("weapon")){
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Weapon"));
            }
            else if (effectresult == dal->translate("set of armor")){
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Armor"));

            }
            else if (effectresult == dal->translate("another item")){
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Item"));

            }
            else if (effectresult == dal->translate("horse or other animal")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Horse"));
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Animal"));

            }
            else if (effectresult == dal->translate("boat or estate")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Estate"));
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Boat"));

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
            if(effectresult == dal->translate("Mahō or Ninjutsu"))
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getmahoninjutsu(1));
            else
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_gettechbytyperank(dal->untranslate(effectresult),1));
            break;
        case 9:
            // heritage 9
            if (effectresult == dal->translate("Blessed Lineage")){
                //add blessed lineage -- no secondary pick.  Set in data?
            }
            break;
        case 10:
            ////////////////// Heritage 10
            if (effectresult == dal->translate("Item (Rank 6 or lower)")){
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
    }
    //SL - SHadowlands heritage table
    else if(ui->heritagetable_comboBox->currentText()=="SL") {
        switch(heritageRow){
        case 1:
            if (effectresult == dal->translate("Blessed Lineage")){
                //add blessed lineage -- no secondary pick.  Set in data?
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            //else if (skillist.contains effectresult ){
            //  addSKill(effectResult);
            //}
            break;
        case 6:

            if (effectresult == tr("No Change")){
                //do item adds
                ui->nc6_q18_special1_comboBox->clear();
                ui->nc6_q18_special2_comboBox->clear();
            }
            else{
                const QMap<QString, int> ringmap = calcCurrentRings();

                QMapIterator<QString, int> i(ringmap);
                while (i.hasNext()) {
                    i.next();
                    if(i.key() == dal->translate("Void"))
                        ui->nc6_q18_special1_comboBox->addItem (i.key());
                    if (i.value() > 1)
                        ui->nc6_q18_special2_comboBox->addItem (i.key());
                }
            }
            break;

        }
    }
    else if(ui->heritagetable_comboBox->currentText()=="CoS"){ //courts of stone
        switch(heritageRow){
        case 1:
            break; //skill
        case 2:
            if (effectresult == dal->translate("Any weapon")){
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Weapon"));
            }
            else if (effectresult == dal->translate("Any game set")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Game Set"));

            }
            else if (effectresult == dal->translate("Other item relevant to your ancestor's victory")){
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Item"));

            }
            else if (effectresult == dal->translate("horse or other animal")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Horse"));
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Animal"));

            }
            else if (effectresult == dal->translate("The deed to a small piece of land on the border of Lion territory")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Estate"));

            }
            ui->nc6_q18_special1_comboBox->addItems(dal->qsl_getqualities());
            ui->nc6_q18_special2_comboBox->addItems(dal->qsl_getqualities());

            break;
        case 3:
            break; //skill
        case 4:
            //support of the kakita dueling academy
            break;
        case 5:
            //skill
            break;
        case 6:
            if (effectresult == tr("No Change")){
                //do item adds
                ui->nc6_q18_special1_comboBox->clear();
                ui->nc6_q18_special2_comboBox->clear();
            }
            else{
                const QMap<QString, int> ringmap = calcCurrentRings();

                QMapIterator<QString, int> i(ringmap);
                while (i.hasNext()) {
                    i.next();
                    if((i.key() == dal->translate("Air") || i.key() == dal->translate("Fire")) )
                        ui->nc6_q18_special1_comboBox->addItem (i.key());
                    if (i.value() > 1)
                        ui->nc6_q18_special2_comboBox->addItem (i.key());
                }
            }
            break;
        }

    }


    else if(ui->heritagetable_comboBox->currentText()=="CR") {
        switch(heritageRow){
        case 1:
            //also has a disadvantage it adds manually
            if (effectresult == tr("No Change")){
                //do item adds
                ui->nc6_q18_special1_comboBox->clear();
                ui->nc6_q18_special2_comboBox->clear();
            }
            else{
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
        case 2:
        case 3:
            break;
        case 4:

            if (effectresult == tr("No Change")){
                //do item adds
                ui->nc6_q18_special1_comboBox->clear();
                ui->nc6_q18_special2_comboBox->clear();
            }
            else{
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
         case 5:
            //heritage 7-8//////////////////////////
            if (effectresult == dal->translate("Weapon")){
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Weapon"));
            }
            else if (effectresult == dal->translate("Set of armor")){
                ui->nc6_q18_secondaryChoice_comboBox->addItems(dal->qsl_getitemsbytype("Armor"));

            }
            else if (effectresult == dal->translate("Valued piece of art")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Valued piece of art"));

            }
            else if (effectresult == dal->translate("Horse or other animal")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Horse"));
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Animal"));

            }
            else if (effectresult == dal->translate("Deed to a small piece of land on the border of Phoenix territory")){
                ui->nc6_q18_secondaryChoice_comboBox->addItem(dal->translate("Deed to land in Phoenix territory"));

            }
            ui->nc6_q18_special1_comboBox->addItems(dal->qsl_getqualities());
            ui->nc6_q18_special2_comboBox->addItems(dal->qsl_getqualities());

            break;
         default:
            break;

        }
    }

    buildq18UI();
}


void NewCharWizardPage6::on_nc6_q18_otherrollButton_clicked()
{
   if (curAncestorBox->currentText() == "") return;
   const QStringList rangestrings = dal->qsl_getheritageranges(curAncestorBox->currentText());
   int roll = rand()%10+1;
   int index = 0;
   foreach (const QString str, rangestrings) {
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

void NewCharWizardPage6::on_heritagetable_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->nc6_q18_ancestor1_comboBox->clear();
    ui->nc6_q18_ancestor2_comboBox->clear();
    ui->nc6_q18_ancestor1_comboBox->addItems(dal->qsl_getancestors(arg1));
    ui->nc6_q18_ancestor2_comboBox->addItems(dal->qsl_getancestors(arg1));
    ui->nc6_q18_ancestor1_comboBox->setCurrentIndex(-1);
    ui->nc6_q18_ancestor2_comboBox->setCurrentIndex(-1);

   buildq18UI();
}



void NewCharWizardPage6::on_nc6_q18_special1_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();

}

void NewCharWizardPage6::on_nc6_q18_special2_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    regenSummary();

}

void NewCharWizardPage6::on_nc6_q17_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
   regenSummary();
}


////////////////TODO - Unify this across all wiz pages


void NewCharWizardPage6::regenSummary(){
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

QMap<QString, int> NewCharWizardPage6::calcCurrentRings(){
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

QMap<QString, int> NewCharWizardPage6::calcSkills(){


    QStringList skills;
    skills.append(dal->qsl_getclanskills(field("currentClan").toString()));
    skills.append(dal->qsl_getfamilyskills(field("currentFamily").toString()));


    //POW
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
               heritage ==   dal->translate("A Little Too Close To Heaven")||
               //Celestial realms
               heritage ==   dal->translate("Great Treatise")||
               heritage ==   dal->translate("Guardian of Forbidden Knowledge")

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

