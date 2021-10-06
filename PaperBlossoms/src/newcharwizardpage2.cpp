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
    this->setTitle(tr("Part 2: Role and School"));
    ui->nc2_HIDDEN_skillLineEdit->setVisible(false); //holds a skill string

    //Handle special cases
    ui->nc2_kitsune_label->setVisible(false);
    ui->nc2_kitsune_comboBox->setVisible(false);
    ui->label_extra_choice->setVisible(false);
    ui->nc2_otherchoice_comboBox->setVisible(false);

    //we're getting all schools anyways; no need for PoW handling
    QStringList kitsuneschoollist = dal->qsl_getschools(field("currentClan").toString(), true);
    kitsuneschoollist.removeAll("Kitsune Impersonator Tradition");
    ui->nc2_kitsune_comboBox->addItems(kitsuneschoollist);
    ui->nc2_kitsune_comboBox->setCurrentIndex(-1);

    ui->nc2_otherchoice_comboBox->addItem("Haunting");
    ui->nc2_otherchoice_comboBox->addItem("Omen of Bad Luck");
    ui->nc2_otherchoice_comboBox->setCurrentIndex(-1);

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
    if(field("characterType").toString() == "Samurai"){
        schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString()));
    }
    else if (field("characterType").toString() == "Gaijin") { //gaijin have ronin schools or their local schools
        schoolModel->setStringList(dal->qsl_getschools(dal->qs_getregionsubtype(field("currentRegion").toString()), false, field("characterType").toString()) );
    }
    else{
        schoolModel->setStringList(dal->qsl_getschools("Rōnin", false, field("characterType").toString() )); //peasants and roning just have ronin schools

    }


    ringModel->setStringList(dal->qsl_getrings());

    registerField("currentSchool",ui->nc2_school_ComboBox,"currentText");
    registerField("currentSchoolIndex*",ui->nc2_school_ComboBox);
    registerField("techChoices",ui->techWidget,"selections",SIGNAL(selectionsChanged)); //????? signal correct?
    registerField("equipSpecialChoices",ui->equipSpecialWidget,"selections",SIGNAL(selectionsChanged));
    registerField("equipChoices",ui->equipWidget,"selections",SIGNAL(selectionsChanged));
    registerField("ringChoices",ui->ringWidget,"selections",SIGNAL(selectionsChanged));
    //registerField("techstring*",ui->nc2_completePlaceholder_lineEdit);
    registerField("schoolSpecialRing",ui->nc2_schoolSpecialtRing_ComboBox,"currentText");
    registerField("schoolSpecialRingIndex",ui->nc2_schoolSpecialtRing_ComboBox);
    registerField("schoolSkills*",ui->nc2_HIDDEN_skillLineEdit);
    registerField("q4Text",ui->nc2_q4_lineEdit);
    registerField("schoolotherchoice",ui->nc2_otherchoice_comboBox,"currentText");

    //ui->nc2_schoolDesc_textEdit->setVisible(false);

    connect(ui->equipWidget,SIGNAL(selectionsChanged(QString)), this, SLOT(equipSelectionChanged(QString)) );
    connect(ui->ringWidget,SIGNAL(selectionsChanged(QString)), this, SLOT(schoolRingSelectionChanged(QString)) );
    regenSummary();

}

NewCharWizardPage2::~NewCharWizardPage2()
{
    delete ui;
}

void NewCharWizardPage2::schoolRingSelectionChanged(const QString newText){
    regenSummary();
    qDebug() << "CAUGHT SCHOOLRINGSELECTIONCHANGED ";
}

