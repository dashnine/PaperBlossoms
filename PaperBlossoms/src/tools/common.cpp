#include "../mainwindow.h"
#include "../enums.h"
#include "../pboutputdata.h"
#include "../dialog/renderdialog.h"
#include "../dialog/aboutdialog.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QMessageBox>
#include <QSqlRecord>

void MainWindow::on_actionGenerate_Character_Sheet_triggered() {
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
    for (int r = 0; r < skillmodel.rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < skillmodel.columnCount(); ++c) {
            //if(!skillmodel.item(r,c)->text().isEmpty()){
            row << skillmodel.item(r, c)->text();
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
    for (int r = 0; r < curriculummodel.rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < curriculummodel.columnCount(); ++c) {
            //if(!skillmodel.item(r,c)->text().isEmpty()){
            row << curriculummodel.record(r).value(c).toString();
            //}
        }
        charData.curriculum << row;
    }
    for (int r = 0; r < titlemodel.rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < titlemodel.columnCount(); ++c) {
            //if(!titlemodel.item(r,c)->text().isEmpty()){
            row << titlemodel.item(r, c)->text();
            //}
        }
        if (row.at(Title::SOURCE) == this->incompleteTitle) charData.curTitle << row;
    }
    for (int r = 0; r < techModel.rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < techModel.columnCount(); ++c) {
            if (techModel.item(r, c) != NULL) {
                row << techModel.item(r, c)->text();
            }
        }
        charData.techniques << row;
    }
    for (int r = 0; r < dis_advmodel.rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < dis_advmodel.columnCount(); ++c) {
            //if(!skillmodel.item(r,c)->text().isEmpty()){
            row << dis_advmodel.item(r, c)->text();
            //}
        }
        if (row.at(Adv_Disadv::TYPE) == "Distinctions")
            charData.distinctions << row;
        if (row.at(Adv_Disadv::TYPE) == "Adversities")
            charData.adversities << row;
        if (row.at(Adv_Disadv::TYPE) == "Passions")
            charData.passions << row;
        if (row.at(Adv_Disadv::TYPE) == "Anxieties")
            charData.anxieties << row;

    }
    for (int r = 0; r < equipmodel.rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < equipmodel.columnCount(); ++c) {
            //if(!skillmodel.item(r,c)->text().isEmpty()){
            row << equipmodel.item(r, c)->text();
            //}
        }
        if (row.at(Equipment::TYPE) == "Weapon")
            charData.weapons << row;
        else if (row.at(Equipment::TYPE) == "Armor")
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
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted received from RenderDialog";
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }

}

void MainWindow::on_actionAbout_triggered() {
    AboutDialog abtdlg;
    abtdlg.exec();
}

void MainWindow::on_actionOpen_Application_Data_Directory_triggered() {
    const QUrl url("file:///" + QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionExit_triggered() {
    if (m_dirtyDataFlag == true) { //if data is dirty, allow user to escape out.
        if (QMessageBox::Cancel == QMessageBox::information(
                this,
                tr("Closing Character Profile"),
                "Warning: All unsaved progress will be lost. Continue?",
                QMessageBox::Yes | QMessageBox::Cancel)) {
            return;
        }
    }
    QApplication::quit();
}
