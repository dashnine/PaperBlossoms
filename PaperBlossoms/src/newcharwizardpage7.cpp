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

#include "newcharwizardpage7.h"
#include "ui_newcharwizardpage7.h"
#include <QDebug>
#include <QMap>
#include <QMessageBox>
#include "enums.h"
NewCharWizardPage7::NewCharWizardPage7(DataAccessLayer *dal, Character *character, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NewCharWizardPage7)
{

    this->setTitle("Part 7: Death");
    ui->setupUi(this);
    this->dal = dal;
    this->character = character;
    ring_overflow = 0;
    skill_overflow = 0;
}

NewCharWizardPage7::~NewCharWizardPage7()
{
    delete ui;
}

void NewCharWizardPage7::initializePage()
{
    //p1
    const QString clan                = field("currentClan").toString(); //get clan skills
    const QString family              = field("currentFamily").toString(); //get fam skills
    //QString familyring          = field("familyRing").toString();

    //p2
    const QString school              = field("currentSchool").toString();
    const QString techChoices         = field("techChoices").toString();
    const QString equipChoices        = field("equipChoices").toString();
    const QString equipSpecialChoices = field("equipSpecialChoices").toString();
    const QString schoolSkills        = field("schoolSkills").toString();
    const QString schoolSpecialRing   = field("schoolSpecialRing").toString();
    const QString school_standout     = field("q4Text").toString();

    //p3
    const bool hasq7GloryBoost            =  field("q7posradio_glory").toBool();
    //bool hasq7skill                 =  field("q7negradio_skill").toBool();
    const bool hasq8HonorBoost            =  field("q8posradio_honor").toBool();
    //bool hasq8skill                 =  field("q8negradio_skill").toBool();
    const QString q7skill                 =  field("q7skill").toString();
    const QString q8skill                 =  field("q8skill").toString();
    const QString ninjo                   =  field("q6Text").toString();
    const QString giri                   =   field("q5Text").toString();
    const QString clanrelationship        =   field("q7Text").toString();
    const QString bushido                 =   field("q8Text").toString();

    //p4
    const QString distinction             = field("distinction").toString();
    const QString adversity               = field("adversity").toString();
    const QString passion                 = field("passion").toString();
    const QString anxiety                 = field("anxiety").toString();
    const bool pickedSkillAndDisadv       = field("q13DisadvChecked").toBool();
    const bool pickedAdv                  = field("q13AdvChecked").toBool();
    const QString q13skill                = field("q13skill").toString();
    const QString q13advantage            = field("q13Advantage").toString();
    const QString q13disadvantage         = field("q13Disadvantage").toString();
    const QString q9                      = field("q9Text").toString();
    const QString q10                      = field("q10Text").toString();
    const QString q11                      = field("q11Text").toString();
    const QString q12                      = field("q12Text").toString();
    const QString important_relationship  = field("q13Text").toString();

    //p5
    const QString q16item                 = field("q16Item").toString();
    const QString noticefirst             = field("q14Text").toString();
    const QString understress             = field("q15Text").toString();
    const QString existing_relationships  = field("q16Text").toString();

    //p6
    const QString parentSkill = field("parentSkill").toString();
    const QString ancestor1 = field("ancestor1").toString();
    const QString ancestor2 = field("ancestor2").toString();
    const bool ancestor1checked = field("ancestor1checked").toBool();
    const bool ancestor2checked = field("ancestor2checked").toBool();
    const QString othereffects = field("q18OtherEffects").toString();
    const QString secondarychoice = field("q18SecondaryChoice").toString();
    const QString special1 = field("q18Spec1").toString();
    const QString special2 = field("q18Spec2").toString();
    const QString personalName        = field("personalName").toString();
    const QString parents         = field("q17Text").toString();
    const QString heritageSourceTable = field("q18SourceTable").toString();



    //calculate ancestor index for later use
    int ancestorIndex = -1;
    QString heritage = "";
    if(ancestor1checked){
        ancestorIndex = field("ancestor1index").toInt()+1;
        heritage = ancestor1;
    }
    else if (ancestor2checked){
        ancestorIndex = field("ancestor2index").toInt()+1;
        heritage = ancestor2;
    }
    QString lostItem = "";
    //if(ancestorIndex == 2){ //2 is a lost item, and not in starting gear.  Tracking it for use later
    if(heritage == "Glorious Sacrifice"){ //heritage Core 2 is a lost item, and not in starting gear.  Tracking it for use later
        lostItem+= special1 + " " + special2 + " " + secondarychoice + ", ";
    }

    /////////////////// get free text values ////////////////
    QString notes = "";
    notes +=     "4. Standing out in school:\n" + school_standout;
    notes += "\n\n7. Clan Relationship: \n" + clanrelationship;
    notes += "\n\n8. Bushido: \n" + bushido;
    notes += "\n\n9. Accomplishment: \n" + q9;
    notes += "\n\n10. Setback: \n" + q10;
    notes += "\n\n11. At Peace: \n" + q11;
    notes += "\n\n12. Anxiety: \n" + q12;
    notes += "\n\n13. Important Relationship: \n" + important_relationship;
    notes += "\n\n14. Distinctive Traits and Behaviors: \n" + noticefirst;
    notes += "\n\n15. Under Stress: \n" + understress;
    notes += "\n\n16. Existing Relationships With Other Groups: \n" + existing_relationships;
    notes += "\n\n17. Parents: \n" + parents;

    //if(ancestorIndex == 2){ //2 is a lost item, and not in starting gear
    if(heritage == "Glorious Sacrifice"){ //heritage Core 2 is a lost item, and not in starting gear.  Tracking it for use later
        if(!secondarychoice.isEmpty()){
            notes += "\n\n18. Glorious Sacrifice: \nOne of your ancestors perished honorably in battle, "
                     "and their signature " + special1 + " " + special2 + " " + secondarychoice + " was lost. ";
        }
    }


    //////////////////get social stats //////////////////////
    const int status = dal->i_getclanstatus(clan);
    const int glory = dal->i_getfamilyglory(family);
    const int honor = dal->i_getschoolhonor(school);
    QMap<QString, int> socialmap = dal->qm_heritagehonorglorystatus(heritage);
    socialmap["Status"]+=status;
    socialmap["Glory"]+=glory;
    socialmap["Honor"]+=honor;
    if(hasq7GloryBoost) socialmap["Glory"]+=5;
    if(hasq8HonorBoost) socialmap["Honor"] += 10;

    const int wealth = dal->i_getfamilywealth(family);


    //////////////////// Calculate which fields should be tracked and assign the contents
    /// Notes:
    /// Part 3 - (glory or skill) and (honor or skill)
    ///  Part 4 - advantage or (disadvantage and skill)
    ///  Part 6:
    ///     Ancestor 1 or 2 combobox
    ///         Glory, Honor, Status change
    ///         Based on selection of Ancestor:
    ///             1-2 : other, second (item), spec 1 and 2 (qualities)
    ///             3-7 : other = skill
    ///             8   : second = tech
    ///             9   : other = advantage
    ///             10  : based on OTHER
    ///                 item - second = item
    ///                 ring exchange - spec1 and spec2 ring = raise and lower
    ///


    //////////////////// Then, set remaining values. Or print to debug.  Whatever works.
    ui->nc7_famnameschoolclan_label->setText(family + " " + personalName + ", " + school);


    ///////////// POPULATE THE RING BOXES AND STRING//////////////////
    ui->nc7_repRing_1_comboBox->clear();
    ui->nc7_repRing_2_comboBox->clear();
    const QMap<QString, int> ringmap = calcRings();
    QString ringText = "";
    QStringList replaceList;
    QMapIterator<QString, int> i(ringmap);
    while (i.hasNext()) {
        i.next();
        ringText += i.key() + ":" + QString::number(i.value())+ " ";
        if(i.value() < 3) {
            replaceList << i.key();  //compile list of rings currently under 3

        }
    }
    ui->nc7_ringlist_label->setText(ringText);
    if( ring_overflow > 0){
        ui->nc7_repRing_1_comboBox->addItems(replaceList);
        ui->nc7_repRing_1_comboBox->setCurrentIndex(-1);
    }
    this->setVis();


    ui->nc7_repSkill_1_comboBox->clear();
    ui->nc7_repSkill_2_comboBox->clear();
    ui->nc7_repSkill_3_comboBox->clear();

    ///////////// POPULATE THE SKILL BOXES AND STRING//////////////////

    const QMap<QString, int> skillmap = calcSkills();

    QString skillText = "";
    QStringList forbiddenList;
    QMapIterator<QString, int> sli(skillmap);
    while (sli.hasNext()) {
        sli.next();
        skillText += sli.key() + ":" + QString::number(sli.value())+ " ";
        if(sli.value() >= 3) {
            forbiddenList << sli.key();  //compile list of rings currently 3+

        }
    }
    ui->nc7_skilllist_label->setText(skillText);
    QStringList allskills = dal->qsl_getskills();

    foreach (const QString string, forbiddenList) { //remove any that were at 3+
        allskills.removeAll(string);
    }

    if( skill_overflow > 0){
        ui->nc7_repSkill_1_comboBox->addItems(allskills);
        ui->nc7_repSkill_1_comboBox->setCurrentIndex(-1);
    }
    this->setVis();


    ///////////// POPULATE THE TECH STRING//////////////////

    QString techText = "";
    QStringList techList;
    foreach(const QString str, techChoices.split("|")){
        if (!str.isEmpty()){
            techText+= str + ", ";
            techList.append(str);
        }
    }
    //check for tech on part 8
    //if(ancestorIndex == 8){
    if(heritage == "Stolen Knowledge"){
        if(!secondarychoice.isEmpty()){
            techText+= secondarychoice + ", ";
            techList.append(secondarychoice);
        }
    }
    if(techText.length()>=2) techText.chop(2);
    ui->nc7_techlist_label->setText(techText);

    ///////////// POPULATE THE EQUIP STRING//////////////////
    QString eqText = "";
    QList<QStringList> eqList;
    const QStringList specialCases = { //special cases
        "One Weapon of Rarity 6 or Lower",
        "Two Items of Rarity 4 or Lower",
        "Two Weapons of Rarity 6 or Lower",
        "One Sword of Rarity 7 or Lower"
    };

    //-----first, school eq choices -----//
    const QList<QStringList> equipsets = dal->ql_getlistsofeq(school);              //get a list of equipsets
    if(equipsets.count()>0){                                                //if this returned nothing, time to bail
        for(int row = 0; row < equipsets.count(); ++row){                   //otherwise, each row is a set
            if(equipsets.at(row).count()==2){                                //if the count is two, there's only one choice -- this is fixed
                QStringList choicesetforcombobox = equipsets.at(row);   //get the list of choices for the box
                choicesetforcombobox.removeFirst();                     //remove the count at the front
                if(    !choicesetforcombobox.first().isEmpty()
                    && !specialCases.contains(choicesetforcombobox.first())){ //skip special cases -- they're chosen elsewhere
                    eqText += choicesetforcombobox.first() + ", ";         //add the combobox
                    eqList.append(populateItemFields(choicesetforcombobox.first(),dal->qs_getitemtype(choicesetforcombobox.first()))) ;
                }
            }
        }
    }

    foreach(const QString str, equipChoices.split("|")){
        if (!str.isEmpty() && !specialCases.contains(str)){ //skip special choices here too -- getting them next
            eqText+= str + ", ";
            eqList.append(populateItemFields(str,dal->qs_getitemtype(str)));
        }
    }
    foreach(const QString str, equipSpecialChoices.split("|")){ //NOW add special choices
        if (!str.isEmpty()){
            eqText+= str + ", ";
            eqList.append(populateItemFields(str,dal->qs_getitemtype(str)));
        }
    }
    //q16
    eqText+= q16item+ ", ";
    eqList.append(populateItemFields(q16item,dal->qs_getitemtype(q16item)));
    //check for eq on part 8
    //if(ancestorIndex == 1){ //2 is a lost item, and not in starting gear
    if(heritage == "Famous Deed"){ //item
        if(!secondarychoice.isEmpty()){

            eqText+= special1 + " " + special2 + " " + secondarychoice + ", ";
            eqList.append(populateItemFields(secondarychoice, dal->qs_getitemtype(secondarychoice),special1,special2));
        }
    }
    //if(ancestorIndex == 10){
    if(heritage == "Unusual Name Origin"){
        if(othereffects == "Item (Rank 6 or Lower)"){
            if(!secondarychoice.isEmpty()){
                eqText+= secondarychoice + ", ";
                eqList.append(populateItemFields(secondarychoice,dal->qs_getitemtype(secondarychoice)));
            }
        }
    }

    if(eqText.length()>=2) eqText.chop(2);
    ui->nc7_gearlist_label->setText(eqText);



    ///////////// POPULATE THE ADVANTAGE STRING//////////////////
    QString advText = "";
    QStringList advList;
    advText+= distinction +", "
            + adversity + ", "
            + passion + ", "
            + anxiety + ", ";

    advList.append(distinction);
    advList.append(adversity);
    advList.append(passion);
    advList.append(anxiety);

    const QString schooladv = dal->qs_getschooladvdisadv(school);
    if(!schooladv.isEmpty()) {
        advList.append(schooladv);
        advText+=schooladv+", ";
    }

    if(pickedAdv){
        if(!q13advantage.isEmpty()) {
            advText += q13advantage + ", ";
            advList.append(q13advantage);
        }

    }
    else if (pickedSkillAndDisadv){
        if(!q13disadvantage.isEmpty()) {
            advText += q13disadvantage + ", ";
            advList.append(q13disadvantage);
        }
    }

    //if(ancestorIndex == 9){
    if(heritage == "Imperial Heritage"|| heritage == "Blood and Mortar"){
        advText+= othereffects + ", ";
        advList.append(othereffects);
    }
    else if(heritage == "Vengeance for the Fallen"){
        advText+= "Haunting, ";
        advList.append("Haunting");
    }
    else if(heritage == "Tainted Blood"){
        advText+= "Fallen Ancestor, ";
        advList.append("Fallen Ancestor");
    }
    if(advText.length()>=2) advText.chop(2);
    ui->nc7_advlist_label->setText(advText);
    qDebug() << advList;

    ///////////// POPULATE SOCIAL STATS//////////////////
    QString socialText = "";
    QMapIterator<QString, int> si(socialmap);
    while (si.hasNext()) {
        si.next();
        socialText += si.key() + ":" + QString::number(si.value())+ " ";
    }
    socialText+="Wealth:"+QString::number(wealth);
    ui->nc7_sociallist_label->setText(socialText);





    character->name = personalName;
    character->clan = clan;
    character->family = family;
    character->school = school;
    //character->rings IS SET ON CALCRINGS
    //character->skills IS SET ON calcSkills;
    character->honor = socialmap["Honor"];
    character->glory = socialmap["Glory"];
    character->status = socialmap["Status"];
    character->koku = wealth;
    character->techniques = techList;
    character->equipment =  eqList; //TODO: Handle EQUIPMENT
    character->adv_disadv = advList;
    character->ninjo = ninjo;
    character->giri = giri;
    character->notes = notes;
    //notes set here, but it gets a final append when the page is closed.
    character->heritage = heritage;


}
QMap<QString, int> NewCharWizardPage7::calcSkills(){


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



    skill_overflow = 0; //saving off ring overflow for validation later
    QStringList remStrings;
    QMapIterator<QString, int> i(skillmap);
    while (i.hasNext()) {
        i.next();
        if(i.value() > 3) {
            for (int j = 0; j < i.value()-3; ++j){
                remStrings << i.key();                //add 'Air' (etc) for each extra
                skill_overflow++;
            }
        }
    }
    foreach (const QString remskill, remStrings) {
        skillmap[remskill]--;
    }

    const QString repskill1 = ui->nc7_repSkill_1_comboBox->currentText();
    const QString repskill2 = ui->nc7_repSkill_2_comboBox->currentText();
    const QString repskill3 = ui->nc7_repSkill_3_comboBox->currentText();

    if(!repskill1.isEmpty()) {
        skillmap[repskill1]++;
        skill_overflow--;
    }
    if(!repskill2.isEmpty()) {
        skillmap[repskill2]++;
        skill_overflow--;
    }
     if(!repskill3.isEmpty()) {
        skillmap[repskill3]++;
        skill_overflow--;
    }




    qDebug()<<skillmap;
    qDebug()<<"skill_overflow = "<<skill_overflow;
    character->baseskills = skillmap;
    return skillmap;
}

