#ifndef BONDSREPOSITORY_H
#define BONDSREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class BondsRepository : public AbstractRepository {
public:
    BondsRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QStringList qsl_getbonds();

    QStringList qsl_getbond(const QString name);

    QStringList qsl_getbondability(const QString bond);
};

#endif // BONDSREPOSITORY_H
