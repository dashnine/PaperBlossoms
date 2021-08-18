#include "../mainwindow.h"
#include "../dialog/dblocalisationeditordialog.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

void MainWindow::on_actionExport_Translation_CSV_triggered() {
    bool success = false;
    qDebug() << QString("Homepath = ") + QDir::homePath();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Translation File..."),
                                                    QDir::homePath() + "/tr.csv", tr("CSV (*.csv)"));
    if (fileName.isEmpty())
        return;
    else {
        qDebug() << QString("Filename = ") + fileName;
        success = deps->dal->exportTranslatableCSV(fileName);
    }
    if (!success) {
        QMessageBox::information(this, tr("Error exporting translation data"),
                                 "Data export encoutered one or more errors. Your file may be corrupt or incomplete.");

    } else {
        QMessageBox::information(this, tr("Translation Template Export Complete"),
                                 "User data export completed. This file can be comitted to the Paper Blossoms github to update your localisation.");

    }

}

void MainWindow::on_actionTranslate_For_Locale_triggered() {
    DBLocalisationEditorDialog dialog(deps);
    const int result = dialog.exec();
    if (result == QDialog::Accepted) {
        dialog.doFinish(true);
        bool success = false;
        qDebug() << QString("Homepath = ") + QDir::homePath();

        QString fileName = QFileDialog::getSaveFileName(this, tr("Export Translation File..."),
                                                        QDir::homePath() + "/tr.csv", tr("CSV (*.csv)"));
        if (fileName.isEmpty())
            return;
        else {
            qDebug() << QString("Filename = ") + fileName;
            success = deps->dal->exportTranslatableCSV(fileName);
        }
        if (!success) {
            QMessageBox::information(this, tr("Error exporting translation data"),
                                     "Data export encoutered one or more errors. Your file may be corrupt or incomplete.");

        } else {
            QMessageBox::information(this, tr("Translation Template Export Complete"),
                                     "User data export completed. This file can be comitted to the Paper Blossoms github to update your localisation.");

        }


        qDebug() << "Accepted: data comitted";

    } else {
        dialog.doFinish(false);
        qDebug() << "Rejected: data discarded";
    }
}
