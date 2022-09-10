#ifndef SCHOOLSREPOSITORY_H
#define SCHOOLSREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>


class SchoolsRepository : AbstractRepository {
public:
    SchoolsRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    void qsm_getschoolcurriculum(QSqlQueryModel *const model, const QString school);

    int i_getschoolhonor(const QString school);

    int i_getschoolskillcount(const QString school);

    QString qs_getschooldesc(const QString school);

    QString qs_getschoolref(const QString school);

    QString qs_getschooladvdisadv(const QString school);

    QStringList qsl_getschoolskills(const QString school);

    QStringList qsl_getschoolability(const QString school);

    QStringList qsl_getschoolmastery(const QString school);

    QStringList qsl_getschools(const QString clan, const bool allclans = false, const QString type = "Samurai");

    QStringList qsl_getschoolrings(const QString clan);

    QStringList qsl_gettechallowedbyschool(QString school);

    QList<QStringList> ql_getlistsofeq(const QString school);

    QList<QStringList> ql_getlistsoftech(const QString school);

    QList<QStringList> qsl_getschoolcurriculum(const QString school);

private:
    QStringList qsl_getschooltechsetids(const QString school);

    QStringList qsl_getschoolequipsetids(const QString school);
};

#endif // SCHOOLSREPOSITORY_H
