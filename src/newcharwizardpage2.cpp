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

#include "newcharwizardpage2.h"
#include "ui_newcharwizardpage2.h"
#include <QDebug>
#include <QMessageBox>

NewCharWizardPage2::NewCharWizardPage2(DataAccessLayer* dal, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage2)
{
    ui->setupUi(this);
    this->dal = dal;
    this->setTitle("Part 2: Role and School");
    ui->nc2_HIDDEN_skillLineEdit->setVisible(false); //holds a skill string

    //Handle special cases
    ui->nc2_kitsune_label->setVisible(false);
    ui->nc2_kitsune_comboBox->setVisible(false);
    QStringList kitsuneschoollist = dal->qsl_getschools(field("currentClan").toString(), true);
    kitsuneschoollist.removeAll("Kitsune Impersonator Tradition");
    ui->nc2_kitsune_comboBox->addItems(kitsuneschoollist);
    ui->nc2_kitsune_comboBox->setCurrentIndex(-1);

    settingupequip = false;

    //initialize models
    schoolModel   = new QStringListModel;
    skillOptModel = new QStringListModel;
    skillSelModel = new QStringListModel;
    ringModel     = new QStringListModel;

    //assign models to views
    ui->nc2_school_ComboBox->setModel(schoolModel);
    ui->nc2_skillOpt_listview->setModel(skillOptModel);
    ui->nc2_skillSel_listview->setModel(skillSelModel);
    ui->nc2_schoolSpecialtRing_ComboBox->setModel(ringModel);

    //populate model
    schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString()));
    ringModel->setStringList(dal->qsl_getrings());

    registerField("currentSchool",ui->nc2_school_ComboBox,"currentText");
    registerField("currentSchoolIndex*",ui->nc2_school_ComboBox);
    registerField("techChoices",ui->techWidget,"selections",SIGNAL(selectionsChanged)); //????? signal correct?
    registerField("equipSpecialChoices",ui->equipSpecialWidget,"selections",SIGNAL(selectionsChanged));
    registerField("equipChoices",ui->equipWidget,"selections",SIGNAL(selectionsChanged));
    registerField("ringChoices",ui->ringWidget,"selections",SIGNAL(selectionsChanged));
    //registerField("techstring*",ui->nc2_completePlaceholder_lineEdit);
    registerField("schoolSpecialRing",ui->nc2_schoolSpecialtRing_ComboBox,"currentText");
    registerField("schoolSpecialRingIndex*",ui->nc2_schoolSpecialtRing_ComboBox);
    registerField("schoolSkills*",ui->nc2_HIDDEN_skillLineEdit);
    registerField("q4Text",ui->nc2_q4_lineEdit);

    //ui->nc2_schoolDesc_textEdit->setVisible(false);

    connect(ui->equipWidget,SIGNAL(selectionsChanged(QString)), this, SLOT(equipSelectionChanged(QString)) );
}

NewCharWizardPage2::~NewCharWizardPage2()
{
    delete ui;
}

void NewCharWizardPage2::equipSelectionChanged(QString newText){
    if(settingupequip) return;
    QStringList specialCases = { //special cases
        "One Weapon of Rarity 6 or Lower",
        "Two Items of Rarity 4 or Lower",
        "Two Weapons of Rarity 6 or Lower"
    };
    ui->equipSpecialWidget->clear();

    QStringList selected = ui->equipWidget->getCurrent();
    foreach (QString box, selected) {
        QString choice = box.split('|').at(1);
        if(specialCases.contains(choice)){    //if the 1-off is a special case
            handleSpecCases(choice);
        }

    }
    QList<QStringList> equipsets = dal->ql_getlistsofeq(ui->nc2_school_ComboBox->currentText());              //get a list of equipsets
    if(equipsets.count()>0){                                                //if this returned nothing, time to bail
        for(int row = 0; row < equipsets.count(); ++row){                   //otherwise, each row is a set
            if(equipsets.at(row).count()==2){                                //if the count is two, there's only one choice -- skip it
                QStringList choicesetforcombobox = equipsets.at(row);       //get the list of choices for the box
                choicesetforcombobox.removeFirst();                         //remove the count at the front
                if(specialCases.contains(choicesetforcombobox.first())){    //if the 1-off is a special case
                    handleSpecCases(choicesetforcombobox.first());
                }
            }
        }
    }
    qDebug() << "CAUGHT EQUIPSELECTIONCHANGED " <<newText;
}

