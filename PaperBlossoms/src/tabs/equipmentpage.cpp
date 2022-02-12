#include "../mainwindow.h"
#include "../dialog/additemdialog.h"
#include "../enums.h"
#include "ui_mainwindow.h"
#include <QModelIndex>
#include <QString>
#include <QDialog>
#include <QDebug>

void MainWindow::on_removeweapon_pushbutton_clicked() {
    const QModelIndex curIndex = weaponProxyModel.mapToSource(ui->weapon_tableview->currentIndex());
    if (!curIndex.isValid()) return;
    const QString name = equipmodel.item(curIndex.row(), Equipment::NAME)->text();
    curCharacter.equipment.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_removearmor_pushbutton_clicked() {
    const QModelIndex curIndex = armorProxyModel.mapToSource(ui->armor_tableview->currentIndex());
    if (!curIndex.isValid()) return;
    const QString name = equipmodel.item(curIndex.row(), Equipment::NAME)->text();
    curCharacter.equipment.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_koku_spinBox_valueChanged(const int arg1) {
    curCharacter.koku = arg1;
    m_dirtyDataFlag = true;

}

void MainWindow::on_bu_spinBox_valueChanged(const int arg1) {
    curCharacter.bu = arg1;
    m_dirtyDataFlag = true;

}

void MainWindow::on_zeni_spinBox_valueChanged(const int arg1) {
    curCharacter.zeni = arg1;
    m_dirtyDataFlag = true;

}

void MainWindow::on_add_weapon_pushButton_clicked() {
    AddItemDialog additemdialog(deps, &curCharacter, "Weapon");
    const int result = additemdialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting item";
        m_dirtyDataFlag = true;
        curCharacter.equipment.append(additemdialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_add_armor_pushButton_clicked() {
    AddItemDialog additemdialog(deps, &curCharacter, "Armor");
    const int result = additemdialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting item";
        m_dirtyDataFlag = true;
        curCharacter.equipment.append(additemdialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_add_other_pushButton_clicked() {
    AddItemDialog additemdialog(deps, &curCharacter, "Other");
    const int result = additemdialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting item";
        m_dirtyDataFlag = true;
        curCharacter.equipment.append(additemdialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_removepersonaleffect_pushbutton_clicked() {
    const QModelIndex curIndex = perseffProxyModel.mapToSource(ui->other_tableview->currentIndex());
    if (!curIndex.isValid()) return;
    const QString name = equipmodel.item(curIndex.row(), Equipment::NAME)->text();
    curCharacter.equipment.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}