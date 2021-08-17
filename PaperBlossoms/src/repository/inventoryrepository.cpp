#include "inventoryrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList InventoryRepository::qsl_getitemsunderrarity(const int rarity) {
    //bonus query - rings, skills
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "select distinct name_tr from personal_effects where rarity <= ? union select distinct name_tr from weapons "
            "where rarity <= ? union select distinct name_tr from armor where rarity <= ?");
    query.bindValue(0, rarity);
    query.bindValue(1, rarity);
    query.bindValue(2, rarity);
    query.exec();
    //TODO - replace with multi-line response in special table, rather than splitting?
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList InventoryRepository::qsl_getweaponsunderrarity(const int rarity) {
    //bonus query - rings, skills
    QStringList out;
    QSqlQuery query;
    query.prepare("select distinct name_tr from weapons "
                  "where rarity <= ?");
    query.bindValue(0, rarity);
    query.exec();
    //TODO - replace with multi-line response in special table, rather than splitting?
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList InventoryRepository::qsl_getweapontypeunderrarity(const int rarity, const QString type) {
    //bonus query - rings, skills
    QStringList out;
    QSqlQuery query;
    query.prepare("select distinct name_tr from weapons "
                  "where rarity <= ?                 "
                  "and category = ?                  ");
    query.bindValue(0, rarity);
    query.bindValue(1, type);
    query.exec();
    //TODO - replace with multi-line response in special table, rather than splitting?
    while (query.next()) {
        QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList InventoryRepository::qsl_getitemsbytype(const QString type) {
    //bonus query - rings, skills
    QStringList out;
    QSqlQuery query;
    if (type == "Weapon") {
        query.prepare("select distinct name_tr from weapons");
    } else if (type == "Armor") {
        query.prepare("select name_tr from armor");

    } else {
        query.prepare("select name_tr from personal_effects");

    }
    query.exec();
    //TODO - replace with multi-line response in special table, rather than splitting?
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QList <QStringList> InventoryRepository::ql_getweapondata(const QString name) {
    QList <QStringList> out;
    QSqlQuery query;
    query.prepare("SELECT category_tr, skill_tr, grip_tr, range_min, range_max, damage, deadliness           "
                  "from weapons where name_tr = ?                                                      ");

    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        row << query.value(3).toString();
        row << query.value(4).toString();
        row << query.value(5).toString();
        row << query.value(6).toString();
        out << row;

    }
    return out;
}

QString InventoryRepository::qs_getitemtype(const QString name) {
    QSqlQuery query;
    query.prepare("select count(distinct name_tr) from weapons where name_tr = ?");
    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        const int count = query.value(0).toInt();
        if (count > 0) return "Weapon";
    }
    query.clear();
    query.prepare("select count(distinct name_tr) from armor where name_tr = ?");
    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        const int count = query.value(0).toInt();
        if (count > 0) return "Armor";
    }
    query.clear();
    query.prepare("select count(distinct name_tr) from personal_effects where name_tr = ?");
    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        const int count = query.value(0).toInt();
        if (count > 0) return "Personal Effect";
    }


    return "Unknown";
}


QStringList InventoryRepository::qsl_getweaponcategories() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT distinct category_tr "
                  "from weapons");

    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
    }
    return out;
}

QStringList InventoryRepository::qsl_getweaponskills() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT distinct skill_tr "
                  "from weapons");

    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
    }
    return out;
}

QStringList InventoryRepository::qsl_getbaseitemdata(const QString name, const QString type) {
    //      0       1               2           3               4                   5               6           7
    //    name  |description    |short_desc |reference_book |reference_page    |price_value    |price_unit |rarity |
    //      8       9       10          11          12      13          14
    //    skill  |grip   |range_min  |range_max  |damage |deadliness | qualities
    //                          15                  16
    //    (qualities)| resistance_category | resist_value
    QStringList out;
    QSqlQuery query;
    if (type == "Weapon") {
        query.prepare(
                "SELECT name_tr, description, short_desc, reference_book, reference_page, price_value, price_unit, rarity "
                "from weapons where name_tr = ?");
    } else if (type == "Armor") {
        query.prepare(
                "SELECT name_tr, description, short_desc, reference_book, reference_page, price_value, price_unit, rarity "
                "from armor where name_tr = ?");
    } else {
        query.prepare(
                "SELECT name_tr, description, short_desc, reference_book, reference_page, price_value, price_unit, rarity "
                "from personal_effects where name_tr = ?");
    }

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
    }
    return out;
}

QStringList InventoryRepository::qsl_getitemqualities(const QString name, const QString type) {
    QStringList out;
    QSqlQuery query;
    if (type == "Weapon") {
        query.prepare("SELECT quality_tr "
                      "from weapon_qualities where weapon_tr = ?");
    } else if (type == "Armor") {
        query.prepare("SELECT quality_tr "
                      "from armor_qualities where armor_tr = ?");
    } else {
        //TODO - handle other qualities
        return out;
    }

    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
    }
    return out;
}

QList <QStringList> InventoryRepository::ql_getarmordata(const QString name) {
    QList <QStringList> out;
    QSqlQuery query;
    query.prepare("SELECT resistance_category, resistance_value                                               "
                  "from armor_resistance where armor_tr = ?                                                      ");

    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        out << row;

    }
    return out;
}

QStringList InventoryRepository::qsl_getqualities() {
    QStringList out;
    QSqlQuery query;
    query.prepare("select quality_tr from qualities");
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList InventoryRepository::qsl_getpatterns() {
    QStringList out;
    QSqlQuery query;
    query.prepare("select name_tr from item_patterns");
    query.exec();
    while (query.next()) {
        QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}