void NewCharWizardPage2::handleSpecCases(QString speccase){
    QString specCase = speccase;        //get thespecial case value
    if(specCase == "One Weapon of Rarity 6 or Lower"){
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(6));
    }
    else if (specCase == "Two Weapons of Rarity 6 or Lower"){
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(6));
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(6));

    }
    else if (specCase == "Two Items of Rarity 4 or Lower") {
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(4));
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(4));

    }
}

void NewCharWizardPage2::initializePage(){

    QString clan = field("currentClan").toString();
    qDebug()<< "Initializing page 2 with clan = " << clan;
    schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString()));
    ui->nc2_school_ComboBox->setCurrentIndex(-1);

    ui->nc2_unrestrictedSchool_checkBox->setChecked(false);

}

bool NewCharWizardPage2::validatePage(){

    if(skillSelModel->rowCount() <  dal->i_getschoolskillcount(ui->nc2_school_ComboBox->currentText())){
        QMessageBox msg;
        msg.setText("Error: insufficient skills selected.");
        msg.exec();
        return false;
    }
    return true;
}

///////////// Allow schools outside of clan. /////////////////////////
void NewCharWizardPage2::on_nc2_unrestrictedSchool_checkBox_toggled(bool checked)
{
    schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString(), checked));
}

//////////// Reconfigure page based on selected school //////////////////////
void NewCharWizardPage2::on_nc2_school_ComboBox_currentIndexChanged(const QString &arg1)
{
    //handle special cases
    if(arg1 == "Kitsune Impersonator Tradition"){
        ui->nc2_kitsune_label->setVisible(true);
        ui->nc2_kitsune_comboBox->setVisible(true);
        ui->nc2_kitsune_comboBox->setCurrentIndex(0);
    }
    else{
        ui->nc2_kitsune_label->setVisible(false);
        ui->nc2_kitsune_comboBox->setVisible(false);
        ui->nc2_kitsune_comboBox->setCurrentIndex(-1);
    }

    //set the desc
    ui->nc2_schoolDesc_textEdit->setText(dal->qs_getschooldesc(arg1)+ " " + dal->qs_getschoolref(arg1));

    qDebug() << "School changed to:  " + arg1;
    //ui->nc2_schooldesc_textEdit->setText(dal->qs_getschooldesc(arg1));
    int skillcount = dal->i_getschoolskillcount(arg1);
    qDebug()<< skillcount;
    QString skilllabel = "Choose " + QString::number(skillcount) + " skills:";
    ui->nc2_skill_Label->setText(skilllabel);
    skillOptModel->setStringList(dal->qsl_getschoolskills(arg1)); //set list with school contents
    skillSelModel->setStringList( QStringList{} );  //clear prior selections, since this changed

    //TECHNIQUES//
    ui->techWidget->clear();
    QList<QStringList> techsets = dal->ql_getlistsoftech(arg1);
    for(int row = 0; row < techsets.count(); ++row){
        int choosenum = techsets.at(row).at(0).toInt();
        for(int boxcount = 0; boxcount < choosenum;++boxcount){
            QStringList choicesetforcombobox = techsets.at(row);
            choicesetforcombobox.removeFirst();
            qDebug() << "Adding Box: " << choicesetforcombobox ;
            ui->techWidget->addCBox(choicesetforcombobox, "Choose a technique:");
        }
    }

    //EQUIP//

    QString schoolname = arg1;
    if(arg1 == "Kitsune Impersonator Tradition") schoolname = ui->nc2_kitsune_comboBox->currentText();
    doEquip(schoolname);
    /*
    settingupequip = true;
    ui->equipSpecialWidget->clear();
    ui->equipWidget->clear();
    QStringList specialCases = {
        "One Weapon of Rarity 6 or Lower",
        "Two Items of Rarity 4 or Lower",
        "Two Weapons of Rarity 6 or Lower"
    };
    QList<QStringList> equipsets = dal->ql_getlistsofeq(schoolname);              //get a list of equipsets
    if(equipsets.count()>0){                                                //if this returned nothing, time to bail
        for(int row = 0; row < equipsets.count(); ++row){                   //otherwise, each row is a set
            if(equipsets.at(row).count()>2){                                //if the count is two, there's only one choice -- skip it
                int choosenum = equipsets.at(row).at(0).toInt();            //the first number is how many you can choose
                for(int boxcount = 0; boxcount < choosenum;++boxcount){     //otherwise, add a box for each count you get to choose
                    QStringList choicesetforcombobox = equipsets.at(row);   //get the list of choices for the box
                    choicesetforcombobox.removeFirst();                     //remove the count at the front

                    qDebug() << "Adding Box: " << choicesetforcombobox ;
                    ui->equipWidget->addCBox(choicesetforcombobox, "Choose an item:");         //add the combobox
                }
            }
            else{                                                           //if it's 1, handle only if it's multiitem
                QStringList choicesetforcombobox = equipsets.at(row);       //get the list of choices for the box
                choicesetforcombobox.removeFirst();                         //remove the count at the front
                if(specialCases.contains(choicesetforcombobox.first())){    //if the 1-off is a special case
                    handleSpecCases(choicesetforcombobox.first());
                }
            }
        }
    }
    settingupequip = false;
    */


    //RINGS//
    ui->ringWidget->clear();
    QStringList schoolrings = dal->qsl_getschoolrings(arg1);
    foreach(QString ring, schoolrings){

        QStringList choicesetforcombobox;

        if(ring == "any"){
            choicesetforcombobox = dal->qsl_getrings();
        }
        else{
            choicesetforcombobox.append( ring);
        }
        qDebug() << "Adding Box: " << choicesetforcombobox ;
        ui->ringWidget->addCBox(choicesetforcombobox, "Ring:");

    }
    //this->resize(sizeHint());
    this->adjustSize();
    this->window()->adjustSize();
}


