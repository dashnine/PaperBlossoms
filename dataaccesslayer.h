/*
 * *******************************************************************
 * This file is part of the Paper Blossoms application
 * (https://github.com/dashnine/PaperBlossoms).
 * Copyright (c) 2019 Kyle Hankins (dashnine)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The Legend of the Five Rings Roleplaying Game is the creation
 * and property of Fantasy Flight Games.
 * *******************************************************************
 */

#ifndef DATAACCESSLAYER_H
#define DATAACCESSLAYER_H
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QStringList>

class DataAccessLayer
{
public:
    DataAccessLayer();

    const QStringList user_tables = {
        "user_advantages_disadvantages",
        "user_armor",
        "user_armor_qualities",
        "user_armor_resistance",
        "user_clans",
        "user_curriculum",
        "user_descriptions",
        "user_families",
        "user_family_rings",
        "user_family_skills",
        "user_personal_effect_qualities",
        "user_personal_effects",
        "user_qualities",
        "user_school_rings",
        "user_school_starting_outfit",
        "user_school_starting_skills",
        "user_school_starting_techniques",
        "user_school_techniques_available",
        "user_schools",
        "user_skills",
        "user_techniques",
        "user_title_advancements",
        "user_titles",
        "user_weapon_qualities",
        "user_weapons"
    }; //list of tables to export/import

    void qsm_getclans(QSqlQueryModel* model);
    void qsm_getfamilies(QSqlQueryModel* model, QString clan);
    QStringList qsl_getclans();
    QStringList qsl_getfamilies(QString clan);
    QStringList qsl_getfamilyrings(QString family);
    QString qs_getclandesc(QString clan);
    QString qs_getfamilydesc(QString family);
    QStringList qsl_getschools(QString clan, bool allclans = false);
    QString qs_getschooldesc(QString school);
    QStringList qsl_getschoolskills(QString school);
    int i_getschoolskillcount(QString school);
    int i_getschooltechcount(QString school);
    QList<QStringList> ql_getlistsoftech(QString school);
    QStringList qsl_getrings();
    QList<QStringList> ql_getlistsofeq(QString school);
    QStringList qsl_getadvdisadv(QString category);
    QStringList qsl_getclanskills(QString clan);
    QStringList qsl_getfamilyskills(QString family);
    QStringList qsl_getskills();
    QStringList qsl_getadv();
    QStringList qsl_getdisadv();
    QStringList qsl_getitemsunderrarity(int rarity);
    QStringList qsl_getancestors();
    QStringList qsl_getancestormods(QString ancestor);
    QStringList qsl_getancestorseffects(QString ancestor);
    QStringList qsl_getitemsbytype(QString type);
    QStringList qsl_gettechbytyperank(QString type, int rank);
    QStringList qsl_getmahoninjutsu(int rank);
    QString qs_getclanring(QString clan);
    QStringList qsl_getschoolrings(QString clan);
    QStringList qsl_getqualities();
    QStringList qsl_getheritageranges(QString heritage);
    QStringList qsl_getstartingeqfixed(QString school);
    int i_getclanstatus(QString clan);
    int i_getfamilyglory(QString family);
    int i_getfamilywealth(QString family);
    int i_getschoolhonor(QString school);
    QMap<QString, int> qm_heritagehonorglorystatus(QString heritage);
    //QStringList qsl_getschooltechavailable(QString school, bool maho_allowed);

    void qsm_gettechniquetable(QSqlQueryModel *model, QString rank, QString school, QString title, QString clan);
    void qsm_getfilteredtechniquetable(QSqlQueryModel *model, QString category, QString rank, QString school);
    void qsm_getschoolcurriculum(QSqlQueryModel *model, QString school);
    void qsm_getschoolcurriculumbyrank(QSqlQueryModel *model, QString school, int rank);
    QStringList qsl_getskillsbygroup(QString group);
    QStringList qsl_gettechbygroup(QString group, int rank);
    QStringList qsl_getskillsandgroup();
    QStringList qsl_gettitles();
    QString qs_gettitleref(QString title);
    QString qs_gettitlexp(QString title);
    QString qs_gettitleability(QString title);
    void qsm_gettitletrack(QSqlQueryModel *model, QString title);
    QStringList qsl_gettitletrack(QString title);
    int i_gettitletechgrouprank(QString title);
    QString qs_getitemtype(QString name);
    QStringList qsl_getweapon(QString name);
    QString qs_getarmor(QString name);
    QString qs_getperseffects(QString name);
    QStringList qsl_getbaseitemdata(QString name, QString type);
    QStringList qsl_getitemqualities(QString name, QString type);
    QList<QStringList> ql_getweapondata(QString name);
    QList<QStringList> ql_getarmordata(QString name);
    QStringList qsl_getadvdisadvbyname(QString name);
    QStringList qsl_gettechbyname(QString name);
    QStringList qsl_getschoolability(QString school);
    QStringList qsl_getschoolmastery(QString school);
    QStringList qsl_gettitlemastery(QString title);
    QString qs_getschoolref(QString school);
    QString qs_getfamilyref(QString family);
    QString qs_getclanref(QString clan);
    QStringList qsl_getweaponsunderrarity(int rarity);
    QString qs_getschooladvdisadv(QString school);
    bool queryToCsv(QString filepath, QString tablename);
    bool importCSV(QString filepath, QString tablename);
private:
    QSqlDatabase db;
    QStringList qsl_getschooltechsetids(QString school);
    QStringList qsl_getschoolequipsetids(QString school);
    QString getLastExecutedQuery(const QSqlQuery &query);
    QString escapedCSV(QString unexc);
    QStringList parseCSV(const QString &string);
};

#endif // DATAACCESSLAYER_H
