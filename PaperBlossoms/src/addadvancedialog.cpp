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

#include "addadvancedialog.h"
#include "ui_addadvancedialog.h"
#include  "dataaccesslayer.h"
#include <QPushButton>
#include <QSqlRecord>
#include <QDebug>
#include "enums.h"

AddAdvanceDialog::AddAdvanceDialog(DataAccessLayer* dal, Character* character, QString sel, QString option, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAdvanceDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/resources/sakura.png"));
    ui->advtype->setCurrentIndex(-1);
    this->dal = dal;
    this->character = character;
    ui->detailTableView->setVisible(false);
    ui->maho_label->setVisible(false);
    removerestrictions = false;

    proxyModel.setDynamicSortFilter(true);
    proxyModel.setSourceModel(&techModel);
    proxyModel.setFilterKeyColumn(1);

    ui->reason_label->setVisible(false);
    ui->reason_lineEdit->setVisible(false);


    ui->detailTableView->setModel(&proxyModel);







    validatePage();

    populateTechModel();


    if(sel == "skill"){
        ui->advtype->setCurrentText(tr("Skill"));
        ui->advchooser_combobox->setCurrentIndex(-1);
        ui->advchooser_combobox->setCurrentText(option);
    }
    else if(sel == "skill_group"){
        ui->advtype->setCurrentText(tr("Skill"));
        ui->advchooser_combobox->setCurrentIndex(-1);
        //no way to specify a group, so right now default to blank)
    }
    else if (sel == "technique"){



        //TODOOOOOOOO  - Handle auto-picking techniques based on a double-click.
        ui->advchooser_combobox->setCurrentIndex(-1);
        //ui->advchooser_combobox->setCurrentText(category);



        /*
        ui->advtype->setCurrentText(tr("Technique"));
        if(!option.isEmpty()) {
            QString category = dal->qs_gettechtypebyname(option);
            ui->advchooser_combobox->setCurrentIndex(-1);
            ui->advchooser_combobox->setCurrentText(category);
            //ui->detailTableView->selectRow(techModel.f)
            for(int i = 0; i < proxyModel.rowCount(); ++i){
                QModelIndex index = proxyModel.mapToSource(proxyModel.index(i,0));
                QSqlRecord r = techModel.record(index.row());
                QString name = r.value("name_tr").toString();
                if (name == option) {
                    ui->detailTableView->selectRow(i);
                    on_detailTableView_clicked(proxyModel.index(i,0));
                }
            }
        }
        */
    }
    else if (sel == "technique_group"){
        ui->advtype->setCurrentText(tr("Technique"));
        if(!option.isEmpty()) {
            QString category = dal->qs_gettechtypebygroupname(option);
            ui->advchooser_combobox->setCurrentIndex(-1);
            ui->advchooser_combobox->setCurrentText(category);

        }
    }

}

AddAdvanceDialog::~AddAdvanceDialog()
{
    delete ui;
}

void AddAdvanceDialog::validatePage(){
    ui->warnlabel->setText("");
    bool ok = true;
    ok &= !ui->advtype->currentText().isEmpty();
    ok &= !ui->advchooser_combobox->currentText().isEmpty();
    if(ui->advtype->currentText() == tr("Technique")){
        ok &= ui->detailTableView->currentIndex().isValid();

        if(ok){


            /*
            const QModelIndex curIndex = proxyModel.mapToSource(ui->detailTableView->currentIndex());
            const QSqlRecord record = techModel.record(curIndex.row());
            const QString name =  record.value("name_tr").toString();
            */

            const QModelIndex curIndex = proxyModel.mapToSource(ui->detailTableView->currentIndex());
            if(!curIndex.isValid()) return;
            QString name = techModel.item(curIndex.row(),TechQuery::NAME)->text();


            if(character->techniques.contains(name)){
                ui->warnlabel->setText("Invalid selection: '"+name+"' is already learned.");
            }
            else{
                //also check advances
                foreach (const QString advance, character->advanceStack) {   //iterate through advances
                    QList<QStandardItem*> itemrow;                       //generate a row in the advancestack
                    foreach(const QString cell, advance.split("|")){           // the advance table is pipe separated for now.  FIx later?
                        itemrow << new QStandardItem(cell);              // turn this into qstandarditems to match other paradigms
                    }
                    if(itemrow.at(0)->text() == "Technique"){            //if it's a tech advance
                        if((itemrow.at(1)->text() == name)){
                            ui->warnlabel->setText("Invalid selection: '"+name+"' is already learned.");
                            ok = false;
                        }
                    }
                }

            }

            ok &= !character->techniques.contains(name);
        }
    }
    if(ok && !ui->free_radioButton->isChecked()){
        //if everything but radio buttons are set, show XP cost unless free is checked
        ui->xp_label->setVisible(true);
        ui->xp_text_label->setVisible(true);
        ui->xp_text_label2->setVisible(true);

    }
    else{
        ui->xp_label->setVisible(false);
        ui->xp_text_label->setVisible(false);
        ui->xp_text_label2->setVisible(false);

    }


    ok &= (ui->curriculum_radioButton->isChecked() || ui->title_radioButton->isChecked() || ui->free_radioButton->isChecked());
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
       //this->window()->adjustSize();


}

