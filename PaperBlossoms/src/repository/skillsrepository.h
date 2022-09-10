#ifndef SKILLSREPOSITORY_H
#define SKILLSREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>

class SkillsRepository : AbstractRepository {
public:
    SkillsRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QStringList qsl_getskills();

    QStringList qsl_getskillsbygroup(const QString group);

    QStringList qsl_getskillsandgroup();
};

#endif // SKILLSREPOSITORY_H
