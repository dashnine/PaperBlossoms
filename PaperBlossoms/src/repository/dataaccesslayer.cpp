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

#include "dataaccesslayer.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSqlRecord>
#include <QDir>
#include <QSqlTableModel>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtCore/QTextCodec>
#else
    #include <QtCore5Compat/QTextCodec>
#endif


DataAccessLayer::DataAccessLayer(QString locale) {
    QString targetpath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(targetpath).exists()) {
        QDir().mkdir(targetpath);
    }
    const QString appdir = QCoreApplication::applicationDirPath();
    targetpath += "/paperblossoms.db";
    qDebug() << targetpath;
    //check filemodtime
    const QFileInfo ri(":/data/paperblossoms.db");
    //QFileInfo ri(appdir + "/paperblossoms.db");
    const QFileInfo fi(targetpath);

    qDebug() << "resource: " + ri.lastModified().toString();
    qDebug() << "localfile: " + fi.lastModified().toString();
    //copy db to standardpaths

    if (ri.lastModified() > fi.lastModified()) {
        QMessageBox msgBox;
        msgBox.setText("The local data is missing or older than the bundled data.");
        msgBox.setInformativeText("Do you want to overwrite local data?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        const int ret = msgBox.exec();

        if (ret == QMessageBox::Yes) {
            bool dbexists = QFile::exists(targetpath);


            if (!QFile::remove(targetpath) && dbexists) {
                QMessageBox msgBox;
                msgBox.setText("Error");
                msgBox.setInformativeText(
                        "Unable to remove the old data. To remove old data, manually delete paperblossoms.db.");
                msgBox.setStandardButtons(QMessageBox::Yes);
                msgBox.setDefaultButton(QMessageBox::Yes);
                msgBox.exec();
            }
        }
    }

    //implicitly fails if the file already exists at the target!
    QFile::copy(":/data/paperblossoms.db", targetpath);
    QFile::setPermissions(targetpath, QFile::WriteOwner | QFile::ReadOwner);
    //QFile::copy(appdir + "/paperblossoms.db", targetpath);



    //connect to DB

    const QString DRIVER("QSQLITE");
    if (QSqlDatabase::isDriverAvailable(DRIVER)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        //db.setDatabaseName(":memory:");
        //db.setDatabaseName("testdb.db");
        //db.setDatabaseName("paperblossoms.db");
        db.setDatabaseName(targetpath);


        if (!db.open())
            qWarning() << "ERROR: " << db.lastError();
    }

    //import translation table for locale (if possible)
    importCSV(":/translations/data/i18n/i18n_" + locale + ".csv", "i18n", false);
    //:/translations/data/i18n/i18n_en.csv

}

QString DataAccessLayer::untranslate(QString string_tr) {
    QSqlQuery query;
    query.prepare("SELECT string FROM i18n WHERE string_tr = ?");
    query.bindValue(0, string_tr);
    query.exec();
    while (query.next()) {
        QString out = query.value(0).toString();
        return out;
    }
    qDebug() << "Untranslated value for " + string_tr + " not found. Using original.";
    return string_tr;
}

QString DataAccessLayer::translate(QString string) {
    QSqlQuery query;
    query.prepare("SELECT string_tr FROM i18n WHERE string = ?");
    query.bindValue(0, string);
    query.exec();
    while (query.next()) {
        QString out = query.value(0).toString();
        if (!out.isEmpty()) return out;
    }
    //qDebug() << "Translated value for "+ string +" not found. Using original.";
    return string; //fall through to here if there are no populated transated values, and return the original value rather than the empty str
}

QStringList DataAccessLayer::qsl_getdescribablenames() {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "           select name                    FROM advantages_disadvantages       "
            "UNION      SELECT name                    FROM armor                          "
            "UNION      SELECT name                    FROM clans                          "
            "UNION      SELECT name                    FROM families                       "
            "UNION      SELECT name                    FROM personal_effects               "
            "UNION      SELECT quality                 FROM qualities                      "
            "UNION      SELECT name                    FROM schools                        "
            "UNION      SELECT school_ability_name     FROM schools                        "
            "UNION      SELECT mastery_ability_name    FROM schools                        "
            "UNION      SELECT name                    FROM techniques                     "
            "UNION      SELECT name                    FROM titles                         "
            "UNION      SELECT title_ability_name      FROM titles                         "
            "UNION      SELECT name                    FROM weapons                        "

    );
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        out << name;
    }
    return out;
}

void DataAccessLayer::qsm_gettranslationmodel(QSqlQueryModel *const model) {

    QSqlQuery query;
    query.prepare(translationquery
    );
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        //        qDebug() << cname;
    }
    model->setQuery(query);

}

QList <QStringList> DataAccessLayer::ql_gettrtemplate() {
    QList <QStringList> out;
    QSqlQuery query;
    query.prepare(translationquery);

    query.exec();
    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        out << row;

    }
    return out;
}

QString DataAccessLayer::escapedCSV(QString unexc) {
    if (!unexc.contains(QLatin1Char(',')))
        return unexc;
    return unexc.replace("\n", "%0A"); //escape newlines
}

bool DataAccessLayer::tableToCsv(const QString filepath, const QString tablename,
                                 bool isDir) //DANGER - DO NOT ALLOW USERS TO CONTROL THIS
{
    QSqlQuery query;
    query.prepare("select * from " + tablename); //DANGER - DO NOT ALLOW USERS TO CONTROL THIS
    //QFile csvFile (filepath + "/" + tablename + ".csv");

    QFile csvFile;
    if (isDir) {
        csvFile.setFileName(filepath + "/" + tablename + ".csv");
    } else {
        csvFile.setFileName(filepath);
    }

    if (!csvFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug("failed to open csv file");
        return false;
    }
    if (!query.exec()) {
        qDebug("failed to run query");
        return false;
    }
    QTextStream outStream(&csvFile);
    outStream.setEncoding(QStringConverter::Utf8);
    while (query.next()) {
        const QSqlRecord record = query.record();
        for (int i = 0, recCount = record.count(); i < recCount; ++i) {
            if (i > 0)
                outStream << ',';
            outStream << escapedCSV("\"" + record.value(i).toString() + "\"");
        }
        outStream << '\n';
    }
    return true;
}

