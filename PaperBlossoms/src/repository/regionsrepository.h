#ifndef REGIONSREPOSITORY_H
#define REGIONSREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class RegionsRepository : AbstractRepository {
public:
    RegionsRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    int i_getregionglory(const QString region);

    QString qs_getregiondesc(const QString region);

    QString qs_getregionref(const QString region);

    QString qs_getregionring(const QString region);

    QString qs_getregionsubtype(const QString region);

    QStringList qsl_getregions(QString type);

    QStringList qsl_getregionskills(const QString region);

};

#endif // REGIONSREPOSITORY_H
