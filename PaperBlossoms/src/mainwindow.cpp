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

#include <QStyledItemDelegate>
#include <QMessageBox>
#include <QSqlRecord>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enums.h"
#include "dependency/dependencybuilder.h"
#include "dependency/databasedependency.h"


//-------------- Simple Class(s) for coloring entries in QTableViews -------
class CurriculumColorDelegate : public QStyledItemDelegate {

public:
    Character* character;
    CurriculumColorDelegate(Character* character, QObject *parent = 0)
        : QStyledItemDelegate(parent)
    {
        this->character = character;
    }

    void initStyleOption(QStyleOptionViewItem *option,
                         const QModelIndex &index) const
    {
        QStyledItemDelegate::initStyleOption(option, index);

        QStyleOptionViewItem *optionV4 =
                qstyleoption_cast<QStyleOptionViewItem*>(option);

        const QAbstractItemModel * model = index.model();
        int isSpecAccess = model->data(model->index(index.row(), 3)).toInt();
        int rank = model->data(model->index(index.row(), 0)).toInt();

        //if(rank%2==0) optionV4->backgroundBrush = QBrush(QColor(210,175,0,5));
        if(rank == character->rank) optionV4->backgroundBrush = QBrush(QColor(210,175,0,50));
        if(isSpecAccess) optionV4->palette.setColor(QPalette::Text, Qt::blue);
    }
};
//-----------------------------------------------------------------------


