#include "advantagesrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList AdvantagesRepository::qsl_getadvdisadv(const QString category) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM advantages_disadvantages WHERE category = :category");
    query.bindValue(0, category);
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList AdvantagesRepository::qsl_getadvdisadvbyname(const QString name) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT category, name_tr, ring_tr, description, short_desc, reference_book, reference_page, types FROM advantages_disadvantages WHERE name_tr = :name");
    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
        out << query.value(1).toString();
        out << query.value(2).toString();
        out << query.value(3).toString();
        out << query.value(4).toString();
        out << query.value(5).toString();
        out << query.value(6).toString();
        out << query.value(7).toString();
        //        qDebug() << name;
    }
    return out;
}

QStringList AdvantagesRepository::qsl_getadv() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM advantages_disadvantages WHERE category IN ('Distinctions', 'Passions')");
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList AdvantagesRepository::qsl_getdisadv() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM advantages_disadvantages WHERE category IN ('Adversities', 'Anxieties')");
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}