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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWizard>
#include <QLabel>
#include <QVBoxLayout>
#include "newcharacterwizard.h"
#include <QDebug>
#include "character.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "aboutdialog.h"
#include "addadvancedialog.h"
#include "addtitledialog.h"
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QStyledItemDelegate>
#include <QColor>
#include <QPalette>
#include "additemdialog.h"
#include "enums.h"
#include "clicklabel.h"
#include "adddisadvdialog.h"
#include "pboutputdata.h"
#include "renderdialog.h"
#include <QDesktopServices>
#include <QCoreApplication>
#include <QApplication>
#include <QFontDatabase>



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

        QStyleOptionViewItemV4 *optionV4 =
                qstyleoption_cast<QStyleOptionViewItemV4*>(option);

        const QAbstractItemModel * model = index.model();
        int isSpecAccess = model->data(model->index(index.row(), 3)).toInt();
        int rank = model->data(model->index(index.row(), 0)).toInt();

        //if(rank%2==0) optionV4->backgroundBrush = QBrush(QColor(210,175,0,5));
        if(rank == character->rank) optionV4->backgroundBrush = QBrush(QColor(210,175,0,50));
        if(isSpecAccess) optionV4->palette.setColor(QPalette::Text, Qt::blue);
    }
};
//-----------------------------------------------------------------------


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("QMainWindow {background-image:url(:/images/resources/sakura_PNG37.png); background-position: center;background-repeat:no-repeat }" );
    this->setWindowIcon(QIcon(":/images/resources/pink-sakura-01-hi.png"));
    //int id = QFontDatabase::addApplicationFont(":/images/resources/Bradley Hand Bold.ttf");
    //int id = QFontDatabase::addApplicationFont("C:/Users/flux/git/paperblossom/resources/Bradley Hand Bold.ttf");
    //int id = QFontDatabase::addApplicationFontFromData(fileContents(":/images/resources/Bradley Hand Bold.ttf"));
    //qDebug() << id;

#ifdef Q_OS_MAC
    const QFont scriptfont = QFont("Bradley Hand", 20, QFont::Bold);
#else
    const QFont scriptfont = QFont("Segoe Script", 20, QFont::Bold);
#endif
    ui->character_name_label->setFont(scriptfont);

    dal = new DataAccessLayer;

    ui->character_name_label->setVisible(false);
    ui->tabWidget->setVisible(false);
    ui->actionGenerate_Character_Sheet->setEnabled(false);
    ui->status_groupBox->setVisible(false);

    QStringList skills = dal->qsl_getskills();
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