void AddAdvanceDialog::populateTechModel(){
    techModel.clear();

   /*


    //---------------------Special access groups and katagroups from title-------------------//
    "(rank <= ? and subcategory in                                                              " //0 trank
    " (SELECT name from title_advancements                                                      "
    "   WHERE title_tr = ?                                                                         " //1 title
    "   AND type = 'technique_group'                                                            "
    "   AND special_access = 1                                                                  "
    "  )  )                                                                                     "
    "OR                                                                                         " //title Katas (cat v subcat)
    "(rank <= ? and category in                                                                 " //2 trank //cat is group
    " (SELECT name from title_advancements                                                      "
    "   WHERE title_tr = ?                                                                         " //3 title
    "   and type = 'technique_group'                                                            "
    "   AND special_access = 1                                                                  "
    "  ) )                                                                                      "
    //----------------------Special access groups and katagroups from curriculum---------------//
    "OR ( rank <= ? and subcategory in                                                          " //4 rank
    " (SELECT advance from curriculum                                                           "
    "   WHERE school_tr = ?                                                                        " //5 school //subcat is group
    "   AND rank = ? and type = 'technique_group'                                               " //6 rank
    "   AND special_access = 1                                                                  "
    " )                                                                                         "
    "OR  rank <= ? and category in                                                              " //7 rank
    " (SELECT advance from curriculum                                                           "
    "   WHERE school_tr = ?                                                                        " //8 school //subcat is group
    "   AND rank = ? and type = 'technique_group'                                               " //9 rank
    "   AND special_access = 1                                                                  "
    " )  )                                                                                      "
    //------------------------special access indiv tech from curric and title------------------//
                                                                               "
    "OR name_tr in (                                                                               "           //title tech
    "  SELECT name_tr from title_advancements                                                      "
    "   WHERE title_tr = ?                                                                         " //12 title
    "   AND type = 'technique'                                                                  "
    "   AND special_access = 1                                                                  "
    "  )                                                                                        "


    */




    //get full
    QList<QStringList> techlist = dal->ql_getalltechniques();

    //const QString rank, const QString school, const QString title, const bool norestriction
    int rank = character->rank;
    QList<QStringList> titletrack;
    if(character->titles.count()>0) {
        titletrack = dal->ql_gettitletrack(character->titles.last());
    }
    QStringList schooltech = dal->qsl_gettechallowedbyschool(character->school);
    QList<QStringList> curriculum = dal->qsl_getschoolcurriculum(character->school);

    foreach(const QStringList tech, techlist){

        //if it's unrestricted, call it a day
        if(removerestrictions){
            addTechRow(tech);
            continue;
        }



        //now, check this row against filters.  If it is allowable, add the row.

        //IF it is less than or equal to my rank, and....
        if(rank >= tech.at(TechQuery::RANK).toInt()){
            //category or subcategory is in my school?
            if(schooltech.contains(tech.at(TechQuery::CATEGORY))) {
                addTechRow(tech);
                continue;
            }
            if(schooltech.contains(tech.at(TechQuery::SUBCATEGORY))) {
                addTechRow(tech);
                continue;
            }

            //get tech that everyone has access to:
            if( (tech.at(TechQuery::CATEGORY)) == "Mahō"                ||
                    (tech.at(TechQuery::CATEGORY)) == "Item Patterns"       ||
                    (tech.at(TechQuery::CATEGORY)) == "Signature Scrolls"   ){
                addTechRow(tech);
                continue;

            }
        }

        //check your curriculum for this rank only, looking for special access
        bool isincurric = false;
        foreach(QStringList curricrow, curriculum){
            if((curricrow.at(Curric::RANK).toInt() == rank) && curricrow.at(Curric::SPEC).toInt()==1){
                if(curricrow.at(Curric::ADVANCE) == tech.at(TechQuery::CATEGORY)) {
                    addTechRow(tech);
                    isincurric = true;
                    break; //break back to main loop
                }
                if(curricrow.at(Curric::ADVANCE) == tech.at(TechQuery::SUBCATEGORY)) {
                    isincurric = true;
                    addTechRow(tech);
                    break; //break back to main loop
                }
                if(curricrow.at(Curric::ADVANCE) == tech.at(TechQuery::NAME)){
                    isincurric = true;
                    addTechRow(tech);
                    break; //break back to main loop
                }
            }
        }
        if(isincurric) continue; //next tech if we found it
        //now do the same for title
        bool isintitle = false;
        foreach(QStringList titlerow, titletrack){


            if(     (titlerow.at(Title::TRANK).isEmpty()||(tech.at(TechQuery::RANK)<=titlerow.at(Title::TRANK)))
                     && ( titlerow.at(Title::SPEC).toInt()==1) ){
                if(titlerow.at(Title::ADVANCE) == tech.at(TechQuery::CATEGORY)) {
                    addTechRow(tech);
                    isintitle = true;
                    break; //break back to main loop
                }
                if(titlerow.at(Title::ADVANCE) == tech.at(TechQuery::SUBCATEGORY)) {
                    addTechRow(tech);
                    isintitle = true;
                    break; //break back to main loop
                }
                if(titlerow.at(Title::ADVANCE) == tech.at(TechQuery::NAME)){
                    addTechRow(tech);
                    isintitle = true;
                    break; //break back to main loop
                }
            }
        }
        if(isintitle) continue; //next tech if we found it
    }





















        //if it gets here, discard it
        //addTechRow(tech);

}

