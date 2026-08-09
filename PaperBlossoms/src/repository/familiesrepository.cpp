//
// Created by Maciej Kruszewski on 17.08.2021.
//

#include "familiesrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList FamiliesRepository::qsl_getfamilies(const QString clan) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM families WHERE clan_tr = :clan ORDER BY name_tr");
    query.bindValue(0, clan);
    query.exec();
    while (query.next()) {
        const QString fname = query.value(0).toString();
        out << fname;
    }
    return out;
}

QString FamiliesRepository::qs_getfamilydesc(const QString family) {
    QSqlQuery query;
    query.prepare("SELECT description FROM families WHERE name_tr = :family");
    query.bindValue(0, family);
    query.exec();
    while (query.next()) {
        const QString desc = query.value(0).toString();
        return desc;
    }
    qWarning() << "ERROR - Family" + family + " not found while searching for desc.";
    return "";
}

QString FamiliesRepository::qs_getfamilyref(const QString family) {
    QSqlQuery query;
    query.prepare("SELECT reference_book, reference_page FROM families WHERE name_tr = :family");
    query.bindValue(0, family);
    query.exec();
    while (query.next()) {
        QString ref = query.value(0).toString() + " ";
        ref += query.value(1).toString();
        return ref;
    }
    qWarning() << "ERROR - Family" + family + " not found while searching for desc.";
    return "";
}

int FamiliesRepository::i_getfamilyglory(const QString family) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT glory FROM families WHERE name_tr = :family");
    query.bindValue(0, family);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();
    }
    return out;
}

int FamiliesRepository::i_getfamilywealth(const QString family) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT wealth FROM families WHERE name_tr = :family");
    query.bindValue(0, family);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();
    }
    return out;
}

QStringList FamiliesRepository::qsl_getfamilyrings(const QString fam) {    ///NOTE - ALSO USED FOR UPBRINGINGS (PoW)
    //bonus query - rings, skills
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT ring_tr FROM family_rings WHERE family_tr = :family");
    query.bindValue(0, fam);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        //        qDebug() << cname;
        out << cname;
    }
    //model->setQuery(query);
    return out;
}

QStringList FamiliesRepository::qsl_getfamilyskills(const QString family) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_tr FROM family_skills WHERE family_tr = :family");
    query.bindValue(0, family);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }
    return out;
}
