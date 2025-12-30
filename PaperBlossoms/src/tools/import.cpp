#include "../mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QtCore/QTextStream>

#undef slots
#undef _DEBUG
#include <Python.h>

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


void outputError(MainWindow* obj, const char* errorStr)
{
    if (PyErr_Occurred())
        PyErr_Print();
    QMessageBox::information(obj, obj->tr("Error"), QStringLiteral("There was a fatal error attempting to import; application may be incorrectly installed: ") + errorStr);
}

void MainWindow::on_actionImport_Official_PDF_triggered() {
    QMessageBox::information(this, tr("Warning"),
                             "This feature is in beta; it will attempt to import official English PDF sourcebooks obtained from DriveThruRPG.");
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Choose Sourcebook"), QDir::homePath(),
                                                          tr("PDF (*.pdf);;Any (*)"));
    if (fileName.isEmpty())
        return;

    if (!this->pythonInitialized)
    {
        this->pythonInitialized = true;
        Py_Initialize();
    }

    PyObject* pModule = PyImport_Import(PyUnicode_DecodeFSDefault("scrape"));
    PyObject* pFunc = NULL;
    if (pModule == NULL)
    {
        outputError(this, "Couldn't get module.");
        return;
    }
    pFunc = PyObject_GetAttrString(pModule, "handle_file");
    if (pFunc == NULL || !PyCallable_Check(pFunc))
    {
        outputError(this, "Couldn't get function.");
    }
    PyObject* pArgs = Py_BuildValue("(s)", qUtf8Printable(fileName));
    PyObject* pRet = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    if (pRet == NULL || PyErr_Occurred())
    {
        Py_XDECREF(pRet);
        outputError(this, "Call failed.");
        return;
    }
    const char* file_id = NULL;
    int blurb_count = -1;
    const char* data_file = NULL;
    PyArg_Parse(pRet, "(sis)", &file_id, &blurb_count, &data_file);

    if (PyErr_Occurred() || file_id == NULL)
    {
        outputError(this, "Trouble parsing.");
        return;
    }
    if (strcmp(file_id, "") == 0)
    {
        QMessageBox::information(this, tr("Read failure"), QStringLiteral("Did not recognize any sourcebook in file ") + fileName);
    }
    else if (blurb_count <= 0)
    {
        QMessageBox::information(this, tr("Read failure"), QStringLiteral("Recognized sourcebook ") + file_id + ", but recovered no descriptions.");
    }
    else
    {
        QString message = QStringLiteral("Successfully read file; contents match sourcebook '%1'.\n%2 entries extracted.").arg(file_id).arg(blurb_count);
        int recordsAdded = deps->dal->importCSVFlexible(data_file, "user_descriptions", false, false, false, "name", 0);
        if (recordsAdded >= 0)
        {
            message += QStringLiteral("\n%1 results written to database.").arg(recordsAdded);
            if (recordsAdded < blurb_count)
            {
                message += QStringLiteral("\nYou may overwrite the remaining %1 records that already exist. Would you like to overwrite?").arg(blurb_count - recordsAdded);
                if (QMessageBox::Yes == QMessageBox::question(this, tr("Overwrite?"), message))
                {
                    recordsAdded = deps->dal->importCSVFlexible(data_file, "user_descriptions", false, false, true, "name", 0);
                    QMessageBox::information(this, tr("Success"), QStringLiteral("%1/%2 available records written.").arg(recordsAdded).arg(blurb_count));
                }
            }
            else
            {
                QMessageBox::information(this, tr("Success"), message);
            }
        }
        else
        {
            message += QStringLiteral("\nCould not modify database. Results in %1.").arg(data_file);
            QMessageBox::information(this, tr("Partial Success"), message);
        }
    }
}
