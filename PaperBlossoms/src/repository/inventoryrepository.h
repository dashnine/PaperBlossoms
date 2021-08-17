#ifndef INVENTORYREPOSITORY_H
#define INVENTORYREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class InventoryRepository : AbstractRepository {
public:
    InventoryRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QString qs_getitemtype(const QString name);

    QStringList qsl_getitemsunderrarity(const int rarity);

    QStringList qsl_getweaponsunderrarity(const int rarity);

    QStringList qsl_getweapontypeunderrarity(const int rarity, const QString type);

    QStringList qsl_getitemsbytype(const QString type);

    QStringList qsl_getweaponcategories();

    QStringList qsl_getweaponskills();

    QStringList qsl_getqualities();

    QStringList qsl_getpatterns();

    QStringList qsl_getbaseitemdata(const QString name, const QString type);

    QStringList qsl_getitemqualities(const QString name, const QString type);

    QList<QStringList> ql_getweapondata(const QString name);

    QList<QStringList> ql_getarmordata(const QString name);
};

#endif // INVENTORYREPOSITORY_H
