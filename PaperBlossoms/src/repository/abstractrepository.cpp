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
    //QMapIterator<QString, QVariant> it(query.boundValues());
    QStringList keys(query.boundValueNames());
    QVariantList it(query.boundValues());
    //while (it.hasNext()) {
    //    it.next();
    //    str.replace(it.key(), it.value().toString());
    //}
    for (qsizetype i = 0; i < it.size(); ++i) {
        str.replace(keys.at(i), it.at(i).toString());
    }
    return str;
}

QString AbstractRepository::translate(QString string_tr) {
    return this->dal->translate(string_tr);
}

QString AbstractRepository::untranslate(QString string_tr) {
    return this->dal->untranslate(string_tr);
}