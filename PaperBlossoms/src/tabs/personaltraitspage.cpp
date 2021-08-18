#include "../mainwindow.h"
#include "../enums.h"
#include "../dialog/adddisadvdialog.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>

void MainWindow::on_addDistinction_pushButton_clicked() {
    AddDisAdvDialog adddisadvdialog(deps, &curCharacter, "Distinctions");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting distrinction";
        m_dirtyDataFlag = true;
        curCharacter.adv_disadv.append(adddisadvdialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_addPassion_pushButton_clicked() {
    AddDisAdvDialog adddisadvdialog(deps, &curCharacter, "Passions");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting distrinction";
        m_dirtyDataFlag = true;
        curCharacter.adv_disadv.append(adddisadvdialog.getResult());
        curCharacter.advanceStack.append("Passion|" + adddisadvdialog.getResult() + "|Curriculum|" + "3");
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_addAdversity_pushButton_clicked() {
    AddDisAdvDialog adddisadvdialog(deps, &curCharacter, "Adversities");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting distrinction";
        m_dirtyDataFlag = true;
        curCharacter.adv_disadv.append(adddisadvdialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_addAnxiety_pushButton_clicked() {
    AddDisAdvDialog adddisadvdialog(deps, &curCharacter, "Anxieties");
    const int result = adddisadvdialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting distrinction";
        m_dirtyDataFlag = true;
        curCharacter.adv_disadv.append(adddisadvdialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_remDistinction_pushButton_clicked() {
    QModelIndex curIndex = distinctionsProxyModel.mapToSource(ui->distinctions_tableView->currentIndex());
    if (!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(), Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_remPassion_pushButton_clicked() {
    QModelIndex curIndex = passionsProxyModel.mapToSource(ui->passions_tableView->currentIndex());
    if (!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(), Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_remAdversity_pushButton_clicked() {
    QModelIndex curIndex = adversitiesProxyModel.mapToSource(ui->adversities_tableView->currentIndex());
    if (!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(), Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_remAnxiety_pushButton_clicked() {
    QModelIndex curIndex = anxietiesProxyModel.mapToSource(ui->anxieties_tableView->currentIndex());
    if (!curIndex.isValid()) return;
    QString name = dis_advmodel.item(curIndex.row(), Adv_Disadv::NAME)->text();
    curCharacter.adv_disadv.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}
