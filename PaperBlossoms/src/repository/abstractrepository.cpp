#include "abstractrepository.h"
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

QString AbstractRepository::getLastExecutedQuery(const QSqlQuery &query) {
    QString str = query.executedQuery();
    const QVariantList values = query.boundValues();
    for (int i = 0; i < values.size(); ++i) {
        const QString name = query.boundValueName(i);
        if (!name.isEmpty()) {
            str.replace(name, values.at(i).toString());
        }
    }
    return str;
}

QString AbstractRepository::translate(QString string_tr) {
    return this->dal->translate(string_tr);
}

QString AbstractRepository::untranslate(QString string_tr) {
    return this->dal->untranslate(string_tr);
}