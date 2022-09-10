#include "bondsrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList BondsRepository::qsl_getbonds() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM bonds");
    //query.bindValue(0, category);
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList BondsRepository::qsl_getbond(const QString name) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT name_tr, bond_ability_name_tr, description, short_desc, reference_book, reference_page FROM bonds WHERE name_tr = :name");
    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        //        qDebug() << name;
        out << query.value(0).toString();
        out << query.value(1).toString();
        out << query.value(2).toString();
        out << query.value(3).toString();
        out << query.value(4).toString();
        out << query.value(5).toString();
    }
    return out;
}

QStringList BondsRepository::qsl_getbondability(const QString bond) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT bond_ability_name_tr, reference_book, reference_page, bond_ability_description FROM bonds WHERE name_tr = ?");
    query.bindValue(0, bond);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
        out << bond;
        out << query.value(1).toString();
        out << query.value(2).toString();
        out << query.value(3).toString();
    }
    return out;
}