bool DataAccessLayer::queryToCsv(const QString querystr, QString filename) //DANGER - DO NOT ALLOW USERS TO CONTROL THIS
{
    QSqlQuery query;
    query.prepare(querystr); //DANGER - DO NOT ALLOW USERS TO CONTROL THIS
    //QFile csvFile (filepath + "/" + tablename + ".csv");

    QFile csvFile;
    csvFile.setFileName(filename);

    if (!csvFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug("failed to open csv file");
        return false;
    }
    if (!query.exec()) {
        qDebug("failed to run query");
        return false;
    }
    QTextStream outStream(&csvFile);
    outStream.setEncoding(QStringConverter::Utf8);
    while (query.next()) {
        const QSqlRecord record = query.record();
        for (int i = 0, recCount = record.count(); i < recCount; ++i) {
            if (i > 0)
                outStream << ',';
            outStream << escapedCSV("\"" + record.value(i).toString() + "\"");
        }
        outStream << '\n';
    }
    return true;
}

bool DataAccessLayer::exportTranslatableCSV(QString filename) {
    return queryToCsv(translationquery, filename);
}

//pulled from https://stackoverflow.com/questions/27318631/parsing-through-a-csv-file-in-qt
//which was in turn adapted from https://github.com/hnaohiro/qt-csv/blob/master/csv.cpp
QStringList DataAccessLayer::parseCSV(const QString &string) {
    enum State {
        Normal, Quote
    } state = Normal;
    QStringList fields;
    QString value;

    for (int i = 0; i < string.size(); i++) {
        const QChar current = string.at(i);

        // Normal state
        if (state == Normal) {
            // Comma
            if (current == ',') {
                // Save field
                fields.append(value.trimmed());
                value.clear();
            }

                // Double-quote
            else if (current == '"') {
                state = Quote;
                value += current;
            }

                // Other character
            else
                value += current;
        }

            // In-quote state
        else if (state == Quote) {
            // Another double-quote
            if (current == '"') {
                if (i < string.size()) {
                    // A double double-quote?
                    if (i + 1 < string.size() && string.at(i + 1) == '"') {
                        value += '"';

                        // Skip a second quote character in a row
                        i++;
                    } else {
                        state = Normal;
                        value += '"';
                    }
                }
            }

                // Other character
            else
                value += current;
        }
    }

    //if (!value.isEmpty())
    fields.append(value.trimmed());

    // Quotes are left in until here; so when fields are trimmed, only whitespace outside of
    // quotes is removed.  The quotes are removed here.
    for (int i = 0; i < fields.size(); ++i)
        if (fields[i].length() >= 1 && fields[i].left(1) == '"') {
            fields[i] = fields[i].mid(1);
            if (fields[i].length() >= 1 && fields[i].right(1) == '"')
                fields[i] = fields[i].left(fields[i].length() - 1);
        }

    return fields;
}

//NOTE - THIS ALLOWS DIRECT INJECTION OF DATA--BAD THINGS CAN HAPPEN.
//based on https://dustri.org/b/import-cvs-to-sqlite-with-qt.html
bool DataAccessLayer::importCSV(const QString filepath, const QString tablename, bool isDir) {
    bool success = true;
    QFile f;
    if (isDir) {
        f.setFileName(filepath + "/" + tablename + ".csv");
    } else {
        f.setFileName(filepath);
    }
    //QFile f(filepath+"/"+tablename+".csv");
    if (f.open(QIODevice::ReadOnly)) {
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        success &= query.exec("DELETE FROM " + tablename);
        if (!success) {
            qDebug() << "Could not delete " + tablename;
        }
        QTextStream ts(&f);
        ts.setEncoding(QStringConverter::Utf8);

        while (!ts.atEnd()) {

            QString req = "INSERT INTO " + tablename + " VALUES(";
            // split every lines on comma
            const QStringList line = parseCSV(ts.readLine());
            /*for every values on a line,
                append it to the INSERT request*/
            for (int i = 0; i < line.length(); ++i) {
                if (line.at(i).isEmpty()) {
                    req.append("NULL");
                    req.append(",");
                } else {
                    req.append("\"" + line.at(i) + "\""); //needed quotes to get SQLITE to recognize it
                    req.append(",");
                }
            }
            req.chop(1); // remove the trailing comma
            req.append(getVersionCorrection(tablename, line)); //handle any special cases cause by updates to data
            req.append(");"); // close the "VALUES([...]" with a ");"
            const bool isuccess = query.exec(req.replace("%0A", "\n")); //fix the encoded %0A
            if (!isuccess) {
                qDebug() << "Could not insert using " + req;
            }
            success &= isuccess;
        }
        if (success) {
            QSqlDatabase::database().commit();
        } else {
            QSqlDatabase::database().rollback();
        }
        f.close();
    } else { //couldn't open file
        return !success;
    }
    return success;
}

QString DataAccessLayer::getVersionCorrection(QString tablename, QStringList line) {
    QString toAppend = "";

    if (tablename == "user_curriculum") {
        if (line.count() ==
            5) { //PoW added two new columns. These can default to null, in which case the old functionality works.
            toAppend.append(",\"\",\"\"");
        }
    }

    return toAppend;
}