void NewCharWizardPage2::equipSelectionChanged(const QString newText){
    if(settingupequip) return;
    const QStringList specialCases = { //special cases
        "One Weapon of Rarity 6 or Lower",
        "Two Items of Rarity 4 or Lower",
        "Two Weapons of Rarity 6 or Lower",
        "One Sword of Rarity 7 or Lower",

        //FoV
        "Two Weapons of Rarity 7 or Lower",

        //PoW
        "One weapon of your signature weapon category of rarity 8 or lower",
        "One Item of Rarity 3 or Lower",
        "One Item of Rarity 4 or Lower",
        "One Item of Rarity 5 or Lower",
        "One Item of Rarity 6 or Lower",
        //"Yumi and quiver of arrows with three special arrows" //special -- handle at end
    };
    ui->equipSpecialWidget->clear();

    const QStringList selected = ui->equipWidget->getCurrent();
    foreach (const QString box, selected) {
        const QString choice = box.split('|').at(1);
        if(specialCases.contains(choice)){    //if the 1-off is a special case
            handleSpecCases(choice);
        }

    }
    const QList<QStringList> equipsets = dal->ql_getlistsofeq(ui->nc2_school_ComboBox->currentText());              //get a list of equipsets
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
    const QString specCase = speccase;        //get thespecial case value
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
    else if (specCase == "One Sword of Rarity 7 or Lower") {
        ui->equipSpecialWidget->addCBox(dal->qsl_getweapontypeunderrarity(7, "Swords"));

    }

    /////////////
    //POW
    else if (specCase == "One weapon of your signature weapon category of rarity 8 or lower") {
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(8));
    }
    else if (specCase == "One Item of Rarity 3 or Lower") {
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(3));
    }
    else if (specCase == "One Item of Rarity 4 or Lower") {
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(4));
    }
    else if (specCase == "One Item of Rarity 5 or Lower") {
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(5));
    }
    else if (specCase == "One Item of Rarity 6 or Lower") {
        ui->equipSpecialWidget->addCBox(dal->qsl_getitemsunderrarity(6));
    }
    //FoV
    else if (specCase == "Two Weapons of Rarity 7 or Lower"){
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(7));
        ui->equipSpecialWidget->addCBox(dal->qsl_getweaponsunderrarity(7));
    }
}

void NewCharWizardPage2::initializePage(){

    //const QString clan = field("currentClan").toString();
    //qDebug()<< "Initializing page 2 with clan = " << clan;
    //schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString()));


    //populate model
    if(field("characterType").toString() == "Samurai"){
        ui->q3_groupBox->setTitle("3. What is your school, and what roles does that school fall into?");
        ui->q4_groupBox->setTitle("4. How do you stand out within your school?");
        schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString()));
    }
    else if (field("characterType").toString() == "Gaijin") { //gaijin have ronin schools or their local schools
        ui->q3_groupBox->setTitle("3. What is your school, and what are its associated roles?");
        ui->q4_groupBox->setTitle("4. What gets you in and out of trouble?");
        schoolModel->setStringList(dal->qsl_getschools(dal->qs_getregionsubtype(field("currentRegion").toString()), false, field("characterType").toString()) );
    }
    else{
        ui->q3_groupBox->setTitle("3. What is your school, and what are its associated roles?");
        ui->q4_groupBox->setTitle("4. What gets you in and out of trouble?");
        schoolModel->setStringList(dal->qsl_getschools("Rōnin", false, "Rōnin" )); //peasants and roning just have ronin schools

    }




    ui->nc2_school_ComboBox->setCurrentIndex(-1);

    ui->nc2_unrestrictedSchool_checkBox->setChecked(false);
    regenSummary();


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
void NewCharWizardPage2::on_nc2_unrestrictedSchool_checkBox_toggled(const bool checked)
{
    schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString(), checked));

    if(field("characterType").toString() == "Samurai"){
        schoolModel->setStringList(dal->qsl_getschools(field("currentClan").toString(), checked));
    }
    else if (field("characterType").toString() == "Gaijin") { //gaijin have ronin schools or their local schools
        schoolModel->setStringList(dal->qsl_getschools(dal->qs_getregionsubtype(field("currentRegion").toString()), checked, "Gaijin") );
    }
    else{
        schoolModel->setStringList(dal->qsl_getschools("Rōnin", checked, "Rōnin" )); //peasants and roning just have ronin schools

    }


}

