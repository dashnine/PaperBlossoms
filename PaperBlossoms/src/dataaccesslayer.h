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
#include <QSqlTableModel>

class DataAccessLayer
{
public:
    DataAccessLayer(QString locale = "en");

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

    //void qsm_getclans(const QSqlQueryModel* model);
    //void qsm_getfamilies(const QSqlQueryModel* model, const QString clan);
    QStringList qsl_getclans();
    QStringList qsl_getfamilies(const QString clan);
    QStringList qsl_getfamilyrings(const QString family);
    QString qs_getclandesc(const QString clan);
    QString qs_getfamilydesc(const QString family);
    QStringList qsl_getschools(const QString clan, const bool allclans = false);
    QString qs_getschooldesc(const QString school);
    QStringList qsl_getschoolskills(const QString school);
    int i_getschoolskillcount(const QString school);
    //int i_getschooltechcount(const QString school);
    QList<QStringList> ql_getlistsoftech(const QString school);
    QStringList qsl_getrings();
    QList<QStringList> ql_getlistsofeq(const QString school);
    QStringList qsl_getadvdisadv(const QString category);
    QStringList qsl_getclanskills(const QString clan);
    QStringList qsl_getfamilyskills(const QString family);
    QStringList qsl_getskills();
    QStringList qsl_getadv();
    QStringList qsl_getdisadv();
    QStringList qsl_getitemsunderrarity(const int rarity);
    QStringList qsl_getancestors(const QString source);
    QStringList qsl_getancestormods(const QString ancestor);
    QStringList qsl_getancestorseffects(const QString ancestor);
    QStringList qsl_getitemsbytype(const QString type);
    QStringList qsl_gettechbytyperank(const QString type, const int rank);
    QStringList qsl_getmahoninjutsu(const int rank);
    QString qs_getclanring(const QString clan);
    QStringList qsl_getschoolrings(const QString clan);
    QStringList qsl_getqualities();
    QStringList qsl_getheritageranges(const QString heritage);
    //QStringList qsl_getstartingeqfixed(const QString school);
    int i_getclanstatus(const QString clan);
    int i_getfamilyglory(const QString family);
    int i_getfamilywealth(const QString family);
    int i_getschoolhonor(const QString school);
    QMap<QString, int> qm_heritagehonorglorystatus(const QString heritage);
    //QStringList qsl_getschooltechavailable(QString school, bool maho_allowed);

    void qsm_gettechniquetable(QSqlQueryModel * const model, const QString rank, const QString school, const QString title, const QString clan);
    //void qsm_getfilteredtechniquetable(QSqlQueryModel * const model, const QString category, const QString rank, const QString school);
    void qsm_getschoolcurriculum(QSqlQueryModel * const model, const QString school);
    //void qsm_getschoolcurriculumbyrank(QSqlQueryModel * const model, const QString school, const int rank);
    QStringList qsl_getskillsbygroup(const QString group);
    QStringList qsl_gettechbygroup(const QString group, const int rank);
    QStringList qsl_getskillsandgroup();
    QStringList qsl_gettitles();
    QString qs_gettitleref(const QString title);
    QString qs_gettitlexp(const QString title);
    QString qs_gettitleability(const QString title);
    //void qsm_gettitletrack(QSqlQueryModel * const model, const QString title);
    QStringList qsl_gettitletrack(const QString title);
    int i_gettitletechgrouprank(const QString title);
    QString qs_getitemtype(const QString name);
    //QStringList qsl_getweapon(const QString name);
    //QString qs_getarmor(const QString name);
    //QString qs_getperseffects(const QString name);
    QStringList qsl_getbaseitemdata(const QString name, const QString type);
    QStringList qsl_getitemqualities(const QString name, const QString type);
    QList<QStringList> ql_getweapondata(const QString name);
    QList<QStringList> ql_getarmordata(const QString name);
    QStringList qsl_getadvdisadvbyname(const QString name);
    QStringList qsl_gettechbyname(const QString name);
    QStringList qsl_getschoolability(const QString school);
    QStringList qsl_getschoolmastery(const QString school);
    QStringList qsl_gettitlemastery(const QString title);
    QString qs_getschoolref(const QString school);
    QString qs_getfamilyref(const QString family);
    QString qs_getclanref(const QString clan);
    QStringList qsl_getweaponsunderrarity(const int rarity);
    QString qs_getschooladvdisadv(const QString school);
    bool tableToCsv(const QString filepath, const QString tablename, bool isDir = true);
    bool importCSV(const QString filepath, const QString tablename, bool isDir = true);
    QStringList qsl_getancestorranges(const QString ancestor);
    QStringList qsl_getweapontypeunderrarity(const int rarity, const QString type);
    QStringList qsl_getpatterns();
    QString qs_getringdesc(const QString ring);
    QStringList qsl_getdescribablenames();
    bool exportTranslatableCSV(QString filename);
    QString untranslate(QString string_tr);
    QString translate(QString string);
    QStringList qsl_getweaponcategories();
    QStringList qsl_getweaponskills();
    QString qs_gettechtypebyname(const QString tech);
private:
    QSqlDatabase db;
    QStringList qsl_getschooltechsetids(const QString school);
    QStringList qsl_getschoolequipsetids(const QString school);
    QString getLastExecutedQuery(const QSqlQuery &query);
    QString escapedCSV(QString unexc);
    QStringList parseCSV(const QString &string);
    bool queryToCsv(const QString querystr, QString filename);
};

#endif // DATAACCESSLAYER_H
