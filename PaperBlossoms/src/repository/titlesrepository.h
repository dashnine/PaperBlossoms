#ifndef TITLESREPOSITORY_H
#define TITLESREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class TitlesRepository : AbstractRepository {
public:
    TitlesRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QString qs_gettitleref(const QString title);

    QString qs_gettitlexp(const QString title);

    QString qs_gettitleability(const QString title);

    QStringList qsl_gettitletrack(const QString title);

    QStringList qsl_gettitlemastery(const QString title);

    QStringList qsl_gettitles();

    QList<QStringList> ql_gettitletrack(const QString title);
};

#endif // TITLESREPOSITORY_H