QList<QStringList> NewCharWizardPage7::populateItemFields(const QString name, const QString type, const QString cust_qual_1, const QString cust_qual_2) const {
    QList<QStringList> out;
    QStringList row;
    if(type == "Unknown"){
        qDebug() << "Error: " + name + "not found. Adding a placeholder.";
        row << "Other";
        row << name ; //basedata.at(ItemData::NAME);                                                                 //1
        row << "";//basedata.at(ItemData::DESCRIPTION);                                                          //2
        row << "";//basedata.at(ItemData::SHORT_DESC);                                                           //3
        row << "";//basedata.at(ItemData::REFERENCE_BOOK);                                                       //4
        row << "";//basedata.at(ItemData::REFERENCE_PAGE);                                                       //5
        row << "";//basedata.at(ItemData::PRICE_VALUE);                                                          //6
        row << "";//basedata.at(ItemData::PRICE_UNIT);                                                           //7
        row << "";//basedata.at(ItemData::RARITY);                                                               //8
        row << "";//qualities                                                                                    //9
    }
    else{
        const QStringList basedata = dal->qsl_getbaseitemdata(name, type);
        if (type == "Personal Effect") row << "Other";                                                                                        //0
        else row << type;                                                                                        //0
        row << basedata.at(ItemData::NAME);                                                                 //1
        row << basedata.at(ItemData::DESCRIPTION);                                                          //2
        row << basedata.at(ItemData::SHORT_DESC);                                                           //3
        row << basedata.at(ItemData::REFERENCE_BOOK);                                                       //4
        row << basedata.at(ItemData::REFERENCE_PAGE);                                                       //5
        row << basedata.at(ItemData::PRICE_VALUE);                                                          //6
        row << basedata.at(ItemData::PRICE_UNIT);                                                           //7
        row << basedata.at(ItemData::RARITY);                                                               //8

        const QStringList qualities = dal->qsl_getitemqualities(name,type);
        QString qualstring = "";
        foreach (const QString q, qualities) {
            qualstring += q + " ";
        }
        if(!cust_qual_1.isEmpty()) qualstring += cust_qual_1 + " ";
        if(!cust_qual_2.isEmpty()) qualstring += cust_qual_2 + " ";
        row << qualstring;                                                                                   //9
    }
    if(type == "Weapon"){
        const QStringList baserow = row; //make a copy of row for output, since this may have multiple copies
        const QList<QStringList> weapondata = dal->ql_getweapondata(name);
        foreach (const QStringList gripdata, weapondata) {
            row = baserow;      //set row to baserow and then append this grip's weapon data
            row << gripdata.at(WeaponData::CATEGORY);
            row << gripdata.at(WeaponData::SKILL);
            row << gripdata.at(WeaponData::GRIP);
            row << gripdata.at(WeaponData::RANGE_MIN);
            row << gripdata.at(WeaponData::RANGE_MAX);
            row << gripdata.at(WeaponData::DAMAGE);
            row << gripdata.at(WeaponData::DEADLINESS);
            row << ""; //physical resist
            row << ""; //supernatural resist
            out << row; //drop a row for each grip
        }
    }
    else if (type == "Armor"){
        const QList<QStringList> armordata = dal->ql_getarmordata(name);
        int physresist = 0;
        int supresist = 0;

        foreach (const QStringList resistdata, armordata) {
            if(resistdata.at(ArmorData::RESIST_CATEGORY) == "Physical")
                physresist = resistdata.at(ArmorData::RESIST_VALUE).toInt();
            else if (resistdata.at(ArmorData::RESIST_CATEGORY) == "Supernatural")
                supresist = resistdata.at(ArmorData::RESIST_VALUE).toInt();
            else{
                qWarning() << "ERROR: " + resistdata.at(ArmorData::RESIST_CATEGORY) + " is not Physical or Supernatural. Skipping.";
            }
        }
            row << ""; //gripdata.at(WeaponData::CATEGORY);
            row << ""; //gripdata.at(WeaponData::SKILL);
            row << ""; //gripdata.at(WeaponData::GRIP);
            row << ""; //gripdata.at(WeaponData::RANGE_MIN);
            row << ""; //gripdata.at(WeaponData::RANGE_MAX);
            row << ""; //gripdata.at(WeaponData::DAMAGE);
            row << ""; //gripdata.at(WeaponData::DEADLINESS);
            row << QString::number(physresist);
            row << QString::number(supresist);
            out << row;  // only one row for armor
    }
    else{ //personal effect - "Other"
            row << ""; //gripdata.at(WeaponData::CATEGORY);
            row << ""; //gripdata.at(WeaponData::SKILL);
            row << ""; //gripdata.at(WeaponData::GRIP);
            row << ""; //gripdata.at(WeaponData::RANGE_MIN);
            row << ""; //gripdata.at(WeaponData::RANGE_MAX);
            row << ""; //gripdata.at(WeaponData::DAMAGE);
            row << ""; //gripdata.at(WeaponData::DEADLINESS);
            row << ""; //physresist;
            row << ""; //supresist;
            out << row;  // only one row for other, and empty entries for the other cols
    }
    return out;
}

