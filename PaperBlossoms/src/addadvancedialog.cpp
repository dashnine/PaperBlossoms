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

AddAdvanceDialog::AddAdvanceDialog(DataAccessLayer* dal, Character* character,QWidget *parent) :
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

    proxyModel.setDynamicSortFilter(true);
    proxyModel.setSourceModel(&techModel);
    proxyModel.setFilterKeyColumn(1);

    ui->reason_label->setVisible(false);
    ui->reason_lineEdit->setVisible(false);


    ui->detailTableView->setModel(&proxyModel);

    validatePage();
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
    if(ui->advtype->currentText() == "Technique"){
        ok &= ui->detailTableView->currentIndex().isValid();

        if(ok){
            const QModelIndex curIndex = proxyModel.mapToSource(ui->detailTableView->currentIndex());
            const QSqlRecord record = techModel.record(curIndex.row());
            const QString name =  record.value("name").toString();

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

void AddAdvanceDialog::on_advtype_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Skill"){
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
    else if (arg1 == "Technique"){
        ui->advchooser_combobox->clear();

        //get a list of types that can be chosen at this time
        QSet<QString> types;
        if(character->titles.count()==0){
            dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, "", character->clan);

        }
        else{


            dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, character->titles.last(), character->clan);
       }
        for(int i = 0; i<techModel.rowCount(); ++i){
            const QSqlRecord record = techModel.record(i);
            types << record.value("Category").toString();
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
    else if (arg1 == "Ring"){
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
                    if(ringiterator.key() != "Void"){
                        lowestring = ringiterator.key();
                        lowestval = value;
                    }
                }
                if(ringiterator.key() == "Void"){ //void
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

    }
    validatePage();
}

void AddAdvanceDialog::on_advchooser_combobox_currentIndexChanged(const QString &arg1)
{



    if(ui->advtype->currentText() == "Skill"){
        const int currentrank = character->baseskills[arg1] + character->skillranks[arg1];
        const int cost = (currentrank+1)*2;
        const int rounded = qRound(double(cost)/2.0);
        ui->xp_label->setText(QString::number((ui->halfxp_checkBox->isChecked()?rounded:cost)));
    }
    if(ui->advtype->currentText() == "Ring"){
        const int currentrank = character->baserings[arg1] + character->ringranks[arg1];
        const int cost = (currentrank+1)*3;
        const int rounded = qRound(double(cost)/2.0);
        ui->xp_label->setText(QString::number((ui->halfxp_checkBox->isChecked()?rounded:cost)));
    }
    else{
        //QSet<QString> types;
        if(character->titles.count()==0){
            dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, "", character->clan);

        }
        else{


            dal->qsm_gettechniquetable(&techModel, QString::number(character->rank),character->school, character->titles.last(), character->clan);
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
    row += ui->advtype->currentText() + "|";
    if(ui->advtype->currentText() == "Technique"){
       const QModelIndex curIndex = proxyModel.mapToSource(ui->detailTableView->currentIndex());
       const QSqlRecord record = techModel.record(curIndex.row());
       row += record.value("name").toString()+"|";
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
    Q_UNUSED(index)

    QModelIndex curIndex = proxyModel.mapToSource(index);
    QSqlRecord record = techModel.record(curIndex.row());
    const int cost = record.value("xp").toInt();
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
    on_detailTableView_clicked(ui->detailTableView->currentIndex());
    on_advchooser_combobox_currentIndexChanged(ui->advchooser_combobox->currentText());
    validatePage();
}
