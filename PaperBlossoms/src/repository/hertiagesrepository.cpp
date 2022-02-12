#include "hertiagesrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList HertiagesRepository::qsl_getancestors(QString source) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT ancestor_tr FROM samurai_heritage WHERE source = ? order by roll_min");
    query.bindValue(0, source);
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList HertiagesRepository::qsl_getancestormods(const QString ancestor) {

    QMap<QString, int> map;
    QStringList out;

    map["Honor"] = 0;
    map["Glory"] = 0;
    map["Status"] = 0;
    QSqlQuery query;
    query.prepare(
            "SELECT modifier_honor, modifier_glory, modifier_status FROM samurai_heritage WHERE ancestor_tr = :ancestor");
    query.bindValue(0, ancestor);
    query.exec();
    while (query.next()) {
        map["Honor"] = query.value(0).toInt();
        map["Glory"] = query.value(1).toInt();
        map["Status"] = query.value(2).toInt();
    }
    out.append(QString::number(map["Honor"]));
    out.append(QString::number(map["Glory"]));
    out.append(QString::number(map["Status"]));
    return out;
}

QStringList HertiagesRepository::qsl_getancestorseffects(const QString ancestor) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT outcome_tr FROM heritage_effects where ancestor_tr = :ancestor order by roll_min");
    query.bindValue(0, ancestor);
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
//        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList HertiagesRepository::qsl_getheritageranges(const QString heritage) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT roll_min, roll_max from HERITAGE_EFFECTS where ancestor_tr = :heritage ORDER BY roll_min");
    query.bindValue(0, heritage);
    query.exec();
    while (query.next()) {
        const QString a = query.value(0).toString();
        const QString b = query.value(1).toString();
//        qDebug() << a + ", " + b;
        out << a + ", " + b;
    }
    return out;
}

QStringList HertiagesRepository::qsl_getancestorranges(const QString source) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT roll_min, roll_max from samurai_heritage where source = ? ORDER BY roll_min");
    query.bindValue(0, source);
    query.exec();
    while (query.next()) {
        QString a = query.value(0).toString();
        QString b = query.value(1).toString();
//        qDebug() << a + ", " + b;
        out << a + ", " + b;
    }
    return out;
}

QMap<QString, int> HertiagesRepository::qm_heritagehonorglorystatus(const QString heritage) {

    QMap<QString, int> map;
    QStringList out;

    map["Honor"] = 0;
    map["Glory"] = 0;
    map["Status"] = 0;
    QSqlQuery query;
    query.prepare(
            "SELECT modifier_honor, modifier_glory, modifier_status FROM samurai_heritage WHERE ancestor_tr = :ancestor");
    query.bindValue(0, heritage);
    query.exec();
    while (query.next()) {
        map["Honor"] = query.value(0).toInt();
        map["Glory"] = query.value(1).toInt();
        map["Status"] = query.value(2).toInt();
    }
    return map;


}