QMap<QString, int> NewCharWizardPage7::calcRings(){

    //initialize the ring map
    QMap<QString, int> ringmap;
    const QStringList ringlist = dal->qsl_getrings();
    foreach (const QString ring, ringlist) {
        ringmap[ring] = 1;
    }

    //NOW - CALCULATE EXISTING RINGS
    //clan
    ringmap[dal->qs_getclanring(field("currentClan").toString())]++;
    //family
    ringmap[field("familyRing").toString()]++;
    //school
    QStringList schoolrings = field("ringChoices").toString().split("|");
    schoolrings.removeAll("");
    //QStringList schoolrings = dal->qsl_getschoolrings(field("currentSchool").toString());
    foreach (const QString r, schoolrings){
        ringmap[r]++;
    }
    //standout
    ringmap[field("schoolSpecialRing").toString()]++;

    //check for ringswap on part 6
    if(field("q18OtherEffects").toString() == "Ring Exchange" || field("q18OtherEffects").toString() == "Void ring exchange"){
        ringmap[field("q18Spec1").toString()]++;
        ringmap[field("q18Spec2").toString()]--;
        qDebug()<< "adjusting based on Ring Exchange";
    }

    ring_overflow = 0; //saving off ring overflow for validation later
    QStringList oldRings;
    QMapIterator<QString, int> i(ringmap);
    while (i.hasNext()) {
        i.next();
        if(i.value() > 3) {
            for (int j = 0; j < i.value()-3; ++j){
                oldRings << i.key();                //add 'Air' (etc) for each extra
                ring_overflow++;
            }
        }
    }
    foreach (const QString remring, oldRings) {
        ringmap[remring]--;
    }

    const QString repring1 = ui->nc7_repRing_1_comboBox->currentText();
    const QString repring2 = ui->nc7_repRing_2_comboBox->currentText();

    if(!repring1.isEmpty()) {
        ringmap[repring1]++;
        ring_overflow--;
    }
    if(!repring2.isEmpty()){
        ringmap[repring2]++;
        ring_overflow--;
    }

    qDebug() << ringmap;
    character->baserings = ringmap;
    return ringmap;

}

