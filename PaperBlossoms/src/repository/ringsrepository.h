#ifndef RINGSREPOSITORY_H
#define RINGSREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class RingsRepository : AbstractRepository {
public:
    RingsRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QString qs_getringdesc(const QString ring);

    QStringList qsl_getrings();
};

#endif // RINGSREPOSITORY_H
