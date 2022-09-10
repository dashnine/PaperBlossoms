//
// Created by Maciej Kruszewski on 17.08.2021.
//

#ifndef FAMILIESREPOSITORY_H
#define FAMILIESREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>


class FamiliesRepository : AbstractRepository {
public:
    FamiliesRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QString qs_getfamilydesc(const QString family);

    QString qs_getfamilyref(const QString family);

    QStringList qsl_getfamilies(const QString clan);

    int i_getfamilyglory(const QString family);

    int i_getfamilywealth(const QString family);

    QStringList qsl_getfamilyrings(const QString family);

    QStringList qsl_getfamilyskills(const QString family);
};


#endif //FAMILIESREPOSITORY_H
