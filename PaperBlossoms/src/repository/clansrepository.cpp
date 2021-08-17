#include "clansrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>


QStringList ClansRepository::qsl_getclans() {
    QStringList out;
    //clan query
    QSqlQuery query("SELECT name_tr FROM clans ORDER BY name_tr");
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
        //        qDebug() << cname;
    }
    return out;
}

QString ClansRepository::qs_getclandesc(const QString clan) {
    QSqlQuery query;
    query.prepare("SELECT description FROM clans WHERE name_tr = :clan");
    query.bindValue(0, clan);
    query.exec();
    while (query.next()) {
        const QString desc = query.value(0).toString();
        //        qDebug() << desc;
        return desc;
    }
    qWarning() << "ERROR - Clan" + clan + " not found while searching for desc.";
    return "";
}

QString ClansRepository::qs_getclanref(const QString clan) {
    QSqlQuery query;
    query.prepare("SELECT reference_book, reference_page FROM clans WHERE name_tr = :clan");
    query.bindValue(0, clan);
    query.exec();
    while (query.next()) {
        QString ref = query.value(0).toString() + " ";
        ref += query.value(1).toString();
        //        qDebug() << desc;
        return ref;
    }
    qWarning() << "ERROR - Clan" + clan + " not found while searching for desc.";
    return "";
}

//TODO: this is a QStringList, but only returns 1 skill right now.  Refactor?
QStringList ClansRepository::qsl_getclanskills(const QString clan) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_tr FROM clans WHERE name_tr = :clan");
    query.bindValue(0, clan);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        //        qDebug() << cname;
        out << cname;
    }
    return out;
}

QString ClansRepository::qs_getclanring(const QString clan) {
    QSqlQuery query;
    query.prepare("SELECT ring_tr FROM clans WHERE name_tr = :clan");
    query.bindValue(0, clan);
    query.exec();
    while (query.next()) {
        const QString ring = query.value(0).toString();
        //        qDebug() << ring;
        return ring;
    }
    qWarning() << "ERROR - Clan" + clan + " not found while searching for rings.";
    return "";
}

int ClansRepository::i_getclanstatus(const QString clan) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT status FROM clans WHERE name_tr = :clan");
    query.bindValue(0, clan);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();
    }
    return out;
}