bool NewCharWizardPage7::validatePage(){
    const QMap<QString, int> ringmap = this->calcRings();
    QMapIterator<QString, int> i(ringmap);
    while (i.hasNext()) {
        i.next();
        if(i.value() > 3) {
            QMessageBox msg;
            msg.setText("Error: "+ i.key()+ " is greater than 3.");
            msg.exec();
            return false;
        }
    }
    if(ring_overflow>0){
        QMessageBox msg;
        msg.setText("Error: please select replacement ring(s).");
        msg.exec();
        return false;
    }
    if(skill_overflow>0){
        QMessageBox msg;
        msg.setText("Error: please select replacement skill(s).");
        msg.exec();
        return false;
    }
    character->notes.append("\n\nDeath:\n"+ui->nc7_q20_lineEdit->text()); //append the death right before closing.
    return true;
}

void NewCharWizardPage7::ringsChanged(const QString dynamicRingString){
    QMap<QString, int> ringmap = calcRings();
    foreach (QString ring, dynamicRingString.split("|")) {
        if (ring != ""){
            ringmap[ring]++;
        }

    }
    QString ringText = "";
    QMapIterator<QString, int> i(ringmap);
    while (i.hasNext()) {
        i.next();
        ringText += i.key() + ":" + QString::number(i.value())+ " ";
    }
    ui->nc7_ringlist_label->setText(ringText);

}

