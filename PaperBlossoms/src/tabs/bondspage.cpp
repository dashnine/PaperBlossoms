#include "../mainwindow.h"
#include "../dialog/addbonddialog.h"
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QMessageBox>
#include <QModelIndex>
#include <QDialog>
#include "ui_mainwindow.h"

void MainWindow::on_bondAdd_pushButton_clicked() {
    AddBondDialog addbonddialog(deps, &curCharacter, "Bonds");
    const int result = addbonddialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting bond";
        m_dirtyDataFlag = true;
        //TODO:SUPPORT BOND SAVING with a BONDMODEL
        curCharacter.bonds.append(addbonddialog.getResult());
        curCharacter.advanceStack.append("Bond|" + addbonddialog.getResult().first() + "|None|" + "3");
        //TODO: Refresh Bonds in UI
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_bondRemove_pushButton_clicked() {
    QModelIndex curIndex = ui->bonds_tableView->currentIndex();
    if (!curIndex.isValid()) return;
    //QString name = bondmodel.item(curIndex.row(),1)->text();
    curCharacter.bonds.removeAt(curIndex.row()); //TODO: TESTING -- is this accurate?
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_bondUpgrade_pushButton_clicked() {
    QModelIndex curIndex = ui->bonds_tableView->currentIndex();
    if (!curIndex.isValid()) return;

    QStringList bondrow = curCharacter.bonds.at(curIndex.row());
    int currank = bondrow.at(1).toInt();


    if (currank < 5) {
        int cost = 0; //cost of upgrade to next rank. Per page 190 PoW (also on CoS)
        switch (currank) {
            case 1:
                cost = 4;
                break;
            case 2:
                cost = 6;
                break;
            case 3:
                cost = 8;
                break;
            case 4:
                cost = 10;
                break;
            default:
                cost = 0;
                break;
        }
        if (QMessageBox::Cancel == QMessageBox::information(this, tr("Upgrading Bond"),
                                                            "This will spend " + QString::number(cost) +
                                                            " XP, and is not reversable. Continue?",
                                                            QMessageBox::Yes | QMessageBox::Cancel)) {
            return;
        }

        bondrow.replace(1, QString::number(++currank));
        curCharacter.bonds.replace(curIndex.row(), bondrow);
        curCharacter.advanceStack.append("Bond Upgrade|" + bondrow.first() + "|None|" + QString::number(cost));

    }


    populateUI();
    m_dirtyDataFlag = true;
}