void MainWindow::on_actionNew_triggered()
{

    NewCharacterWizard wizard(dal);
    const int result = wizard.exec();
    if (result == QDialog::Accepted){
        if(m_dirtyDataFlag == true){ //if data is dirty, allow user to escape out.
            if(QMessageBox::Cancel==QMessageBox::information(this, tr("Closing Character Profile"), "Warning: All unsaved progress will be lost. Continue?",QMessageBox::Yes|QMessageBox::Cancel)){
                return;
            }
        }
        qDebug() << "Accepted: getting character.";
       curCharacter = wizard.getCharacter();
       m_dirtyDataFlag = true;
       ui->character_name_label->setVisible(true);
       ui->tabWidget->setVisible(true);
       ui->actionGenerate_Character_Sheet->setEnabled(true);
       ui->status_groupBox->setVisible(true);

       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::populateUI(){

    //-------------SET Personal notes and NAME ----------------------------
    ui->character_name_label->setVisible(true);
    ui->character_name_label->setText(curCharacter.family + " " + curCharacter.name + ", " + curCharacter.school);
    ui->name_lineEdit->setText(curCharacter.name);
    ui->family_label->setText(curCharacter.family);
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
    const QStringList skills = dal->qsl_getskills(); //get all skills
    //initialise skillranks
    foreach (const QString skill, skills) {
        curCharacter.skillranks[skill] = 0;
    }
    const QStringList rings = dal->qsl_getrings(); //get all rings
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
    dal->qsm_getschoolcurriculum(&curriculummodel, curCharacter.school);
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
            QStringList track = dal->qsl_gettitletrack(title);
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
        titleXPcounts << dal->qs_gettitlexp(title).toInt();
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
    curCharacter.abilities << dal->qsl_getschoolability(curCharacter.school);
    abiltext+=dal->qsl_getschoolability(curCharacter.school).at(0) + ", ";
    if(curCharacter.rank > 5) {
        curCharacter.abilities << dal->qsl_getschoolmastery(curCharacter.school);
        abiltext+= dal->qsl_getschoolmastery(curCharacter.school).at(0)+", ";
    }
    foreach (const QString title, curCharacter.titles) {
        if(title != curTitle) { //there should be no way to get a title that isn't curtitle without finishing it.  So the extras are fininshed - get abilities
                curCharacter.abilities << dal->qsl_gettitlemastery(title);
                const QStringList abl = dal->qsl_gettitlemastery(title);
                if(abl.count()>0){
                    abiltext+=abl.at(0)+", ";
                }
        }
    }
    if(abiltext.count()>=2) abiltext.chop(2); //trim the last ", "
    ui->ability_label->setText(abiltext);
    //-------------------SET RINGS ---------------------------
    QString ringtext = "";
    QMap <QString, int> ringmap = curCharacter.baserings; //get base values
    QMapIterator<QString, int> i(curCharacter.ringranks);
    while (i.hasNext()) {
        i.next();
        ringmap[i.key()] += i.value();                //add 'Air' (etc) for each extra
    }
    ringtext += "Air "+ QString::number(ringmap["Air"]) + ", ";
    ringtext += "Earth "+ QString::number(ringmap["Earth"]) + ", ";
    ringtext += "Fire "+ QString::number(ringmap["Fire"]) + ", ";
    ringtext += "Water "+ QString::number(ringmap["Water"]) + ", ";
    ringtext += "Void "+ QString::number(ringmap["Void"]);

    ui->ring_label->setText(ringtext);
    ui->ringWidget->setRings(ringmap);

    //------------------SET SKILL TABLE AND VALUES -----------------
    skillmodel.clear();
    QString skilltext = "";
    const QStringList skillgrouplist = dal->qsl_getskillsandgroup();
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

    //---------------CALCULATE DERIVED STATS ------------------------------//
    ui->endurance_label->setText(QString::number(( curCharacter.baserings["Earth"]
                                                +  curCharacter.ringranks["Earth"]
                                                +  curCharacter.baserings["Fire"]
                                                +  curCharacter.ringranks["Fire"])*2));
    ui->composure_label->setText(QString::number(( curCharacter.baserings["Earth"]
                                                 + curCharacter.ringranks["Earth"]
                                                 + curCharacter.baserings["Water"]
                                                 + curCharacter.ringranks["Water"])*2));
    ui->focus_label->setText(QString::number( curCharacter.baserings["Fire"]
                                            + curCharacter.ringranks["Fire"]
                                            + curCharacter.baserings["Air"]
                                            + curCharacter.ringranks["Air"]  ));
    ui->vigilance_label->setText(
                QString::number(qRound(double(curCharacter.baserings["Water"]
                                            + curCharacter.ringranks["Water"]
                                            + curCharacter.baserings["Air"]
                                            + curCharacter.ringranks["Air"])/2.0))); //round up, because the FAQ was cruel.

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
        foreach (const QString t, dal->qsl_gettechbyname(str)){
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
            foreach (const QString t, dal->qsl_gettechbyname(itemrow.at(1)->text())){ //grab a qstringlist...
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

        const QStringList dis_advdata = dal->qsl_getadvdisadvbyname(str);
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


void MainWindow::on_actionSave_As_triggered()
{
    qDebug()<<QString("Homepath = ") + QDir::homePath();
    QString cname = this->curCharacter.family + " " + curCharacter.name;
    if (cname.isEmpty())
        cname = "untitled";
    cname.remove(QRegExp("[^a-zA-Z\\d\\s]"));
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save File As..."), QDir::homePath()+"/"+cname+".pbc", tr("Paper Blossoms Character Profile (*.pbc)"));
    if (fileName.isEmpty())
        return;
    else
    {
        qDebug()<<QString("Filename = ") + fileName;

        QFile file(fileName);

        if (!file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QDataStream stream(&file);

        //SAVE_FILE_VERSION
        const int version = SAVE_FILE_VERSION;
        stream<<version;


        stream<<curCharacter.name;
        stream<<curCharacter.titles;
        stream<<curCharacter.clan;
        stream<<curCharacter.family;
        stream<<curCharacter.school;
        stream<<curCharacter.ninjo;
        stream<<curCharacter.giri;
        stream<<curCharacter.baseskills;
        stream<<curCharacter.baserings;
        stream<<curCharacter.ringranks;
        stream<<curCharacter.honor;
        stream<<curCharacter.glory;
        stream<<curCharacter.status;
        stream<<curCharacter.koku;
        stream<<curCharacter.bu;
        stream<<curCharacter.zeni;
        stream<<curCharacter.rank;
        stream<<curCharacter.techniques;
        stream<<curCharacter.adv_disadv;
        stream<<curCharacter.equipment;
        stream<<curCharacter.abilities;
        stream<<curCharacter.heritage;
        stream<<curCharacter.notes;
        stream<<curCharacter.advanceStack;
        stream<<curCharacter.portrait;
        stream<<curCharacter.totalXP;

        file.close();
        m_dirtyDataFlag = false; //reset to false (just saved!)
    }


}

void MainWindow::on_actionOpen_triggered()
{
    const QString fileName = QFileDialog::getOpenFileName( this, tr("Load..."), QDir::homePath(), tr("Paper Blossoms Character (*.pbc);;Any (*)"));
    if (fileName.isEmpty())
        return;
    else
    {
        if(m_dirtyDataFlag == true){ //if data is dirty, allow user to escape out.
            if(QMessageBox::Cancel==QMessageBox::information(this, tr("Opening Profile"), "Warning: This action will lose all unsaved progress. Continue?",QMessageBox::Yes|QMessageBox::Cancel)){
                return;
            }
        }
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
        QDataStream stream(&file);


        //first, reinitialize data
        curCharacter.clear();

        //VERSION------------------
        int version = -1;
        stream>>version;
        if(version<MIN_FILE_VERSION || version > MAX_FILE_VERSION){
            QMessageBox::information(this, tr("Incompatible Save File"), "This save file was created with an incompatible version of Paper Blossoms. Aborting import.");
            return;
        }
        //CHARACTER-----------------

        stream>>                  curCharacter.name         ;
        stream>>                  curCharacter.titles       ;
        stream>>                  curCharacter.clan         ;
        stream>>                  curCharacter.family       ;
        stream>>                  curCharacter.school       ;
        stream>>                  curCharacter.ninjo        ;
        stream>>                  curCharacter.giri         ;
        stream>>                  curCharacter.baseskills   ;
        stream>>                  curCharacter.baserings    ;
        stream>>                  curCharacter.ringranks    ;
        stream>>                  curCharacter.honor        ;
        stream>>                  curCharacter.glory        ;
        stream>>                  curCharacter.status       ;
        stream>>                  curCharacter.koku         ;
        stream>>                  curCharacter.bu           ;
        stream>>                  curCharacter.zeni         ;
        stream>>                  curCharacter.rank         ;
        stream>>                  curCharacter.techniques   ;
        stream>>                  curCharacter.adv_disadv   ;
        stream>>                  curCharacter.equipment    ;
        stream>>                  curCharacter.abilities    ;
        stream>>                  curCharacter.heritage     ;
        stream>>                  curCharacter.notes        ;
        stream>>                  curCharacter.advanceStack ;
        stream>>                  curCharacter.portrait     ;
        stream>>                  curCharacter.totalXP      ;

        file.close();
    }
    populateUI();
    ui->character_name_label->setVisible(true);
    ui->tabWidget->setVisible(true);
    ui->actionGenerate_Character_Sheet->setEnabled(true);
    ui->status_groupBox->setVisible(true);


    m_dirtyDataFlag = false; //just loaded!
}



void MainWindow::on_addadvance_button_clicked()
{
    AddAdvanceDialog addadvancedialog(dal, &curCharacter);
    const int result = addadvancedialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting advance.";
       m_dirtyDataFlag = true;
       curCharacter.advanceStack.append(addadvancedialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_remove_pushButton_clicked()
{
    if(curCharacter.advanceStack.count()>0){
        //curCharacter.advanceStack.pop_back();

        if(!ui->advance_tableView->currentIndex().isValid()) return;
        const int row = ui->advance_tableView->currentIndex().row();
        curCharacter.advanceStack.removeAt(row); //TODO: TESTING -- is this accurate?
        populateUI();
        m_dirtyDataFlag = true;

    }
}

QPair<int, int> MainWindow::recalcRank(){
    //   advheaders << "Type"<<"Advance"<<"Track"<<"Cost";
    int curricXP = 0;
    int rank = 1;

    foreach (const QString advance, curCharacter.advanceStack) {
        QStringList itemrow;
        foreach(const QString cell, advance.split("|")){
            itemrow << cell;
        }
        if(itemrow.at(2)=="Curriculum"){
            if(isInCurriculum(itemrow.at(1),itemrow.at(0), rank)){
                curricXP += itemrow.at(3).toInt();
            }
            else{
                curricXP += qRound(double(itemrow.at(3).toInt())/2.0);
            }
        }
        switch(rank){ //chart from page 98
        case 1:
            if (curricXP >= 20){
                rank++;
                curricXP = 0;
            }
            break;
        case 2:
            if (curricXP >= 24){
                rank++;
                curricXP = 0;
            }
            break;
        case 3:
            if (curricXP >= 32){
                rank++;
                curricXP = 0;
            }
            break;
        case 4:
            if (curricXP >= 44){
                rank++;
                curricXP = 0;
            }
            break;
        case 5:
            if (curricXP >= 60){
                rank++;
                curricXP = 0;
            }
            break;
        default:
            break;
        }
    }
    return QPair<int,int>(rank,curricXP);
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

int MainWindow::isInCurriculum(const QString value, const QString type, const int currank){
    //   advheaders << "Type"<<"Advance"<<"Track"<<"Cost";

    QStringList skills;
    QStringList techniques;


    for(int i = 0; i<curriculummodel.rowCount(); ++i){
        const QSqlRecord record = curriculummodel.record(i);
        //if(record.value("rank").toInt()!=curCharacter.rank) continue; //only get items in current rank;
        if(record.value("rank").toInt()!=currank) continue; //only get items in current rank;
        if(record.value("type").toString() == "skill_group"){
            QStringList groupskills = dal->qsl_getskillsbygroup(record.value("advance").toString());
            skills.append(groupskills);
        }
        else if (record.value("type").toString() == "skill"){
            skills << record.value("advance").toString();
        }
        else if(record.value("type").toString() == "technique"){
            techniques << record.value("advance").toString();
        }
        else if(record.value("type").toString() == "technique_group"){
            QStringList grouptech = dal->qsl_gettechbygroup(record.value("advance").toString(), curCharacter.rank);
            techniques.append(grouptech);
        }

        if(type == "Skill"){
            if (skills.contains(value)) return true;
        }
        else if (type == "Technique"){
            if (techniques.contains(value)) return true;
        }

    }
    return false;
}

int MainWindow::isInTitle(const QString value, const QString adv_type, const QString title){
    //   advheaders << "Type"<<"Advance"<<"Track"<<"Cost";

    QStringList skills;
    QStringList techniques;
    QStringList rings;


    for(int i = 0; i<titlemodel.rowCount(); ++i){
        const QString titlename = titlemodel.item(i,0)->text();
        const QString advance = titlemodel.item(i,1)->text();
        const QString type = titlemodel.item(i,2)->text();
        const QString spec_acc = titlemodel.item(i,3)->text();
        const QString rank = titlemodel.item(i,4)->text();
        if(titlename!=title) continue; //only get items in current title;
        if(type == "skill_group"){
            const QStringList groupskills = dal->qsl_getskillsbygroup(advance);
            skills.append(groupskills);
        }
        else if (type == "skill"){
            skills << advance;
        }
        else if(type == "technique"){
            techniques << advance;
        }
        else if(type == "technique_group"){
            const QStringList grouptech = dal->qsl_gettechbygroup(advance, rank.toInt()); //special -- uses title rank
            techniques.append(grouptech);
        }
        else if(type == "ring"){
            rings << advance;
        }

        if(adv_type == "Skill"){
            if (skills.contains(value)) return true;
        }
        else if (adv_type == "Technique"){
            if (techniques.contains(value)) return true;
        }
        else if (adv_type == "Ring"){ // dunno if this can ever be true, but prepping for ishikin
            if (rings.contains(value)) return true;
        }

    }
    return false;
}

void MainWindow::on_addTitle_pushButton_clicked()
{
    AddTitleDialog addtitledialog(dal, &curCharacter);
    const int result = addtitledialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting title.";
       m_dirtyDataFlag = true;
       curCharacter.titles.append(addtitledialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_koku_spinBox_valueChanged(const int arg1)
{
   curCharacter.koku = arg1;
   m_dirtyDataFlag = true;

}

void MainWindow::on_bu_spinBox_valueChanged(const int arg1)
{
   curCharacter.bu = arg1 ;
   m_dirtyDataFlag = true;

}

void MainWindow::on_zeni_spinBox_valueChanged(const int arg1)
{
    curCharacter.zeni = arg1;
   m_dirtyDataFlag = true;

}


void MainWindow::on_add_weapon_pushButton_clicked()
{
    AddItemDialog additemdialog(dal, &curCharacter,"Weapon");
    const int result = additemdialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting item";
       m_dirtyDataFlag = true;
       curCharacter.equipment.append(additemdialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_add_armor_pushButton_clicked()
{
    AddItemDialog additemdialog(dal, &curCharacter,"Armor");
    const int result = additemdialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting item";
       m_dirtyDataFlag = true;
       curCharacter.equipment.append(additemdialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_add_other_pushButton_clicked()
{
    AddItemDialog additemdialog(dal, &curCharacter,"Other");
    const int result = additemdialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting item";
       m_dirtyDataFlag = true;
       curCharacter.equipment.append(additemdialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_name_lineEdit_textChanged(const QString &arg1)
{
   curCharacter.name = arg1;
   ui->character_name_label->setText(curCharacter.family + " " + curCharacter.name + ", " + curCharacter.school);
   m_dirtyDataFlag = true;

}

void MainWindow::on_image_label_clicked()
{
    //QString selfilter = tr("JPEG (*.jpg *.jpeg)");
    const QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            QDir::homePath(),
            tr("All files (*.*);;JPEG (*.jpg *.jpeg);;PNG (*.png)" )
            //&selfilter
    );

    //QString fileName = QFileDialog::getOpenFileName(this,
    //                                                tr("Open File"), QDir::currentPath(),);
    if (!fileName.isEmpty()) {
        curCharacter.portrait.load(fileName);
        if (curCharacter.portrait.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        const QPixmap p = QPixmap::fromImage(curCharacter.portrait);
        ui->image_label->setPixmap(p.scaled(ui->image_label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));


        ui->image_label->adjustSize();
    }
    m_dirtyDataFlag = true;
}

void MainWindow::on_addDistinction_pushButton_clicked()
{
    AddDisAdvDialog adddisadvdialog(dal, &curCharacter,"Distinctions");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting distrinction";
       m_dirtyDataFlag = true;
       curCharacter.adv_disadv.append(adddisadvdialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_addPassion_pushButton_clicked()
{
    AddDisAdvDialog adddisadvdialog(dal, &curCharacter,"Passions");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting distrinction";
       m_dirtyDataFlag = true;
       curCharacter.adv_disadv.append(adddisadvdialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_addAdversity_pushButton_clicked()
{
    AddDisAdvDialog adddisadvdialog(dal, &curCharacter,"Adversities");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting distrinction";
       m_dirtyDataFlag = true;
       curCharacter.adv_disadv.append(adddisadvdialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_addAnxiety_pushButton_clicked()
{
    AddDisAdvDialog adddisadvdialog(dal, &curCharacter,"Anxieties");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted: getting distrinction";
       m_dirtyDataFlag = true;
       curCharacter.adv_disadv.append(adddisadvdialog.getResult());
       populateUI();
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_remDistinction_pushButton_clicked()
{
    QModelIndex curIndex = distinctionsProxyModel.mapToSource(ui->distinctions_tableView->currentIndex());
    if(!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(),Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_remPassion_pushButton_clicked()
{
    QModelIndex curIndex = passionsProxyModel.mapToSource(ui->passions_tableView->currentIndex());
    if(!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(),Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_remAdversity_pushButton_clicked()
{
    QModelIndex curIndex = adversitiesProxyModel.mapToSource(ui->adversities_tableView->currentIndex());
    if(!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(),Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_remAnxiety_pushButton_clicked()
{
    QModelIndex curIndex = anxietiesProxyModel.mapToSource(ui->anxieties_tableView->currentIndex());
    if(!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(),Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_actionGenerate_Character_Sheet_triggered()
{
    PBOutputData charData;

    charData.name = curCharacter.name;
    charData.family = curCharacter.family;
    charData.titles = curCharacter.titles;
    charData.clan = curCharacter.clan;
    charData.school = curCharacter.school;
    charData.ninjo = curCharacter.ninjo;
    charData.giri = curCharacter.giri;
    charData.abilities = curCharacter.abilities;

    //get skills
    for(int r = 0; r < skillmodel.rowCount();++r){
            QStringList row;
            for(int c = 0; c < skillmodel.columnCount();++c){
                    //if(!skillmodel.item(r,c)->text().isEmpty()){
                            row<< skillmodel.item(r,c)->text();
                    //}
            }
            charData.skills << row;
    }
    ui->ringWidget->setBackgroundWhite();
    charData.rings = ui->ringWidget->grab().toImage();
    ui->ringWidget->setBackgroundClear();
    charData.honor = QString::number(curCharacter.honor);
    charData.glory = QString::number(curCharacter.glory);
    charData.status = QString::number(curCharacter.status);
    charData.koku = QString::number(curCharacter.koku);
    charData.bu = QString::number(curCharacter.bu);
    charData.zeni = QString::number(curCharacter.zeni);
    charData.focus = ui->focus_label->text();
    charData.vigilance = ui->vigilance_label->text();
    charData.endurance = ui->endurance_label->text();
    charData.composure = ui->composure_label->text();
    charData.curricStatus = ui->curric_status_label->text();
    charData.titleStatus = ui->title_status_label->text();
    for(int r = 0; r < curriculummodel.rowCount();++r){
            QStringList row;
            for(int c = 0; c < curriculummodel.columnCount();++c){
                    //if(!skillmodel.item(r,c)->text().isEmpty()){
                            row<< curriculummodel.record(r).value(c).toString();
                    //}
            }
            charData.curriculum << row;
    }
    for(int r = 0; r < titlemodel.rowCount();++r){
            QStringList row;
            for(int c = 0; c < titlemodel.columnCount();++c){
                    //if(!titlemodel.item(r,c)->text().isEmpty()){
                            row<< titlemodel.item(r,c)->text();
                    //}
            }
            if(row.at(Title::SOURCE) == this->incompleteTitle) charData.curTitle << row;
    }
    for(int r = 0; r < techModel.rowCount();++r){
            QStringList row;
            for(int c = 0; c < techModel.columnCount();++c){
                    if(techModel.item(r,c)!= NULL){
                            row<< techModel.item(r,c)->text();
                    }
            }
            charData.techniques << row;
    }
    for(int r = 0; r < dis_advmodel.rowCount();++r){
            QStringList row;
            for(int c = 0; c < dis_advmodel.columnCount();++c){
                    //if(!skillmodel.item(r,c)->text().isEmpty()){
                            row<< dis_advmodel.item(r,c)->text();
                    //}
            }
            if(row.at(Adv_Disadv::TYPE) == "Distinctions")
                    charData.distinctions << row;
            if(row.at(Adv_Disadv::TYPE) == "Adversities")
                    charData.adversities << row;
            if(row.at(Adv_Disadv::TYPE) == "Passions")
                    charData.passions << row;
            if(row.at(Adv_Disadv::TYPE) == "Anxieties")
                    charData.anxieties << row;

    }
    for(int r = 0; r < equipmodel.rowCount();++r){
            QStringList row;
            for(int c = 0; c < equipmodel.columnCount();++c){
                    //if(!skillmodel.item(r,c)->text().isEmpty()){
                            row<< equipmodel.item(r,c)->text();
                    //}
            }
            if(row.at(Equipment::TYPE) == "Weapon")
                    charData.weapons << row;
            else if(row.at(Equipment::TYPE) == "Armor")
                    charData.armor << row;
            else
                    charData.personaleffects << row;

    }
    charData.heritage = curCharacter.heritage;
    charData.advanceStack = curCharacter.advanceStack;
    charData.notes = curCharacter.notes;
    charData.portrait = curCharacter.portrait;

    RenderDialog renderdlg(&charData);
    const int result = renderdlg.exec();
    if (result == QDialog::Accepted){
        qDebug() << "Accepted received from RenderDialog";
    }
    else{
        qDebug() << "Not accepted; discarding changes.";
    }

}

void MainWindow::on_ninjo_textEdit_textChanged()
{
   curCharacter.ninjo = ui->ninjo_textEdit->toPlainText();
   m_dirtyDataFlag = true;
}

void MainWindow::on_giri_textEdit_textChanged()
{
   curCharacter.giri = ui->giri_textEdit->toPlainText();
   m_dirtyDataFlag = true;
}

void MainWindow::on_notes_textEdit_textChanged()
{
   curCharacter.notes = ui->notes_textEdit->toPlainText();
   m_dirtyDataFlag = true;
}

void MainWindow::on_xpSpinBox_valueChanged(const int arg1)
{
   curCharacter.totalXP = arg1;
   m_dirtyDataFlag = true;
}

void MainWindow::on_glory_spinBox_valueChanged(const int arg1)
{
   curCharacter.glory = arg1;
   m_dirtyDataFlag = true;
}

void MainWindow::on_honor_spinBox_valueChanged(const int arg1)
{
    curCharacter.honor = arg1;
   m_dirtyDataFlag = true;
}

void MainWindow::on_status_spinBox_valueChanged(const int arg1)
{
    curCharacter.status = arg1;
   m_dirtyDataFlag = true;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog abtdlg;
    abtdlg.exec();
}

void MainWindow::on_actionExport_User_Tables_triggered()
{
    qDebug()<<QString("Homepath = ") + QDir::homePath();
    const QString fileName = QFileDialog::getExistingDirectory( this, tr("Choose an export directory..."), QDir::homePath());
    if (fileName.isEmpty())
        return;
    else
    {
        bool success = true;
        foreach(QString tablename, dal->user_tables){
            success &= dal->queryToCsv(fileName,tablename);
        }
        if(!success){
            QMessageBox::information(this, tr("Error Exporting Data"), "One or more errors occured exporting user data. Please check write permissions in the target folder.");

        }
        else{
            QMessageBox::information(this, tr("Export Complete"), "User data export completed.");

        }

    }
}

void MainWindow::on_actionImport_User_Data_Tables_triggered()
{
    QMessageBox::information(this, tr("Warning"), "This feature is in beta; it will attempt to import a folder containing exported user data files.  Errors importing could cause inconsistencies in the data. Proceed with caution. If you experience difficulty, remove the local copy of the DB and relaunch Paper Blossoms.");
    const QString fileName = QFileDialog::getExistingDirectory( this, tr("Choose a folder containing exported data..."), QDir::homePath());
    if (fileName.isEmpty())
        return;
    else
    {
        bool success = true;
        foreach(QString tablename, dal->user_tables){
        //dal->queryToCsv(fileName,tablename);
            success &= dal->importCSV(fileName, tablename);
        }
        if(!success){
            QMessageBox::information(this, tr("Error Importing Data"), "Data import encoutered one or more errors. Your database may be inconsistent or incomplete. Recommend restoring default DB.");

        }
        else{
            QMessageBox::information(this, tr("Import Complete"), "User data import completed. This feature is in beta; please verify that your data still functions normally.");

        }
    }
}

void MainWindow::on_actionOpen_Application_Data_Directory_triggered()
{
    const QUrl url("file:///"+QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDesktopServices::openUrl(  url );
}

void MainWindow::on_actionExit_triggered()
{
    if(m_dirtyDataFlag == true){ //if data is dirty, allow user to escape out.
        if(QMessageBox::Cancel==QMessageBox::information(this, tr("Closing Character Profile"), "Warning: All unsaved progress will be lost. Continue?",QMessageBox::Yes|QMessageBox::Cancel)){
            return ;
        }
        //exit(0);
    }
        QApplication::quit();
}

void MainWindow::closeEvent (QCloseEvent * const event)
{
    if(m_dirtyDataFlag == true){ //if data is dirty, allow user to escape out.
        if(QMessageBox::Cancel==QMessageBox::information(this, tr("Closing Character Profile"), "Warning: All unsaved progress will be lost. Continue?",QMessageBox::Yes|QMessageBox::Cancel)){
            event->ignore(); ;
        }
    }
    else{
        event->accept();
    }
}

void MainWindow::on_removeweapon_pushbutton_clicked()
{
    const QModelIndex curIndex = weaponProxyModel.mapToSource(ui->weapon_tableview->currentIndex());
    if(!curIndex.isValid()) return;
    const QString name = equipmodel.item(curIndex.row(),Equipment::NAME)->text();
    curCharacter.equipment.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_removearmor_pushbutton_clicked()
{
    const QModelIndex curIndex = armorProxyModel.mapToSource(ui->armor_tableview->currentIndex());
    if(!curIndex.isValid()) return;
    const QString name = equipmodel.item(curIndex.row(),Equipment::NAME)->text();
    curCharacter.equipment.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_removepersonaleffect_pushbutton_clicked()
{
    const QModelIndex curIndex = perseffProxyModel.mapToSource(ui->other_tableview->currentIndex());
    if(!curIndex.isValid()) return;
    const QString name = equipmodel.item(curIndex.row(),Equipment::NAME)->text();
    curCharacter.equipment.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}


