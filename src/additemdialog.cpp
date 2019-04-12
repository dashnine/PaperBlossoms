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

#include "additemdialog.h"
#include "ui_additemdialog.h"
#include "enums.h"
#include <QDebug>

AddItemDialog::AddItemDialog(DataAccessLayer* dal, Character* character, QString type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/resources/pink-sakura-01-hi.png"));
    this->dal = dal;
    this->character = character;
    this->type = type;

    if(type == "Weapon"){
        ui->weap_1_groupBox->setVisible(true);
        ui->weap_2_groupBox->setVisible(true);
        ui->armor_groupBox->setVisible(false);
    }
    else if (type == "Armor"){
        ui->weap_1_groupBox->setVisible(false);
        ui->weap_2_groupBox->setVisible(false);
        ui->armor_groupBox->setVisible(true);
    }
    else{
        ui->weap_1_groupBox->setVisible(false);
        ui->weap_2_groupBox->setVisible(false);
        ui->armor_groupBox->setVisible(false);
    }
    this->layout()->setSizeConstraint(QLayout::SetMinimumSize);
    this->adjustSize();
    ui->itemtemplate_combobox->addItems(dal->qsl_getitemsbytype(type));

    ui->qual_listView->setModel(&qualities);
    ui->qual_comboBox->addItems(dal->qsl_getqualities());
    ui->pattern_comboBox->addItems(dal->qsl_getpatterns());

}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}

void AddItemDialog::on_itemtemplate_combobox_currentIndexChanged(const QString &arg1)
{
    clearFields();
    // Populate base values
    QStringList basedata = dal->qsl_getbaseitemdata(arg1, type);
    ui->itemname_lineEdit->setText(basedata.at(ItemData::NAME));
    ui->desc_textEdit->setText(basedata.at(ItemData::DESCRIPTION));
    ui->shortdesc_lineEdit->setText(basedata.at(ItemData::SHORT_DESC));
    ui->book_lineEdit->setText(basedata.at(ItemData::REFERENCE_BOOK));
    ui->page_lineEdit->setText(basedata.at(ItemData::REFERENCE_PAGE));
    ui->price_spinbox->setValue( basedata.at(ItemData::PRICE_VALUE).toInt());
    ui->unit_comboBox->setCurrentText(basedata.at(ItemData::PRICE_UNIT));
    ui->rarity_spinBox->setValue( basedata.at(ItemData::RARITY).toInt());

    qualities.setStringList(dal->qsl_getitemqualities(arg1,type));
    //ui->qual_comboBox->addItems(dal->qsl_getqualities());

    if(type == "Weapon"){
        QList<QStringList> weapondata = dal->ql_getweapondata(arg1);
        bool didFirst = false;
        bool didSecond = false;
        foreach (QStringList gripdata, weapondata) {

            if(!didFirst){
                ui->grip1_category_comboBox->setCurrentText(gripdata.at(WeaponData::CATEGORY));
                ui->grip1_skill_comboBox->setCurrentText(gripdata.at(WeaponData::SKILL));
                ui->grip1_lineEdit->setText(gripdata.at(WeaponData::GRIP));
                ui->grip1_minrange_spinBox->setValue(gripdata.at(WeaponData::RANGE_MIN).toInt());
                ui->grip1_maxrange_spinBox->setValue(gripdata.at(WeaponData::RANGE_MAX).toInt());
                ui->grip1_dam_spinBox->setValue(gripdata.at(WeaponData::DAMAGE).toInt());
                ui->grip1_dls_spinBox->setValue(gripdata.at(WeaponData::DEADLINESS).toInt());
                didFirst = true;
            }
            else if(!didSecond){
                ui->grip2_category_comboBox->setCurrentText(gripdata.at(WeaponData::CATEGORY));
                ui->grip2_skill_comboBox->setCurrentText(gripdata.at(WeaponData::SKILL));
                ui->grip2_lineEdit->setText(gripdata.at(WeaponData::GRIP));
                ui->grip2_minrange_spinBox->setValue(gripdata.at(WeaponData::RANGE_MIN).toInt());
                ui->grip2_maxrange_spinBox->setValue(gripdata.at(WeaponData::RANGE_MAX).toInt());
                ui->grip2_dam_spinBox->setValue(gripdata.at(WeaponData::DAMAGE).toInt());
                ui->grip2_dls_spinBox->setValue(gripdata.at(WeaponData::DEADLINESS).toInt());
                didSecond = true;
            }
            else{
                qWarning() << "ERROR: too many grips.  Skipping remainder.";
            }
        }
    }
    else if (type == "Armor"){
        QList<QStringList> armordata = dal->ql_getarmordata(arg1);
        foreach (QStringList resistdata, armordata) {
            if(resistdata.at(ArmorData::RESIST_CATEGORY) == "Physical")
                ui->armor_phys_spinBox->setValue(resistdata.at(ArmorData::RESIST_VALUE).toInt());
            else if (resistdata.at(ArmorData::RESIST_CATEGORY) == "Supernatural")
                ui->armor_supernatural_spinBox->setValue(resistdata.at(ArmorData::RESIST_VALUE).toInt());
            else{
                qWarning() << "ERROR: " + resistdata.at(ArmorData::RESIST_CATEGORY) + " is not Physical or Supernatural. Skipping.";
            }
        }
    }
}

