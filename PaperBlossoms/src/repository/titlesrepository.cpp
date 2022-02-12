#include "titlesrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList TitlesRepository::qsl_gettitlemastery(const QString title) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT title_ability_name_tr, reference_book, reference_page, title_ability_description FROM titles WHERE name_tr = ?");
    query.bindValue(0, title);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
        out << title;
        out << query.value(1).toString();
        out << query.value(2).toString();
        out << query.value(3).toString();
    }
    return out;
}


QStringList TitlesRepository::qsl_gettitles() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM titles ");
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }
    return out;
}

QString TitlesRepository::qs_gettitleref(const QString title) {
    QString out = "";
    QSqlQuery query;
    query.prepare("SELECT reference_book, reference_page FROM titles where name_tr = ?");
    query.bindValue(0, title);
    query.exec();
    while (query.next()) {
        const QString b = query.value(0).toString();
        const QString p = query.value(1).toString();
        out += (b + " " + p);
    }
    return out;
}

QString TitlesRepository::qs_gettitlexp(const QString title) {
    QString out = "";
    QSqlQuery query;
    query.prepare("SELECT xp_to_completion FROM titles where name_tr = ?");
    query.bindValue(0, title);
    query.exec();
    while (query.next()) {
        const QString b = query.value(0).toString();
        out = b;
    }
    return out;
}

QString TitlesRepository::qs_gettitleability(const QString title) {
    QString out = "";
    QSqlQuery query;
    query.prepare("SELECT title_ability_name_tr FROM titles where name_tr = ?");
    query.bindValue(0, title);
    query.exec();
    while (query.next()) {
        const QString b = query.value(0).toString();
        out = b;
    }
    return out;
}

QStringList TitlesRepository::qsl_gettitletrack(const QString title) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT title_tr, name_tr, type, special_access,rank           " //select main list
                  "FROM title_advancements                                     " // from table
                  "WHERE title_tr = ?                                             "
    );
    query.bindValue(0, title);
    query.exec();
    while (query.next()) {
        const QString title = query.value(0).toString();
        const QString name = query.value(1).toString();
        const QString type = query.value(2).toString();
        const QString sp_ac = query.value(3).toString();
        const QString rank = query.value(4).toString();
        out << title + "|" + name + "|" + type + "|" + sp_ac + "|" + rank;
        //        qDebug() << out;
    }
    return out;
}

QList <QStringList> TitlesRepository::ql_gettitletrack(const QString title) {
    QList <QStringList> out;
    QSqlQuery query;
    query.prepare("SELECT title_tr, name_tr, type, special_access,rank           " //select main list
                  "FROM title_advancements                                     " // from table
                  "WHERE title_tr = ?                                             "
    );
    query.bindValue(0, title);
    query.exec();
    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        row << query.value(3).toString();
        row << query.value(4).toString();
        out << row;
        //        qDebug() << out;
    }
    return out;
}