void NewCharWizardPage2::doEquip(QString school){
    QString schoolname = school;

    settingupequip = true;
    ui->equipSpecialWidget->clear();
    ui->equipWidget->clear();
    QStringList specialCases = {
        "One Weapon of Rarity 6 or Lower",
        "Two Items of Rarity 4 or Lower",
        "Two Weapons of Rarity 6 or Lower"
    };
    QList<QStringList> equipsets = dal->ql_getlistsofeq(schoolname);              //get a list of equipsets
    if(equipsets.count()>0){                                                //if this returned nothing, time to bail
        for(int row = 0; row < equipsets.count(); ++row){                   //otherwise, each row is a set
            if(equipsets.at(row).count()>2){                                //if the count is two, there's only one choice -- skip it
                int choosenum = equipsets.at(row).at(0).toInt();            //the first number is how many you can choose
                for(int boxcount = 0; boxcount < choosenum;++boxcount){     //otherwise, add a box for each count you get to choose
                    QStringList choicesetforcombobox = equipsets.at(row);   //get the list of choices for the box
                    choicesetforcombobox.removeFirst();                     //remove the count at the front

                    qDebug() << "Adding Box: " << choicesetforcombobox ;
                    ui->equipWidget->addCBox(choicesetforcombobox, "Choose an item:");         //add the combobox
                }
            }
            else{                                                           //if it's 1, handle only if it's multiitem
                QStringList choicesetforcombobox = equipsets.at(row);       //get the list of choices for the box
                choicesetforcombobox.removeFirst();                         //remove the count at the front
                if(specialCases.contains(choicesetforcombobox.first())){    //if the 1-off is a special case
                    handleSpecCases(choicesetforcombobox.first());
                }
            }
        }
    }
    settingupequip = false;

}

/////////////// ADD and REMOVE Skills ///////////////////////////////
void NewCharWizardPage2::on_nc2_skillAdd_pushButton_clicked()
{
    doAddSkill();
}

void NewCharWizardPage2::doAddSkill(){
    QModelIndex index = ui->nc2_skillOpt_listview->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    if(itemText.isEmpty()) return;
    qDebug()<<itemText;
    QStringList sellist = skillSelModel->stringList();
    if (!sellist.contains(itemText) && sellist.count() < dal->i_getschoolskillcount(ui->nc2_school_ComboBox->currentText())){
        sellist.append(itemText);
        skillSelModel->setStringList(sellist);
    }
    QString skillstring = "";
    foreach(QString skill, skillSelModel->stringList()){
        skillstring += skill + "|";
    }
    if (skillstring.length()>=1)skillstring.chop(1); //remove trailing separator
    ui->nc2_HIDDEN_skillLineEdit->setText(skillstring);
}

void NewCharWizardPage2::on_nc2_skillRem_pushButton_clicked()
{
   doRemSkill();
}

void NewCharWizardPage2::doRemSkill(){
    QModelIndex index = ui->nc2_skillSel_listview->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    qDebug()<<itemText;
    QStringList sellist = skillSelModel->stringList();
    if (sellist.contains(itemText)){
        sellist.removeAll(itemText);
        skillSelModel->setStringList(sellist);
    }
}

void NewCharWizardPage2::on_nc2_kitsune_comboBox_currentTextChanged(const QString &arg1)
{
    if(!ui->nc2_school_ComboBox->currentText().isEmpty())
        doEquip(arg1);
}

void NewCharWizardPage2::on_nc2_skillOpt_listview_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    doAddSkill();
}

void NewCharWizardPage2::on_nc2_skillSel_listview_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    doRemSkill();
}

