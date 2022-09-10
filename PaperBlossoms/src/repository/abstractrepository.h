#ifndef ABSTRACTREPOSITORY_H
#define ABSTRACTREPOSITORY_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include "dataaccesslayer.h"

class AbstractRepository {
public:
    AbstractRepository(DataAccessLayer *dal) {
        this->dal = dal;
    }

protected:
    DataAccessLayer *dal;

    QString translate(QString string_tr);

    QString untranslate(QString string_tr);

    QString getLastExecutedQuery(const QSqlQuery &query);
};

#endif // ABSTRACTREPOSITORY_H
