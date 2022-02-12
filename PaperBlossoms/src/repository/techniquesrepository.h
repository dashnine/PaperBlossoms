#ifndef TECHNIQUESREPOSITORY_H
#define TECHNIQUESREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class TechniquesRepository : AbstractRepository {
public:
    TechniquesRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    void
    qsm_gettechniquetable(QSqlQueryModel *const model, const QString rank, const QString school, const QString title,
                          const bool norestrictions = false);

    QString qs_gettechtypebyname(const QString tech);

    QString qs_gettechtypebygroupname(const QString tech);

    QStringList qsl_gettechbytyperank(const QString type, const int rank);

    QStringList qsl_getmahoninjutsu(const int rank);

    QStringList qsl_gettechbyname(const QString name);

    QStringList qsl_gettechbygroup(const QString group, const int minrank, const int maxrank);

    QStringList qsl_gettechniquessubtypes();

    QStringList qsl_gettechniquesbysubcategory(const QString subcategory, const int minRank, const int maxRank);

    QList<QStringList> ql_getalltechniques();

private:
    int i_gettitletechgrouprank(const QString title);
};

#endif // TECHNIQUESREPOSITORY_H