void AddItemDialog::clearFields(){
    ui->itemname_lineEdit->setText("");
    ui->desc_textEdit->setText("");
    ui->shortdesc_lineEdit->setText("");
    ui->book_lineEdit->setText("");
    ui->page_lineEdit->setText("");
    ui->price_spinbox->setValue(0);
    ui->unit_comboBox->setCurrentIndex(-1);
    ui->rarity_spinBox->setValue(0);

    qualities.setStringList(QStringList{});
    //ui->qual_comboBox->clear();

    ui->grip1_category_comboBox->setCurrentIndex(-1);
    ui->grip1_skill_comboBox->setCurrentIndex(-1);
    ui->grip1_lineEdit->setText("");
    ui->grip1_minrange_spinBox->setValue(0);
    ui->grip1_maxrange_spinBox->setValue(0);
    ui->grip1_dam_spinBox->setValue(0);
    ui->grip1_dls_spinBox->setValue(0);

    ui->grip2_category_comboBox->setCurrentIndex(-1);
    ui->grip2_skill_comboBox->setCurrentIndex(-1);
    ui->grip2_lineEdit->setText("");
    ui->grip2_minrange_spinBox->setValue(0);
    ui->grip2_maxrange_spinBox->setValue(0);
    ui->grip2_dam_spinBox->setValue(0);
    ui->grip2_dls_spinBox->setValue(0);

    ui->armor_phys_spinBox->setValue(0);
    ui->armor_supernatural_spinBox->setValue(0);
}

QList<QStringList> AddItemDialog::getResult(){
    QList<QStringList> out;
    QStringList row;



    row << type;                                //FILTERCOLUMN              //0
    row <<  ui->itemname_lineEdit->text();                                  //
    row <<  ui->desc_textEdit->toPlainText();                               //
    row <<  ui->shortdesc_lineEdit->text();                                 //
    row <<  ui->book_lineEdit->text();                                      //
    row <<  ui->page_lineEdit->text();
    row <<  QString::number(ui->price_spinbox->value());
    row << ui->unit_comboBox->currentText();
    row << QString::number(ui->rarity_spinBox->value());

    QString qualitiestr = "";
    foreach (QString str, qualities.stringList()) {
        qualitiestr += str + " ";
    }
    row << qualitiestr;

    row << ui->grip1_category_comboBox->currentText();
    row << ui->grip1_skill_comboBox->currentText();
    row << ui->grip1_lineEdit->text();
    row << QString::number(ui->grip1_minrange_spinBox->value());
    row << QString::number(ui->grip1_maxrange_spinBox->value());
    row << QString::number(ui->grip1_dam_spinBox->value());
    row << QString::number(ui->grip1_dls_spinBox->value());

    row << QString::number(ui->armor_phys_spinBox->value());
    row << QString::number(ui->armor_supernatural_spinBox->value());

    out << row;

    if(!ui->grip2_lineEdit->text().isEmpty()){
        row.clear();

        row << type;                                //FILTERCOLUMN
        row <<  ui->itemname_lineEdit->text();
        row <<  ui->desc_textEdit->toPlainText();
        row <<  ui->shortdesc_lineEdit->text();
        row <<  ui->book_lineEdit->text();
        row <<  ui->page_lineEdit->text();
        row <<  QString::number(ui->price_spinbox->value());
        row << ui->unit_comboBox->currentText();
        row << QString::number(ui->rarity_spinBox->value());

        qualitiestr = "";
        foreach (QString str, qualities.stringList()) {
            qualitiestr += str + " ";
        }
        row << qualitiestr;

        row << ui->grip2_category_comboBox->currentText();
        row << ui->grip2_skill_comboBox->currentText();
        row << ui->grip2_lineEdit->text();
        row << QString::number(ui->grip2_minrange_spinBox->value());
        row << QString::number(ui->grip2_maxrange_spinBox->value());
        row << QString::number(ui->grip2_dam_spinBox->value());
        row << QString::number(ui->grip2_dls_spinBox->value());

        row << QString::number(ui->armor_phys_spinBox->value());
        row << QString::number(ui->armor_supernatural_spinBox->value());
        out << row;
    }
    return out;



}



void AddItemDialog::on_qual_add_pushButton_clicked()
{
    QStringList curlist = qualities.stringList();
    QString newqual = ui->qual_comboBox->currentText();
    if(!newqual.isEmpty() && !curlist.contains(newqual)) curlist.append(newqual);
    qualities.setStringList(curlist);
}

void AddItemDialog::on_qual_rem_pushButton_clicked()
{
    QModelIndex index = ui->qual_listView->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();


    QStringList curlist = qualities.stringList();
    curlist.removeAll(itemText);
    qualities.setStringList(curlist);
}

void AddItemDialog::on_pattern_add_pushButton_clicked()
{
    QStringList curlist = qualities.stringList();
    QString newqual = ui->pattern_comboBox->currentText();
    if(!newqual.isEmpty() && !curlist.contains(newqual)) curlist.append(newqual);
    qualities.setStringList(curlist);
}
