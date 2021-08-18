#include "../mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

void MainWindow::on_actionImport_User_Data_Tables_triggered() {
    QMessageBox::information(this, tr("Warning"),
                             "This feature is in beta; it will attempt to import a folder containing exported user data files.  Errors importing could cause inconsistencies in the data. Proceed with caution. If you experience difficulty, remove the local copy of the DB and relaunch Paper Blossoms.");
    const QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose a folder containing exported data..."),
                                                               QDir::homePath());
    if (fileName.isEmpty())
        return;
    else {
        bool success = true;
                foreach(QString tablename, deps->dal->user_tables) {
                //dal->queryToCsv(fileName,tablename);
                success &= deps->dal->importCSV(fileName, tablename);
            }
        if (!success) {
            QMessageBox::information(this, tr("Error Importing Data"),
                                     "Data import encoutered one or more errors. Your database may be inconsistent or incomplete. Recommend restoring default DB.");

        } else {
            QMessageBox::information(this, tr("Import Complete"),
                                     "User data import completed. This feature is in beta; please verify that your data still functions normally.");

        }
    }
}

void MainWindow::on_actionImport_User_Descriptions_Table_triggered() {
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Import User Descriptions..."), QDir::homePath(),
                                                          tr("CSV (*.csv);;Any (*)"));
    if (fileName.isEmpty())
        return;
    else {
        bool success = true;
        success &= deps->dal->importCSV(fileName, "user_descriptions", false);
        if (!success) {
            QMessageBox::information(this, tr("Error Importing Data"),
                                     "Data import encoutered one or more errors. Your database may be inconsistent or incomplete. Recommend restoring default DB.");

        } else {
            QMessageBox::information(this, tr("Import Complete"),
                                     "User data import completed. This feature is in beta; please verify that your data still functions normally.");

        }
    }

}
