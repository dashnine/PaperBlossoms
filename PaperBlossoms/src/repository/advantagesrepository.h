#ifndef ADVANTAGESREPOSITORY_H
#define ADVANTAGESREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class AdvantagesRepository : public AbstractRepository {
public:
    AdvantagesRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QStringList qsl_getadv();

    QStringList qsl_getadvdisadv(const QString category);

    QStringList qsl_getadvdisadvbyname(const QString name);

    QStringList qsl_getdisadv();
};

#endif // ADVANTAGESREPOSITORY_H