void AddAdvanceDialog::addTechRow(QStringList tech){
    QList<QStandardItem*> itemrow;
    foreach (const QString t, tech){
        //now, do the real work for the tables
        itemrow << new QStandardItem(t);
    }
    techModel.appendRow(itemrow);
}

void AddAdvanceDialog::on_advtype_currentIndexChanged(const QString &arg1)
{
    if(arg1 == tr("Skill")){
        ui->advchooser_combobox->clear();
        QStringList skillsopts = dal->qsl_getskills();
        ui->detailTableView->setVisible(false);
        //TODO - filter to allowable skills
        int maxrank = 5;
        QMapIterator<QString, int> j(character->baseskills);
        while (j.hasNext()) {
            j.next();
            const int value = j.value()+character->skillranks[j.key()];
            if(value>=maxrank){
                skillsopts.removeAll(j.key());
            }
        }
        ui->advchooser_combobox->addItems(skillsopts);


    }
    else if (arg1 == tr("Technique")){
        ui->advchooser_combobox->clear();

        //get a list of types that can be chosen at this time
        QSet<QString> types;
        if(character->titles.count()==0){
            //dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, "", removerestrictions);
            populateTechModel();

        }
        else{


            //dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, character->titles.last(), removerestrictions);
            populateTechModel();
       }
        for(int i = 0; i<techModel.rowCount(); ++i){
            /*
            const QSqlRecord record = techModel.record(i);
            types << record.value("Category").toString();
            */
            for(int r = 0; r < techModel.rowCount();++r){
                    QStringList row;
                    for(int c = 0; c < techModel.columnCount();++c){
                            //if(!titlemodel.item(r,c)->text().isEmpty()){
                                    row<< techModel.item(r,c)->text();
                            //}
                    }
                    types << row.at(TechQuery::CATEGORY);
            }
        }
        qDebug()<< types;
        QStringList typelist;
        foreach(QString str, types){
            typelist<<str;
        }

        qSort(typelist);
        ui->advchooser_combobox->addItems(typelist);

        //tech cost is variable now -- hide the text and clear it
        ui->xp_label->setText(QString::number(0));
        ui->xp_label->setVisible(false);
        ui->xp_text_label->setVisible(false);
        ui->xp_text_label2->setVisible(false);

        ui->detailTableView->resizeColumnsToContents();
        ui->detailTableView->setVisible(true);
    }
    else if (arg1 == tr("Ring")){
        ui->advchooser_combobox->clear();
        QStringList rings = (dal->qsl_getrings());
        ui->detailTableView->setVisible(false);
        //TODO - filter to allowable rings


        QMapIterator<QString, int> ringiterator(character->baserings);
        int voidring = 0;
        QString lowestring = "";
        int lowestval = 99;
        while (ringiterator.hasNext()) {
            ringiterator.next();
            if(!ringiterator.key().isEmpty()){
                int value = ringiterator.value()+character->ringranks[ringiterator.key()];
                if(value < lowestval) {
                    if(ringiterator.key() != dal->translate("Void")){
                        lowestring = ringiterator.key();
                        lowestval = value;
                    }
                }
                if(ringiterator.key() == dal->translate("Void")){ //void
                    voidring = value;
                }
            }
        }

        int maxrank = lowestval + voidring;
        QMapIterator<QString, int> j(character->baserings);
        while (j.hasNext()) {
            j.next();
            int value = j.value()+character->ringranks[j.key()];
            if(value>=maxrank || value >= 5){
                rings.removeAll(j.key());
            }
        }
        ui->advchooser_combobox->addItems(rings);

    }
    else{
        ui->detailTableView->setVisible(false);
    }
    validatePage();
}