void NewCharWizardPage7::on_nc7_repRing_1_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->nc7_repRing_2_comboBox->clear();
    const QMap<QString, int> ringmap = calcRings();
    QString ringText = "";
    QStringList replaceList;
    QMapIterator<QString, int> i(ringmap);
    while (i.hasNext()) {
        i.next();
        ringText += i.key() + ":" + QString::number(i.value())+ " ";
        if(i.value() < 3) {
            replaceList << i.key();  //compile list of rings currently under 3

        }
    }
    ui->nc7_ringlist_label->setText(ringText);
    if(ui->nc7_repRing_1_comboBox->currentText() != "" && ring_overflow > 0){
        ui->nc7_repRing_2_comboBox->addItems(replaceList);
        ui->nc7_repRing_2_comboBox->setCurrentIndex(-1);
    }
    this->setVis();
}

void NewCharWizardPage7::on_nc7_repRing_2_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    const QMap<QString, int> ringmap = calcRings();
    QString ringText = "";
    QStringList replaceList;
    QMapIterator<QString, int> i(ringmap);
    while (i.hasNext()) {
        i.next();
        ringText += i.key() + ":" + QString::number(i.value())+ " ";

    }
    ui->nc7_ringlist_label->setText(ringText);
    this->setVis();
}

void NewCharWizardPage7::on_nc7_repSkill_1_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->nc7_repSkill_2_comboBox->clear();

    const QMap<QString, int> skillmap = calcSkills();

    QString skillText = "";
    QStringList forbiddenList;
    QMapIterator<QString, int> sli(skillmap);
    while (sli.hasNext()) {
        sli.next();
        skillText += sli.key() + ":" + QString::number(sli.value())+ " ";
        if(sli.value() >= 3) {
            forbiddenList << sli.key();  //compile list of rings currently 3+

        }
    }
    ui->nc7_skilllist_label->setText(skillText);
    QStringList allskills = dal->qsl_getskills();

    foreach (const QString string, forbiddenList) { //remove any that were at 3+
        allskills.removeAll(string);
    }

    if(ui->nc7_repSkill_1_comboBox->currentText() != "" && skill_overflow > 0){
        ui->nc7_repSkill_2_comboBox->addItems(allskills);
        ui->nc7_repSkill_2_comboBox->setCurrentIndex(-1);
    }
    this->setVis();
}

