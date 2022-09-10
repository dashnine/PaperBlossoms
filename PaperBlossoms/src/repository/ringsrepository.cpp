#include "ringsrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList RingsRepository::qsl_getrings() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM rings");
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QString RingsRepository::qs_getringdesc(const QString ring) {
    QString out;
    QSqlQuery query;
    query.prepare("SELECT outstanding_quality_tr FROM rings WHERE name_tr = :ring");
    query.bindValue(0, ring);
    query.exec();
    while (query.next()) {
        const QString result = query.value(0).toString();
        //        qDebug() << result;
        out = result;
    }
    return out;

}