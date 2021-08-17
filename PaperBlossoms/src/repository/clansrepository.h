#ifndef CLANSREPOSITORY_H
#define CLANSREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class ClansRepository : AbstractRepository {
public:
    ClansRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    int i_getclanstatus(const QString clan);

    QString qs_getclandesc(const QString clan);

    QString qs_getclanref(const QString clan);

    QString qs_getclanring(const QString clan);

    QStringList qsl_getclans();

    QStringList qsl_getclanskills(const QString clan);
};

#endif // CLANSREPOSITORY_H