void NewCharWizardPage7::on_nc7_repSkill_2_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->nc7_repSkill_3_comboBox->clear();

    const QMap<QString, int> skillmap = calcSkills();

    QString skillText = "";
    QStringList forbiddenList;
    QMapIterator<QString, int> sli(skillmap);
    while (sli.hasNext()) {
        sli.next();
        skillText += sli.key() + ":" + QString::number(sli.value())+ " ";
        if(sli.value() >= 3) {
            forbiddenList << sli.key();  //compile list of rings currently 3+

        }
    }
    ui->nc7_skilllist_label->setText(skillText);
    QStringList allskills = dal->qsl_getskills();

    foreach (const QString string, forbiddenList) { //remove any that were at 3+
        allskills.removeAll(string);
    }

    if(ui->nc7_repSkill_2_comboBox->currentText() != "" && skill_overflow > 0){
        ui->nc7_repSkill_3_comboBox->addItems(allskills);
        ui->nc7_repSkill_3_comboBox->setCurrentIndex(-1);
    }
    this->setVis();
}

void NewCharWizardPage7::on_nc7_repSkill_3_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    const QMap<QString, int> skillmap = calcSkills();

    QString skillText = "";
    QMapIterator<QString, int> sli(skillmap);
    while (sli.hasNext()) {
        sli.next();
        skillText += sli.key() + ":" + QString::number(sli.value())+ " ";
    }
    ui->nc7_skilllist_label->setText(skillText);
    this->setVis();
}

