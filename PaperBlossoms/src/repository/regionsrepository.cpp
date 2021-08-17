#include "regionsrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList RegionsRepository::qsl_getregions(QString type) {
    QStringList out;
    //clan query
    QSqlQuery query("SELECT name_tr FROM regions WHERE type = :type ORDER BY name_tr");
    query.bindValue(0, type);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
        //        qDebug() << cname;
    }
    //qDebug() << getLastExecutedQuery(query);

    return out;
}

QString RegionsRepository::qs_getregiondesc(const QString region) {
    QSqlQuery query;
    query.prepare("SELECT description FROM regions WHERE name_tr = :region");
    query.bindValue(0, region);
    query.exec();
    while (query.next()) {
        const QString desc = query.value(0).toString();
        //        qDebug() << desc;
        return desc;
    }
    qWarning() << "ERROR - Region" + region + " not found while searching for desc.";
    return "";
}

QString RegionsRepository::qs_getregionref(const QString region) {
    QSqlQuery query;
    query.prepare("SELECT reference_book, reference_page FROM regions WHERE name_tr = :region");
    query.bindValue(0, region);
    query.exec();
    while (query.next()) {
        QString ref = query.value(0).toString() + " ";
        ref += query.value(1).toString();
        //        qDebug() << desc;
        return ref;
    }
    qWarning() << "ERROR - Region" + region + " not found while searching for ref.";
    return "";
}

QString RegionsRepository::qs_getregionring(const QString region) {
    QSqlQuery query;
    query.prepare("SELECT ring_increase_tr FROM regions WHERE name_tr = :region");
    query.bindValue(0, region);
    query.exec();
    while (query.next()) {
        const QString ring = query.value(0).toString();
        //        qDebug() << ring;
        return ring;
    }
    qWarning() << "ERROR - Region" + region + " not found while searching for rings.";
    return "";
}

//TODO: this is a QStringList, but only returns 1 skill right now.  Refactor?
QStringList RegionsRepository::qsl_getregionskills(const QString region) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_increase_tr FROM regions WHERE name_tr = :region");
    query.bindValue(0, region);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        //        qDebug() << cname;
        out << cname;
    }
    return out;
}


QString RegionsRepository::qs_getregionsubtype(const QString region) {
    QSqlQuery query;
    query.prepare("SELECT subtype FROM regions WHERE name_tr = :region");
    query.bindValue(0, region);
    query.exec();
    while (query.next()) {
        QString subtype = query.value(0).toString();
        return subtype;
    }
    qWarning() << "ERROR - Region" + region + " not found while searching for ref.";
    return "";
}

int RegionsRepository::i_getregionglory(const QString region) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT glory FROM regions WHERE name_tr = :region");
    query.bindValue(0, region);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();
    }
    return out;
}