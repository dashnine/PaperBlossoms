#include "skillsrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList SkillsRepository::qsl_getskills() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_tr FROM skills ");
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }
    return out;
}

QStringList SkillsRepository::qsl_getskillsandgroup() {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_tr, skill_group_tr FROM skills ");
    query.exec();
    while (query.next()) {
        const QString s = query.value(0).toString() + "|" + query.value(1).toString();
        out << s;
    }
    return out;
}

QStringList SkillsRepository::qsl_getskillsbygroup(const QString group) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_tr FROM skills WHERE skill_group_tr = ?");
    query.bindValue(0, group);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }
    return out;
}