//////////// Reconfigure page based on selected school //////////////////////
void NewCharWizardPage2::on_nc2_school_ComboBox_currentIndexChanged(const QString &arg1)
{
    //handle special cases
    if(arg1 == "Kitsune Impersonator Tradition"){
        ui->nc2_kitsune_label->setVisible(true);
        ui->nc2_kitsune_comboBox->setVisible(true);
        ui->nc2_kitsune_comboBox->setCurrentIndex(0);
        ui->label_extra_choice->setVisible(false);
        ui->nc2_otherchoice_comboBox->setVisible(false);
        ui->nc2_otherchoice_comboBox->setCurrentIndex(-1);
    }
    else if(arg1 == "Mazoku's Enforcer Tradition"){
        ui->nc2_kitsune_label->setVisible(false);
        ui->nc2_kitsune_comboBox->setVisible(false);
        ui->nc2_kitsune_comboBox->setCurrentIndex(-1);
        ui->label_extra_choice->setVisible(true);
        ui->nc2_otherchoice_comboBox->setVisible(true);
        ui->nc2_otherchoice_comboBox->setCurrentIndex(0);
    }
    else{
        ui->nc2_kitsune_label->setVisible(false);
        ui->nc2_kitsune_comboBox->setVisible(false);
        ui->nc2_kitsune_comboBox->setCurrentIndex(-1);
        ui->label_extra_choice->setVisible(false);
        ui->nc2_otherchoice_comboBox->setVisible(false);
        ui->nc2_otherchoice_comboBox->setCurrentIndex(-1);
    }

    //set the desc
    ui->nc2_schoolDesc_textEdit->setText(dal->qs_getschooldesc(arg1)+ " " + dal->qs_getschoolref(arg1));

    qDebug() << "School changed to:  " + arg1;
    //ui->nc2_schooldesc_textEdit->setText(dal->qs_getschooldesc(arg1));
    const int skillcount = dal->i_getschoolskillcount(arg1);
    qDebug()<< skillcount;
    const QString skilllabel = "Choose " + QString::number(skillcount) + " skills:";
    ui->nc2_skill_Label->setText(skilllabel);
    skillOptModel->setStringList(dal->qsl_getschoolskills(arg1)); //set list with school contents
    skillSelModel->setStringList( QStringList{} );  //clear prior selections, since this changed

    const QStringList subcategories = dal->qsl_gettechniquessubtypes();

    //TECHNIQUES//
    ui->techWidget->clear();
    const QList<QStringList> techsets = dal->ql_getlistsoftech(arg1);
    for(int row = 0; row < techsets.count(); ++row){
        int choosenum = techsets.at(row).at(0).toInt();
        for(int boxcount = 0; boxcount < choosenum;++boxcount){
            QStringList choicesetforcombobox = techsets.at(row);
            QStringList map;
            choicesetforcombobox.removeFirst();

            bool added = false;
            for (const auto& option : choicesetforcombobox){
                added = false;
                for (const auto& subcategory : subcategories){
                    if (added){
                        break;
                    }

                    if (subcategory == option) {
                        map << dal->qsl_gettechniquesbysubcategory(option, 1, 1);
                        added = true;
                    }
                }

                if (added == false) {
                    map << option;
                }
            }

            qDebug() << "Adding Box: " << map ;

            ui->techWidget->addCBox(map, "Choose a technique:");
        }
    }

    //EQUIP//

    QString schoolname = arg1;
    if(arg1 == "Kitsune Impersonator Tradition") schoolname = ui->nc2_kitsune_comboBox->currentText();
    doEquip(schoolname);

    //RINGS//
    ui->ringWidget->clear();
    const QStringList schoolrings = dal->qsl_getschoolrings(arg1);
    foreach(const QString ring, schoolrings){

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
    regenSummary();

}


void NewCharWizardPage2::doEquip(const QString school){
    const QString schoolname = school;

    settingupequip = true;
    ui->equipSpecialWidget->clear();
    ui->equipWidget->clear();
    const QStringList specialCases = {
        "One Weapon of Rarity 6 or Lower",
        "Two Items of Rarity 4 or Lower",
        "Two Weapons of Rarity 6 or Lower",
        "One Sword of Rarity 7 or Lower",
        //FoV
        "Two Weapons of Rarity 7 or Lower",
        //PoW
        "One weapon of your signature weapon category of rarity 8 or lower",
        "One Item of Rarity 3 or Lower",
        "One Item of Rarity 4 or Lower",
        "One Item of Rarity 5 or Lower",
        "One Item of Rarity 6 or Lower",
        "Yumi and quiver of arrows with three special arrows" //special -- handle at end
    };
    const QList<QStringList> equipsets = dal->ql_getlistsofeq(schoolname);              //get a list of equipsets
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
    const QModelIndex index = ui->nc2_skillOpt_listview->currentIndex();
    const QString itemText = index.data(Qt::DisplayRole).toString();
    if(itemText.isEmpty()) return;
    qDebug()<<itemText;

    QStringList sellist = skillSelModel->stringList();
    if (sellist.count() >= dal->i_getschoolskillcount(ui->nc2_school_ComboBox->currentText())) return;

    Q_ASSERT_X(!sellist.contains(itemText), "doAddSkill", "Skill selection list view already contained the selected item text");
    sellist.append(itemText);
    sellist.sort();
    skillSelModel->setStringList(sellist);

    QStringList optlist = skillOptModel->stringList();
    Q_ASSERT_X(optlist.contains(itemText), "doAddSkill", "Skill options list view didn't contain the selected item text");
    optlist.removeAll(itemText);
    skillOptModel->setStringList(optlist);

    QString skillstring = "";
    foreach(QString skill, skillSelModel->stringList()){
        skillstring += skill + "|";
    }
    if (skillstring.length()>=1)skillstring.chop(1); //remove trailing separator
    ui->nc2_HIDDEN_skillLineEdit->setText(skillstring);
    regenSummary();

}

void NewCharWizardPage2::on_nc2_skillRem_pushButton_clicked()
{
   doRemSkill();

}

void NewCharWizardPage2::doRemSkill(){
    const QModelIndex index = ui->nc2_skillSel_listview->currentIndex();
    const QString itemText = index.data(Qt::DisplayRole).toString();
    if(itemText.isEmpty()) return;
    qDebug()<<itemText;

    QStringList sellist = skillSelModel->stringList();
    Q_ASSERT_X(sellist.contains(itemText), "doRemSkill", "Skill selection list view didn't contain the selected item text");
    sellist.removeAll(itemText);
    skillSelModel->setStringList(sellist);

    QStringList optlist = skillOptModel->stringList();
    Q_ASSERT_X(!optlist.contains(itemText), "doRemSkill", "Skill options list view already contained the selected item text");
    optlist.append(itemText);
    optlist.sort();
    skillOptModel->setStringList(optlist);

    QString skillstring = "";
    foreach(QString skill, skillSelModel->stringList()){
        skillstring += skill + "|";
    }
    if (skillstring.length()>=1)skillstring.chop(1); //remove trailing separator
    ui->nc2_HIDDEN_skillLineEdit->setText(skillstring);
    regenSummary();

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


void NewCharWizardPage2::on_nc2_schoolSpecialtRing_ComboBox_currentIndexChanged(const QString &arg1)
{
    //if(ui->nc2_q4_lineEdit->text().isEmpty()){
        ui->nc2_q4_lineEdit->setText(dal->qs_getringdesc(arg1));
    //}
    regenSummary();

}

void NewCharWizardPage2::regenSummary(){
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

QMap<QString, int> NewCharWizardPage2::calcCurrentRings(){
    QMap<QString, int> ringmap;
    QStringList ringlist = dal->qsl_getrings();
    foreach (const QString ring, ringlist) {
        ringmap[ring] = 1;
    }


    if(field("characterType").toString()=="Samurai"){
        //NOW - CALCULATE EXISTING RINGS
        //clan
        ringmap[dal->qs_getclanring(field("currentClan").toString())]++;
        //family
        ringmap[field("familyRing").toString()]++;

    }
    else{

        ///////////PoW
        ///
        ///

        //region
        ringmap[dal->qs_getregionring(field("currentRegion").toString())]++;
        //upbringing
        ringmap[field("upbringingRing").toString()]++;

        /////////////////
    }

    if(ui->nc2_school_ComboBox->currentIndex()!=-1){
        //school
        //QStringList schoolrings = dal->qsl_getschoolrings(field("currentSchool").toString());
        QStringList schoolrings = field("ringChoices").toString().split("|");
        schoolrings.removeAll("");

        foreach (const QString r, schoolrings){
            ringmap[r]++;
        }
    }
    //standout
    ringmap[field("schoolSpecialRing").toString()]++;
    qDebug() << ringmap;
    return ringmap;
}

QMap<QString, int> NewCharWizardPage2::calcSkills(){


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