void AddAdvanceDialog::on_advchooser_combobox_currentIndexChanged(const QString &arg1)
{



    if(ui->advtype->currentText() == tr("Skill")){
        const int currentrank = character->baseskills[arg1] + character->skillranks[arg1];
        const int cost = (currentrank+1)*2;
        const int rounded = qRound(double(cost)/2.0);
        ui->xp_label->setText(QString::number((ui->halfxp_checkBox->isChecked()?rounded:cost)));
    }
    if(ui->advtype->currentText() == tr("Ring")){
        const int currentrank = character->baserings[arg1] + character->ringranks[arg1];
        const int cost = (currentrank+1)*3;
        const int rounded = qRound(double(cost)/2.0);
        ui->xp_label->setText(QString::number((ui->halfxp_checkBox->isChecked()?rounded:cost)));
    }
    else{
        //QSet<QString> types;
        if(character->titles.count()==0){
            //dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, "", removerestrictions);
            populateTechModel();

        }
        else{
            //dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, character->titles.last(), removerestrictions);
            populateTechModel();
       }
        // for(int i = 0; i<techModel.rowCount(); ++i){
       //     QSqlRecord record = techModel.record(i);
       //     types << record.value("Category").toString();
        //}
        //qDebug()<< types;
        proxyModel.setFilterFixedString(arg1);
        if(arg1 == "Mahō"){
            ui->maho_label->setVisible(true);
        }
        else{
            ui->maho_label->setVisible((false));
        }


    }
    validatePage();
}

void AddAdvanceDialog::on_curriculum_radioButton_clicked()
{
   validatePage();
}

void AddAdvanceDialog::on_title_radioButton_clicked()
{
    validatePage();
}

QString AddAdvanceDialog::getResult() const {
    QString row = "";
    if(ui->advtype->currentText() == tr("Skill")){
        row+="Skill|";
    }
    else if (ui->advtype->currentText() == tr("Technique")){
        row += "Technique|";
    }
    else{
        row += "Ring|";
    }

    //row += ui->advtype->currentText() + "|";
    if(ui->advtype->currentText() == tr("Technique")){
       const QModelIndex curIndex = proxyModel.mapToSource(ui->detailTableView->currentIndex());
       //const QSqlRecord record = techModel.record(curIndex.row());
       //row += record.value("name_tr").toString()+"|";
               QStringList tr;
               for(int c = 0; c < techModel.columnCount();++c){
                       //if(!titlemodel.item(r,c)->text().isEmpty()){
                               tr<< techModel.item(curIndex.row(),c)->text();
                       //}
               }
               row += tr.at(TechQuery::NAME);
    }
    else{
        row += ui->advchooser_combobox->currentText() + "|";
    }
    if(ui->curriculum_radioButton->isChecked())
        row += "Curriculum|";
    else if(ui->free_radioButton->isChecked()){
        row += ui->reason_lineEdit->text().replace("|","").replace("Title","").replace("Curriculum","")+"|";
    }
    else
        row += "Title|";
    if(ui->free_radioButton->isChecked()){
        row+="0";
    }
    else{
        row += ui->xp_label->text();
    }
    return row;
}

void AddAdvanceDialog::on_detailTableView_clicked(const QModelIndex &index)
{
    //Q_UNUSED(index)

    QModelIndex curIndex = proxyModel.mapToSource(index);
    /*
    QSqlRecord record = techModel.curIndex.row());
    const int cost = record.value("xp").toInt();
    */
    const int cost = techModel.item(curIndex.row(),TechQuery::XP)->text().toInt();
    const int rounded = qRound(double(cost)/2.0);
    const QString text = QString::number(ui->halfxp_checkBox->isChecked()?rounded:cost);

    ui->xp_label->setText(text);

    validatePage();
}


void AddAdvanceDialog::on_free_radioButton_toggled(const bool checked)
{
   if(checked){
       ui->reason_label->setVisible(true);
       ui->reason_lineEdit->setVisible(true);
       ui->xp_label->setVisible(false);
       ui->xp_text_label->setVisible(false);
       ui->xp_text_label2->setVisible(false);
   }
   else{
       ui->reason_label->setVisible(false);
       ui->reason_lineEdit->setVisible(false);
       ui->xp_label->setVisible(true);
       ui->xp_text_label->setVisible(false);
       ui->xp_text_label2->setVisible(true);

   }
   validatePage();
}

void AddAdvanceDialog::on_halfxp_checkBox_toggled(bool checked)
{
    Q_UNUSED(checked);
    on_detailTableView_clicked(ui->detailTableView->currentIndex());
    on_advchooser_combobox_currentIndexChanged(ui->advchooser_combobox->currentText());
    validatePage();
}

void AddAdvanceDialog::on_restrictioncheckBox_toggled(bool checked)
{
    removerestrictions = checked;
    ui->advtype->setCurrentIndex(-1);
    ui->advchooser_combobox->setCurrentIndex(-1);

}
