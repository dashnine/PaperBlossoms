#include "../mainwindow.h"
#include "../characterwizard/newcharacterwizard.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>

void MainWindow::on_actionNew_triggered() {

    NewCharacterWizard wizard(deps);
    const int result = wizard.exec();
    if (result == QDialog::Accepted) {
        if (m_dirtyDataFlag == true) { //if data is dirty, allow user to escape out.
            if (QMessageBox::Cancel == QMessageBox::information(this, tr("Closing Character Profile"),
                                                                "Warning: All unsaved progress will be lost. Continue?",
                                                                QMessageBox::Yes | QMessageBox::Cancel)) {
                return;
            }
        }
        qDebug() << "Accepted: getting character.";
        curCharacter = wizard.getCharacter();
        m_dirtyDataFlag = true;
        ui->character_name_label->setVisible(true);
        ui->tabWidget->setVisible(true);
        ui->actionExport_to_XML->setEnabled(true);
        ui->actionSave_As->setEnabled(true);

        ui->actionGenerate_Character_Sheet->setEnabled(true);
        ui->status_groupBox->setVisible(true);

        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_actionSave_As_triggered() {
    QString settingfile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/settings.ini";
    QSettings settings(settingfile, QSettings::IniFormat);
    //QSettings settings;
    QString filepath = QDir::homePath();
    //if(!settings.contains("savefilepath")){
    QString path = settings.value("savefilepath").toString();
    if (!path.isEmpty()) {
        if (QFileInfo::exists(path)) filepath = path;
    }
    //}


    qDebug() << QString("Homepath = ") + QDir::homePath();
    QString cname = this->curCharacter.family + " " + curCharacter.name;
    if (cname.isEmpty())
        cname = "untitled";
    cname.remove(QRegularExpression("[^a-zA-Z\\d\\s]"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As..."), filepath + "/" + cname + ".pbc",
                                                    tr("Paper Blossoms Character Profile (*.pbc)"));
    if (fileName.isEmpty())
        return;
    else {
        qDebug() << QString("Filename = ") + fileName;

        QFile file(fileName);

        if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QDataStream stream(&file);

        //SAVE_FILE_VERSION
        const int version = SAVE_FILE_VERSION;
        stream << version;

        //V 3 fields
        stream << curCharacter.bonds;

        //V 2 fields
        stream << curLocale;

        //v1 fields
        stream << curCharacter.name;
        stream << curCharacter.titles;
        stream << curCharacter.clan;
        stream << curCharacter.family;
        stream << curCharacter.school;
        stream << curCharacter.ninjo;
        stream << curCharacter.giri;
        stream << curCharacter.baseskills;
        stream << curCharacter.baserings;
        stream << curCharacter.ringranks;
        stream << curCharacter.honor;
        stream << curCharacter.glory;
        stream << curCharacter.status;
        stream << curCharacter.koku;
        stream << curCharacter.bu;
        stream << curCharacter.zeni;
        stream << curCharacter.rank;
        stream << curCharacter.techniques;
        stream << curCharacter.adv_disadv;
        stream << curCharacter.equipment;
        stream << curCharacter.abilities;
        stream << curCharacter.heritage;
        stream << curCharacter.notes;
        stream << curCharacter.advanceStack;
        stream << curCharacter.portrait;
        stream << curCharacter.totalXP;

        file.close();
        QFileInfo fi(fileName);
        settings.setValue("savefilepath", fi.canonicalPath());
        settings.sync();
        m_dirtyDataFlag = false; //reset to false (just saved!)
    }


}

void MainWindow::on_actionOpen_triggered() {
    QString settingfile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/settings.ini";
    QSettings settings(settingfile, QSettings::IniFormat);
    //QSettings settings;
    QString filepath = QDir::homePath();
    //if(!settings.contains("savefilepath")){
    QString path = settings.value("savefilepath").toString();
    if (!path.isEmpty()) {
        if (QFileInfo::exists(path)) filepath = path;
    }
    //}

    const QString fileName = QFileDialog::getOpenFileName(this, tr("Load..."), filepath,
                                                          tr("Paper Blossoms Character (*.pbc);;Any (*)"));
    if (fileName.isEmpty())
        return;
    else {
        if (m_dirtyDataFlag == true) { //if data is dirty, allow user to escape out.
            if (QMessageBox::Cancel == QMessageBox::information(this, tr("Opening Profile"),
                                                                "Warning: This action will lose all unsaved progress. Continue?",
                                                                QMessageBox::Yes | QMessageBox::Cancel)) {
                return;
            }
        }
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
        QDataStream stream(&file);


        //first, reinitialize data
        curCharacter.clear();

        //VERSION------------------
        int version = -1;
        stream >> version;
        if (version < MIN_FILE_VERSION || version > MAX_FILE_VERSION) {
            QMessageBox::information(this, tr("Incompatible Save File"),
                                     tr("This save file was created with an incompatible version of Paper Blossoms. Aborting import."));
            return;
        }

        //BONDS------------------- (v3)
        if (version < 3) { //need to default the locale to en
            //nothing to stream in on v1-v2 files: no bond support
            curCharacter.bonds.clear();
            qDebug() << "Old save file: no bonds to import.";
        } else {
            stream >> curCharacter.bonds;
        }

        //LOCALE------------------- (v2)
        QString filelocale = "";
        if (version < 2) { //need to default the locale to en
            //nothing to stream in on v1 save files - all of them were EN
            filelocale = "en";
        } else {
            stream >> filelocale;
        }
        if (filelocale != curLocale) {
            QMessageBox::information(this, tr("Incompatible Locale"),
                                     tr("This save file was created with a different locale (") + filelocale + "). " +
                                     tr("Aborting import. To load this save file, you can change your DB locale to ") +
                                     filelocale +
                                     tr(" in ") + settingfile + tr(" and relaunch the application."));
            return;
        }

        //CHARACTER----------------- (v1)

        stream >> curCharacter.name;
        stream >> curCharacter.titles;
        stream >> curCharacter.clan;
        stream >> curCharacter.family;
        stream >> curCharacter.school;
        stream >> curCharacter.ninjo;
        stream >> curCharacter.giri;
        stream >> curCharacter.baseskills;
        stream >> curCharacter.baserings;
        stream >> curCharacter.ringranks;
        stream >> curCharacter.honor;
        stream >> curCharacter.glory;
        stream >> curCharacter.status;
        stream >> curCharacter.koku;
        stream >> curCharacter.bu;
        stream >> curCharacter.zeni;
        stream >> curCharacter.rank;
        stream >> curCharacter.techniques;
        stream >> curCharacter.adv_disadv;
        stream >> curCharacter.equipment;
        stream >> curCharacter.abilities;
        stream >> curCharacter.heritage;
        stream >> curCharacter.notes;
        stream >> curCharacter.advanceStack;
        stream >> curCharacter.portrait;
        stream >> curCharacter.totalXP;

        file.close();
        QFileInfo fi(fileName);
        settings.setValue("savefilepath", fi.canonicalPath());
        settings.sync();
    }
    populateUI();
    ui->character_name_label->setVisible(true);
    ui->tabWidget->setVisible(true);
    ui->actionExport_to_XML->setEnabled(true);
    ui->actionSave_As->setEnabled(true);

    ui->actionGenerate_Character_Sheet->setEnabled(true);
    ui->status_groupBox->setVisible(true);

    m_dirtyDataFlag = false; //just loaded!
}

void MainWindow::closeEvent(QCloseEvent *const event) {
    if (m_dirtyDataFlag == true) { //if data is dirty, allow user to escape out.
        if (QMessageBox::Cancel == QMessageBox::information(this, tr("Closing Character Profile"),
                                                            "Warning: All unsaved progress will be lost. Continue?",
                                                            QMessageBox::Yes | QMessageBox::Cancel)) {
            event->ignore();;
        }
    } else {
        event->accept();
    }
}