void NewCharWizardPage7::setVis(){
    if(ui->nc7_repRing_1_comboBox->count() == 0){
        ui->nc7_repRing_1_comboBox->setVisible(false);
        ui->nc7_replacering_label->setVisible(false);
    }
    else{
        ui->nc7_repRing_1_comboBox->setVisible(true);
        ui->nc7_replacering_label->setVisible(true);
    }
    if(ui->nc7_repRing_2_comboBox->count() == 0){
        ui->nc7_repRing_2_comboBox->setVisible(false);
    }
    else{
        ui->nc7_repRing_2_comboBox->setVisible(true);
    }

    if(ui->nc7_repSkill_1_comboBox->count() == 0){
        ui->nc7_repSkill_1_comboBox->setVisible(false);
        ui->nc7_replaceskill_label->setVisible(false);
    }
    else {
        ui->nc7_repSkill_1_comboBox->setVisible(true);
        ui->nc7_replaceskill_label->setVisible(true);
    }
    if(ui->nc7_repSkill_2_comboBox->count() == 0){
        ui->nc7_repSkill_2_comboBox->setVisible(false);
    }
    else {
        ui->nc7_repSkill_2_comboBox->setVisible(true);
    }
    if(ui->nc7_repSkill_3_comboBox->count() == 0){
        ui->nc7_repSkill_3_comboBox->setVisible(false);
    }
    else {
        ui->nc7_repSkill_3_comboBox->setVisible(true);
    }

    if(ui->nc7_replaceskill_label->isVisible() || ui->nc7_replaceskill_label->isVisible()){
        ui->nc7_divider->setVisible(true);
        ui->nc7_replaceinstructions_label->setVisible(true);
    }
    else{
        ui->nc7_divider->setVisible(false);
        ui->nc7_replaceinstructions_label->setVisible(false);
    }
}