MainWindow::MainWindow(QString locale, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(locale == ""){
        curLocale = "en"; //this should NEVER HAPPEN BUT JUST IN CASE
    }
    else{
        curLocale = locale;
    }
    
    this->setStyleSheet("QMainWindow {background-image:url(:/images/resources/sakura_PNG37.png); background-position: center;background-repeat:no-repeat }" );
    this->setWindowIcon(QIcon(":/images/resources/sakura.png"));

#ifdef Q_OS_MAC
    const QFont scriptfont = QFont("Bradley Hand", 20, QFont::Bold);
#else
    const QFont scriptfont = QFont("Segoe Script", 20, QFont::Bold);
#endif
    ui->character_name_label->setFont(scriptfont);

    deps = (new DependencyBuilder())->build(locale);

    ui->character_name_label->setVisible(false);
    ui->tabWidget->setVisible(false);
    ui->actionExport_to_XML->setEnabled(false);
    ui->actionSave_As->setEnabled(false);

    ui->actionGenerate_Character_Sheet->setEnabled(false);
    ui->status_groupBox->setVisible(false);

    QStringList skills = deps->skillsRepository->qsl_getskills();
    foreach (QString skill, skills) {
        this->curCharacter.baseskills[skill] = 0;
    }
    QStringList skillheaders;
    skillheaders << "Skill"<<"Rank"<<"Group";
    skillmodel.setHorizontalHeaderLabels(skillheaders);
    ui->skill_tableview->setModel(&skillmodel);
    QStringList advheaders;
    advheaders << "Type"<<"Advance"<<"Track"<<"Cost";
    advanceStack.setHorizontalHeaderLabels(advheaders);
    ui->advance_tableView->setModel(&advanceStack);
    QStringList eqheaders;
    eqheaders << "Type"<<"Name"<<"Desc"<<"Short Desc"<<"Book"<<"Page"<<"Price"<<"Unit"<<"Rarity"<<"Qualities";
    eqheaders << "Category"<<"Skill"<<"Grip"<<"Min Range"<<"Max Range"<<"DMG"<<"DLS";
    eqheaders <<"Physical"<<"Supernatural";
    equipmodel.setHorizontalHeaderLabels(eqheaders);

    QStringList disadvheaders;
    disadvheaders << "KEY"<<"Name"<<"Ring"<<"Desc"<<"Short Desc"<<"Book"<<"Page"<<"Types";
    dis_advmodel.setHorizontalHeaderLabels(disadvheaders);

    QStringList bondheaders;
    bondheaders << "Name"<<"Rank"<<"Ability"<<"Desc"<<"Short Desc"<<"Book"<<"Page";
    bondmodel.setHorizontalHeaderLabels(bondheaders);

    QStringList techheaders;
    techheaders << "Name"<<"Type"<<"Subtype"<<"Rank"<<"Book"<<"Page"<<"Restriction"<<"Desc"<<"Description";
    techModel.setHorizontalHeaderLabels(techheaders);

    QStringList titleheaders;
    titleheaders << "Title"<<"Advance"<<"Type"<<"Special Access"<<"Max Rank";
    titlemodel.setHorizontalHeaderLabels(titleheaders);

    ui->techniqueTableView->setModel(&techModel);
    ui->techniqueTableView->resizeColumnsToContents();


    ui->curriculum_tableView->setModel(&curriculummodel);
    ui->curriculum_tableView->setItemDelegate(new CurriculumColorDelegate(&curCharacter));


    titleProxyModel.setDynamicSortFilter(true);
    titleProxyModel.setSourceModel(&titlemodel);
    titleProxyModel.setFilterKeyColumn(0);
    ui->title_tableview->setModel(& titleProxyModel);

    weaponProxyModel.setDynamicSortFilter(true);
    weaponProxyModel.setSourceModel(&equipmodel);
    weaponProxyModel.setFilterKeyColumn(0);
    weaponProxyModel.setFilterFixedString("Weapon");
    ui->weapon_tableview->setModel(& weaponProxyModel);

    armorProxyModel.setDynamicSortFilter(true);
    armorProxyModel.setSourceModel(&equipmodel);
    armorProxyModel.setFilterKeyColumn(0);
    armorProxyModel.setFilterFixedString("Armor");
    ui->armor_tableview->setModel(& armorProxyModel);

    perseffProxyModel.setDynamicSortFilter(true);
    perseffProxyModel.setSourceModel(&equipmodel);
    perseffProxyModel.setFilterKeyColumn(0);
    perseffProxyModel.setFilterFixedString("Other");
    ui->other_tableview->setModel(& perseffProxyModel);

    ui->bonds_tableView->setModel(&bondmodel);

    distinctionsProxyModel.setDynamicSortFilter(true);
    distinctionsProxyModel.setSourceModel(&dis_advmodel);
    distinctionsProxyModel.setFilterKeyColumn(0);
    distinctionsProxyModel.setFilterFixedString("Distinctions");
    ui->distinctions_tableView->setModel(& distinctionsProxyModel);

    adversitiesProxyModel.setDynamicSortFilter(true);
    adversitiesProxyModel.setSourceModel(&dis_advmodel);
    adversitiesProxyModel.setFilterKeyColumn(0);
    adversitiesProxyModel.setFilterFixedString("Adversities");
    ui->adversities_tableView->setModel(& adversitiesProxyModel);

    passionsProxyModel.setDynamicSortFilter(true);
    passionsProxyModel.setSourceModel(&dis_advmodel);
    passionsProxyModel.setFilterKeyColumn(0);
    passionsProxyModel.setFilterFixedString("Passions");
    ui->passions_tableView->setModel(& passionsProxyModel);

    anxietiesProxyModel.setDynamicSortFilter(true);
    anxietiesProxyModel.setSourceModel(&dis_advmodel);
    anxietiesProxyModel.setFilterKeyColumn(0);
    anxietiesProxyModel.setFilterFixedString("Anxieties");
    ui->anxieties_tableView->setModel(& anxietiesProxyModel);


    setColumnsHidden();
    ui->character_name_label->setVisible(false);
    this->incompleteTitle = "";

    m_dirtyDataFlag = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::populateUI(){

    //-------------SET Personal notes and NAME ----------------------------
    ui->character_name_label->setVisible(true);
    ui->character_name_label->setText(curCharacter.family + " " + curCharacter.name + ", " + curCharacter.school);
    ui->name_lineEdit->setText(curCharacter.name);
    ui->family_lineEdit->setText(curCharacter.family);
    ui->clan_label->setText(curCharacter.clan);
    ui->ninjo_textEdit->setText(curCharacter.ninjo);
    ui->giri_textEdit->setText(curCharacter.giri);
    ui->notes_textEdit->setText(curCharacter.notes);
    ui->xpSpinBox->setValue(curCharacter.totalXP);

    ui->image_label->clear();
    ui->image_label->setText("Click to add a portrait...");
    if (!curCharacter.portrait.isNull()) {
        const QPixmap p = QPixmap::fromImage(curCharacter.portrait);
        ui->image_label->setPixmap(p.scaled(ui->image_label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

        ui->image_label->adjustSize();
    }

    //---------------INITIALIZE SKILL AND RING RANKS --------------------
    //note -- doesn't touch base values on the character
    const QStringList skills = deps->skillsRepository->qsl_getskills(); //get all skills
    //initialise skillranks
    foreach (const QString skill, skills) {
        curCharacter.skillranks[skill] = 0;
    }
    const QStringList rings = deps->ringsRepository->qsl_getrings(); //get all rings
    foreach (const QString ring, rings) {
        curCharacter.ringranks[ring] = 0;
    }

    //--------------ITERATE THROUGH ADVANCES ------------------------
    advanceStack.clear();
    QStringList advheaders;
    int xp_spent = 0;
    advheaders << "Type"<<"Advance"<<"Track"<<"Cost";
    advanceStack.setHorizontalHeaderLabels(advheaders);

    foreach (const QString advance, curCharacter.advanceStack) {
        QList<QStandardItem*> itemrow;
        foreach(QString cell, advance.split("|")){
            itemrow << new QStandardItem(cell);
        }
        if(itemrow.at(0)->text()=="Skill") {
            //save skillranks for character skill calculation
            curCharacter.skillranks[itemrow.at(1)->text()]++;
        }
        if(itemrow.at(0)->text()=="Ring") {
            //save ringranks for character skill calculation
            curCharacter.ringranks[itemrow.at(1)->text()]++;
        }
        xp_spent += itemrow.at(3)->text().toInt();
        //if(itemrow.at(0)->text() == "Technique"){ //for tech, add it directly to the model
        //    curCharacter.techniques.append(itemrow.at(1)->text());
        //}
        advanceStack.appendRow(itemrow);
    }
    ui->xpSpentLabel->setText(QString::number(xp_spent));
    ui->advance_tableView->resizeColumnsToContents();


    //--------------------CURRICULUM ------------------------------------------
    deps->schoolsRepository->qsm_getschoolcurriculum(&curriculummodel, curCharacter.school);
    for(int i = 0; i<curriculummodel.rowCount(); ++i){
        QSqlRecord record = curriculummodel.record(i);
       // if(record.value("special_access").toInt()==1){
       //     curriculummodel.record(i).setForeground(QBrush(Qt::darkGreen));
       // }
    }
    ui->curriculum_tableView->resizeColumnsToContents();

    //---------------------TITLE-----------------------------------------------
    titlemodel.clear();
    if(curCharacter.titles.count()>0)
        foreach (const QString title, curCharacter.titles) {
        QStringList track = deps->titlesRepository->qsl_gettitletrack(title);
            foreach (const QString row, track) {
                QStringList cells = row.split("|");
                QString tname = cells.at(0);
                QString advname = cells.at(1);
                QString type = cells.at(2);
                QString spec_acc = cells.at(3);
                QString rank = cells.at(4);

                //generate row and add it to title model
                QList<QStandardItem*> itemrow;
                itemrow << new QStandardItem(tname);
                itemrow << new QStandardItem(advname);
                itemrow << new QStandardItem(type);
                itemrow << new QStandardItem(spec_acc);
                itemrow << new QStandardItem(rank);
                titlemodel.appendRow(itemrow);
            }


        }
    QStringList titleheaders;
    titleheaders << "Title"<<"Advance"<<"Type"<<"Special Access"<<"Max Rank";
    titlemodel.setHorizontalHeaderLabels(titleheaders);


    //-------------------SET RANK ---------------------------
    //note - rank and title must be calculated after curric and title curric are set.
    const QPair<int, int> rankdata = recalcRank();
    curCharacter.rank = rankdata.first;
    const int curricXP = rankdata.second;

    ui->curric_status_label->setText("Rank: " + QString::number(curCharacter.rank)+", XP in Rank: "+ QString::number(curricXP));

    //-------------------SET TITLE ---------------------------
    QList<int> titleXPcounts;
    foreach(QString title, curCharacter.titles) {
        titleXPcounts << deps->titlesRepository->qs_gettitlexp(title).toInt();
    }
    const QPair<QString, int> titledata = recalcTitle(titleXPcounts);
    QString curTitle = titledata.first;
    const int titleXP = titledata.second;

    ui->title_status_label->setText("Title: " + curTitle+", Title XP: "+ QString::number(titleXP));
    this->incompleteTitle = curTitle;
    if(curTitle.isEmpty()){
        titleProxyModel.setFilterFixedString("XXXXXXX"); //clear the title block
        ui->addTitle_pushButton->setEnabled(true);
    }
    else{
        titleProxyModel.setFilterFixedString(curTitle);
        ui->addTitle_pushButton->setEnabled(false);
    }

    //-------------------SET ABILITIES------------------------

    curCharacter.abilities.clear();
    QString abiltext = "";
    curCharacter.abilities << deps->schoolsRepository->qsl_getschoolability(curCharacter.school);
    abiltext+=deps->schoolsRepository->qsl_getschoolability(curCharacter.school).at(0) + ", ";
    if(curCharacter.rank > 5) {
        curCharacter.abilities << deps->schoolsRepository->qsl_getschoolmastery(curCharacter.school);
        abiltext+= deps->schoolsRepository->qsl_getschoolmastery(curCharacter.school).at(0)+", ";
    }
    foreach (const QString title, curCharacter.titles) {
        if(title != curTitle) { //there should be no way to get a title that isn't curtitle without finishing it.  So the extras are fininshed - get abilities
            curCharacter.abilities << deps->titlesRepository->qsl_gettitlemastery(title);
            const QStringList abl = deps->titlesRepository->qsl_gettitlemastery(title);
                if(abl.count()>0){
                    abiltext+=abl.at(0)+", ";
                }
        }
    }
    foreach (const QStringList bond, curCharacter.bonds) {
        curCharacter.abilities << deps->bondsRepository->qsl_getbondability(bond.at(0));
        const QStringList abl = deps->bondsRepository->qsl_getbondability(bond.at(0));
                if(abl.count()>0){
                    abiltext+=abl.at(0)+", ";
                }
    }
    if(abiltext.count()>=2) abiltext.chop(2); //trim the last ", "
    ui->ability_label->setText(abiltext);
    //-------------------SET RINGS ---------------------------
    QString ringtext = "";
    QMap <QString, int> ringmap = curCharacter.baserings; //get base values
    QMap <QString, int> engringmap;
    QMapIterator<QString, int> ei(curCharacter.baserings); //grab an english version of the baserings
    while (ei.hasNext()) {
        ei.next();
        engringmap[deps->dal->untranslate(ei.key())] += ei.value();                //add 'Air' (etc) for each extra
    }
    QMapIterator<QString, int> i(curCharacter.ringranks);
    while (i.hasNext()) {
        i.next();
        ringmap[i.key()] += i.value();                //add 'Air' (etc) for each extra
        engringmap[deps->dal->untranslate(i.key())] += i.value();
    }
    ringtext += deps->dal->translate("Air")+ " " +QString::number(ringmap[deps->dal->translate("Air")]) + ", ";
    ringtext += deps->dal->translate("Earth")+  " " +QString::number(ringmap[deps->dal->translate("Earth")]) + ", ";
    ringtext += deps->dal->translate("Fire")+  " " +QString::number(ringmap[deps->dal->translate("Fire")]) + ", ";
    ringtext += deps->dal->translate("Water")+ " " + QString::number(ringmap[deps->dal->translate("Water")]) + ", ";
    ringtext += deps->dal->translate("Void")+ " " +QString::number(ringmap[deps->dal->translate("Void")]);

    ui->ring_label->setText(ringtext);
    ui->ringWidget->setRings(engringmap);

    //------------------SET SKILL TABLE AND VALUES -----------------
    skillmodel.clear();
    QString skilltext = "";
    const QStringList skillgrouplist = deps->skillsRepository->qsl_getskillsandgroup();
    foreach (const QString skill, skillgrouplist) {

        //generate row and add it to skill model
        QList<QStandardItem*> itemrow;
        itemrow << new QStandardItem(skill.split("|").at(0));
        itemrow << new QStandardItem(QString::number(curCharacter.baseskills[skill.split("|").at(0)] + curCharacter.skillranks[skill.split("|").at(0)]));
        itemrow << new QStandardItem(skill.split("|").at(1));
        skillmodel.appendRow(itemrow);
        if(itemrow.at(1)->text().toInt()>0) skilltext += itemrow.at(0)->text()+" "+ itemrow.at(1)->text()+", ";

    }
    if(skilltext.count()>=2) skilltext.chop(2); //trim the last ", "
    ui->skill_label->setText(skilltext);
    QStringList skillheaders;
    skillheaders << "Skill"<<"Rank"<<"Group";
    skillmodel.setHorizontalHeaderLabels(skillheaders);
    ui->skill_tableview->resizeColumnsToContents();

    //------------------SET EQ TABLE-------------------------------------//
    equipmodel.clear();
    foreach (const QStringList equiplist, curCharacter.equipment){
        QList<QStandardItem*> itemrow;
        foreach (QString str, equiplist) {
            itemrow << new QStandardItem(str);
        }
        equipmodel.appendRow(itemrow);
    }
    ui->weapon_tableview->resizeColumnsToContents();
    ui->armor_tableview->resizeColumnsToContents();
    ui->other_tableview->resizeColumnsToContents();
    QStringList eqheaders;
    eqheaders << "Type"<<"Name"<<"Desc"<<"Short Desc"<<"Book"<<"Page"<<"Price"<<"Unit"<<"Rarity"<<"Qualities";
    eqheaders << "Category"<<"Skill"<<"Grip"<<"Min Range"<<"Max Range"<<"DMG"<<"DLS";
    eqheaders <<"Physical"<<"Supernatural";
    equipmodel.setHorizontalHeaderLabels(eqheaders);


    //------------------SET Bond TABLE-------------------------------------//
    bondmodel.clear();
    foreach (const QStringList bondlist, curCharacter.bonds){
        QList<QStandardItem*> bondrow;
        foreach (QString str, bondlist) {
            bondrow << new QStandardItem(str);
        }
        bondmodel.appendRow(bondrow);
    }
    ui->bonds_tableView->resizeColumnsToContents();
    QStringList bondheaders;
    bondheaders << "Name"<<"Rank"<<"Ability"<<"Desc"<<"Short Desc"<<"Book"<<"Page";
    bondmodel.setHorizontalHeaderLabels(bondheaders);

    //---------------CALCULATE DERIVED STATS ------------------------------//
    ui->endurance_label->setText(QString::number(( curCharacter.baserings[deps->dal->translate("Earth")]
                                                +  curCharacter.ringranks[deps->dal->translate("Earth")]
                                                +  curCharacter.baserings[deps->dal->translate("Fire")]
                                                +  curCharacter.ringranks[deps->dal->translate("Fire")])*2));
    ui->composure_label->setText(QString::number(( curCharacter.baserings[deps->dal->translate("Earth")]
                                                 + curCharacter.ringranks[deps->dal->translate("Earth")]
                                                 + curCharacter.baserings[deps->dal->translate("Water")]
                                                 + curCharacter.ringranks[deps->dal->translate("Water")])*2));
    ui->focus_label->setText(QString::number( curCharacter.baserings[deps->dal->translate("Fire")]
                                            + curCharacter.ringranks[deps->dal->translate("Fire")]
                                            + curCharacter.baserings[deps->dal->translate("Air")]
                                            + curCharacter.ringranks[deps->dal->translate("Air")]  ));
    ui->vigilance_label->setText(
                QString::number(qRound(double(curCharacter.baserings[deps->dal->translate("Water")]
                                            + curCharacter.ringranks[deps->dal->translate("Water")]
                                            + curCharacter.baserings[deps->dal->translate("Air")]
                                            + curCharacter.ringranks[deps->dal->translate("Air")])/2.0))); //round up, because the FAQ was cruel.

    ui->glory_spinBox->setValue(curCharacter.glory );
    ui->honor_spinBox->setValue(curCharacter.honor );
    ui->status_spinBox->setValue(curCharacter.status);
    //ui->wealth_label->setText(QString::number(curCharacter.wealth));
    ui->koku_spinBox->setValue(curCharacter.koku);
    ui->bu_spinBox->setValue(curCharacter.bu);
    ui->zeni_spinBox->setValue(curCharacter.zeni);

    //-------------------TECHNIQUE LISTS -------------------------------------
    techModel.clear();
    QString techlist = "";
    foreach(const QString str, curCharacter.techniques){
        techlist += str + ", ";
        QList<QStandardItem*> itemrow;
        foreach (const QString t, deps->techniquesRepository->qsl_gettechbyname(str)){
            //now, do the real work for the tables
            itemrow << new QStandardItem(t);
        }
        techModel.appendRow(itemrow);
    }
    //also check advances
    foreach (const QString advance, curCharacter.advanceStack) {   //iterate through advances
        QList<QStandardItem*> itemrow;                       //generate a row in the advancestack
        foreach(const QString cell, advance.split("|")){           // the advance table is pipe separated for now.  FIx later?
            itemrow << new QStandardItem(cell);              // turn this into qstandarditems to match other paradigms
        }
        if(itemrow.at(0)->text() == "Technique"){            //if it's a tech advance
            QList<QStandardItem*> techrow;                   //create a row for the techModel
            foreach (const QString t, deps->techniquesRepository->qsl_gettechbyname(itemrow.at(1)->text())){ //grab a qstringlist...
                techrow << new QStandardItem(t);                                //and convert it into qstandarditems for the row
            }
            techlist+=itemrow.at(1)->text()+", ";                                    //add it to the techlist
            techModel.appendRow(techrow);                                       //and add it to the tech model.
        }
    }



    if(techlist.length()>=2) techlist.chop(2);
    ui->tech_label->setText(techlist);

    QStringList techheaders;
    techheaders << "Name"<<"Type"<<"Subtype"<<"Rank"<<"Book"<<"Page"<<"Restriction"<<"Desc"<<"Description";
    techModel.setHorizontalHeaderLabels(techheaders);
    //ui->techniqueTableView->horizontalHeader()->setMaximumSectionSize(300);
    ui->techniqueTableView->resizeColumnsToContents();

    //--------------------ADVANTAGES AND DISADVANTAGES ------------------------

    dis_advmodel.clear();
    QString advlist = ""; //simple text string for the front page, for now
    foreach(const QString str, curCharacter.adv_disadv){
        advlist += str + ", "; //populate the string

        const QStringList dis_advdata = deps->advantagesRepository->qsl_getadvdisadvbyname(str);
        QList<QStandardItem*> itemrow;
        foreach (const QString adv_disadv_str, dis_advdata){
            //now, do the real work for the tables
            itemrow << new QStandardItem(adv_disadv_str);
        }
        dis_advmodel.appendRow(itemrow);

    }

    QStringList disadvheaders;
    disadvheaders << "KEY"<<"Name"<<"Ring"<<"Desc"<<"Short Desc"<<"Book"<<"Page"<<"Types";
    dis_advmodel.setHorizontalHeaderLabels(disadvheaders);

    if(advlist.length()>=2) advlist.chop(2);
    ui->adv_label->setText(advlist);

    setColumnsHidden();
    this->setStyleSheet("" );


}

void MainWindow::setColumnsHidden(){
    ui->weapon_tableview->setColumnHidden(Equipment::TYPE, true);
    ui->weapon_tableview->setColumnHidden(Equipment::NAME,false);
    ui->weapon_tableview->setColumnHidden(Equipment::DESC,true);
    ui->weapon_tableview->setColumnHidden(Equipment::SHORT_DESC,true);
    ui->weapon_tableview->setColumnHidden(Equipment::BOOK,false);
    ui->weapon_tableview->setColumnHidden(Equipment::PAGE,false);
    ui->weapon_tableview->setColumnHidden(Equipment::UNIT,false);
    ui->weapon_tableview->setColumnHidden(Equipment::RARITY,false);
    ui->weapon_tableview->setColumnHidden(Equipment::QUALITIES,false);
    ui->weapon_tableview->setColumnHidden(Equipment::W_CATEGORY,false);
    ui->weapon_tableview->setColumnHidden(Equipment::W_SKILL,false);
    ui->weapon_tableview->setColumnHidden(Equipment::W_GRIP,false);
    ui->weapon_tableview->setColumnHidden(Equipment::W_MINRANGE,false);
    ui->weapon_tableview->setColumnHidden(Equipment::W_MAXRANGE,false);
    ui->weapon_tableview->setColumnHidden(Equipment::W_DAM,false);
    ui->weapon_tableview->setColumnHidden(Equipment::W_DLS,false);
    ui->weapon_tableview->setColumnHidden(Equipment::A_PHYSRES,true);
    ui->weapon_tableview->setColumnHidden(Equipment::A_SUPERRES,true);

    ui->armor_tableview->setColumnHidden(Equipment::TYPE, true);
    ui->armor_tableview->setColumnHidden(Equipment::NAME,false);
    ui->armor_tableview->setColumnHidden(Equipment::DESC,true);
    ui->armor_tableview->setColumnHidden(Equipment::SHORT_DESC,true);
    ui->armor_tableview->setColumnHidden(Equipment::BOOK,false);
    ui->armor_tableview->setColumnHidden(Equipment::PAGE,false);
    ui->armor_tableview->setColumnHidden(Equipment::UNIT,false);
    ui->armor_tableview->setColumnHidden(Equipment::RARITY,false);
    ui->armor_tableview->setColumnHidden(Equipment::QUALITIES,false);
    ui->armor_tableview->setColumnHidden(Equipment::W_CATEGORY,true);
    ui->armor_tableview->setColumnHidden(Equipment::W_SKILL,true);
    ui->armor_tableview->setColumnHidden(Equipment::W_GRIP,true);
    ui->armor_tableview->setColumnHidden(Equipment::W_MINRANGE,true);
    ui->armor_tableview->setColumnHidden(Equipment::W_MAXRANGE,true);
    ui->armor_tableview->setColumnHidden(Equipment::W_DAM,true);
    ui->armor_tableview->setColumnHidden(Equipment::W_DLS,true);
    ui->armor_tableview->setColumnHidden(Equipment::A_PHYSRES,false);
    ui->armor_tableview->setColumnHidden(Equipment::A_SUPERRES,false);

    ui->other_tableview->setColumnHidden(Equipment::TYPE, true);
    ui->other_tableview->setColumnHidden(Equipment::NAME,false);
    ui->other_tableview->setColumnHidden(Equipment::DESC,true);
    ui->other_tableview->setColumnHidden(Equipment::SHORT_DESC,true);
    ui->other_tableview->setColumnHidden(Equipment::BOOK,false);
    ui->other_tableview->setColumnHidden(Equipment::PAGE,false);
    ui->other_tableview->setColumnHidden(Equipment::UNIT,false);
    ui->other_tableview->setColumnHidden(Equipment::RARITY,false);
    ui->other_tableview->setColumnHidden(Equipment::QUALITIES,false);
    ui->other_tableview->setColumnHidden(Equipment::W_CATEGORY,true);
    ui->other_tableview->setColumnHidden(Equipment::W_SKILL,true);
    ui->other_tableview->setColumnHidden(Equipment::W_GRIP,true);
    ui->other_tableview->setColumnHidden(Equipment::W_MINRANGE,true);
    ui->other_tableview->setColumnHidden(Equipment::W_MAXRANGE,true);
    ui->other_tableview->setColumnHidden(Equipment::W_DAM,true);
    ui->other_tableview->setColumnHidden(Equipment::W_DLS,true);
    ui->other_tableview->setColumnHidden(Equipment::A_PHYSRES,true);
    ui->other_tableview->setColumnHidden(Equipment::A_SUPERRES,true);

    ui->distinctions_tableView->setColumnHidden(Adv_Disadv::TYPE, true);
    ui->distinctions_tableView->setColumnHidden(Adv_Disadv::DESC, true);
    ui->distinctions_tableView->setColumnHidden(Adv_Disadv::SHORT_DESC, true);

    ui->adversities_tableView->setColumnHidden(Adv_Disadv::TYPE, true);
    ui->adversities_tableView->setColumnHidden(Adv_Disadv::DESC, true);
    ui->adversities_tableView->setColumnHidden(Adv_Disadv::SHORT_DESC, true);

    ui->passions_tableView->setColumnHidden(Adv_Disadv::TYPE, true);
    ui->passions_tableView->setColumnHidden(Adv_Disadv::DESC, true);
    ui->passions_tableView->setColumnHidden(Adv_Disadv::SHORT_DESC, true);

    ui->anxieties_tableView->setColumnHidden(Adv_Disadv::TYPE, true);
    ui->anxieties_tableView->setColumnHidden(Adv_Disadv::DESC, true);
    ui->anxieties_tableView->setColumnHidden(Adv_Disadv::SHORT_DESC, true);

    ui->techniqueTableView->setColumnHidden(Tech::DESCRIPTION,true);

    ui->curriculum_tableView->setColumnHidden(Curric::SPEC, true);

    ui->title_tableview->setColumnHidden(0, true); //hide the title -- it's shown above.

    ui->weapon_tableview->resizeColumnsToContents();
    ui->armor_tableview->resizeColumnsToContents();
    ui->other_tableview->resizeColumnsToContents();

    ui->distinctions_tableView->resizeColumnsToContents();
    ui->adversities_tableView->resizeColumnsToContents();
    ui->passions_tableView->resizeColumnsToContents();
    ui->anxieties_tableView->resizeColumnsToContents();

    ui->techniqueTableView->resizeColumnsToContents();
}

QPair<QString, int> MainWindow::recalcTitle(const QList<int> xp_list){
    bool success = true;
    int curricXP = 0;
    int title_index = 0;
    QString currentTitle = "";
    if(xp_list.count()==0) return QPair<QString, int>("",0);

    if(title_index < curCharacter.titles.count()) {
        currentTitle = curCharacter.titles.at(title_index);
    }
    else{
        currentTitle = "";
    }

    //   advheaders << "Type"<<"Advance"<<"Track"<<"Cost";
    foreach (const QString advance, curCharacter.advanceStack) {
        QStringList itemrow;
        foreach(const QString cell, advance.split("|")){
            itemrow << cell;
        }
        if(itemrow.at(2)=="Title"){
            if(isInTitle(itemrow.at(1),itemrow.at(0), currentTitle)){
                curricXP += itemrow.at(3).toInt();
            }
            else{
                curricXP += qRound(double(itemrow.at(3).toInt())/2.0);
            }
        }
        if(title_index+1>xp_list.count()) {
            success = false;
            continue;
        }
        if( (curricXP>=xp_list.at(title_index))){ //TODO - graceful handling?
            title_index++;
            curricXP = 0;
            if(title_index < curCharacter.titles.count()) {
                currentTitle = curCharacter.titles.at(title_index);
            }
            else{
                currentTitle = "";
            }
        }
   }
    if(!success) QMessageBox::information(this, tr("Error"), tr("Unable to load some Title data. This character depends on data that isn't present, and may be inconsistent. Did you need to import custom data?"));
    return QPair<QString,int>(currentTitle,curricXP);
}
