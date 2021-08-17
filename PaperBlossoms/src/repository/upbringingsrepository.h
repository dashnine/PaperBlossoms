#ifndef UPBRINGINGSREPOSITORY_H
#define UPBRINGINGSREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class UpbringingsRepository : AbstractRepository {
public:
    UpbringingsRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    int i_getupbringingstatusmod(const QString upbringing);

    int i_getupbringingkoku(const QString upbringing);

    int i_getupbringingbu(const QString upbringing);

    int i_getupbringingzeni(const QString upbringing);

    QString qs_getupbringingitem(const QString upbringing);

    QString qs_getupbringingdesc(const QString upbringing);

    QString qs_getupbringingref(const QString upbringing);

    QStringList qsl_getupbringingrings(const QString upbringing);

    QStringList qsl_getupbringings();

    QStringList qsl_getupbringingskills2(const QString upbringing);

    QStringList qsl_getupbringingskillsbyset(const QString upbringing, const int setID);
};

#endif // UPBRINGINGSREPOSITORY_H
