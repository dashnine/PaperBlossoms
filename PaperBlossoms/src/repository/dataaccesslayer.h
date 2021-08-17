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

class DataAccessLayer {
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
            "user_weapons",
            "user_bonds"
    }; //list of tables to export/import

    const QString translationquery =
            "select strings.term, i18n.string_tr from (                                                                            "
            "select distinct * from (                                                                                   "
            "select distinct name as term from advantages_disadvantages                                                "
            "union select distinct ring as term from advantages_disadvantages                                   "
            "union select distinct types as term from advantages_disadvantages                                   "
            "union select distinct name as term from armor                                                                      "
            "union select distinct price_unit as term from armor                                                                      "
            "union select distinct quality as term from armor_qualities                                   "
            "union select distinct name as term from clans                                                                      "
            "union select distinct type as term from clans                                                                      "
            "union select distinct ring as term from clans                                                                      "
            "union select distinct skill as term from clans                                                                      "
            "union select distinct school as term from curriculum                                                                      "
            "union select distinct advance as term from curriculum                                                                      "
            "union select distinct clan as term from families                                                                      "
            "union select distinct name as term from families                                                                      "
            "union select distinct family as term from family_rings                                                                      "
            "union select distinct family as term from family_skills                                                                      "
            "union select distinct skill as term from family_skills                                                                      "
            "union select distinct ancestor as term from heritage_effects                                                                      "
            "union select distinct outcome as term from heritage_effects                                                                      "
            "union select distinct name as term from item_patterns                                                                      "
            "union select distinct quality as term from personal_effect_qualities                                                                      "
            "union select distinct price_unit as term from personal_effects                                                                      "
            "union select distinct quality as term from qualities                                                                      "
            "union select distinct name as term from rings                                                                      "
            "union select distinct outstanding_quality as term from rings                                                                      "
            "union select distinct ancestor as term from samurai_heritage                                                                      "
            "union select distinct effect_type as term from samurai_heritage                                                                      "
            "union select distinct effect_instructions as term from samurai_heritage                                                                      "
            "union select distinct school as term from school_rings                                                                      "
            "union select distinct ring as term from school_rings                                                                      "
            "union select distinct school as term from school_starting_outfit                                                                      "
            "union select distinct equipment as term from school_starting_outfit                                                                      "
            "union select distinct school as term from school_starting_skills                                                                      "
            "union select distinct school from school_starting_techniques                                                                      "
            "union select distinct technique from school_starting_techniques                                                                      "
            "union select distinct school from school_techniques_available                                                                      "
            "union select distinct technique from school_techniques_available                                                                      "
            "union select distinct name from schools                                                                      "
            "union select distinct role from schools                                                                      "
            "union select distinct clan from schools                                                                      "
            "union select distinct school_ability_name from schools                                                                      "
            "union select distinct mastery_ability_name from schools                                                                      "
            "union select distinct skill_group from skills                                                                      "
            "union select distinct skill from skills                                                                      "
            "union select distinct category from techniques                                                                      "
            "union select distinct subcategory from techniques                                                                      "
            "union select distinct name from techniques                                                                      "
            "union select distinct restriction from techniques                                                                      "
            "union select distinct title from title_advancements                                                                      "
            "union select distinct name from title_advancements                                                                      "
            "union select distinct type from title_advancements                                                                      "
            "union select distinct name from titles                                                                      "
            "union select distinct title_ability_name from titles                                                                      "
            "union select distinct weapon from weapon_qualities                                                                      "
            "union select distinct quality from weapon_qualities                                                                      "
            "union select distinct category from weapons                                                                      "
            "union select distinct name from weapons                                                                      "
            "union select distinct skill from weapons                                                                      "
            "union select distinct grip from weapons                                                                      "
            "union select distinct price_unit from weapons                                                                      "
            "union select distinct name from personal_effects                                                                       "
            ") where term is not NULL and term is not ''                                                                      "
            ") strings                                                                                                          "
            "left join i18n on strings.term = i18n.string                                                                        "
            "order by string_tr, term                                                                        ";


    bool tableToCsv(const QString filepath, const QString tablename, bool isDir = true);

    bool importCSV(const QString filepath, const QString tablename, bool isDir = true);

    QStringList qsl_getdescribablenames();

    bool exportTranslatableCSV(QString filename);

    QString untranslate(QString string_tr);

    QString translate(QString string);

    void qsm_gettranslationmodel(QSqlQueryModel *const model);

    QList<QStringList> ql_gettrtemplate();

private:
    QSqlDatabase db;

    QString escapedCSV(QString unexc);

    QStringList parseCSV(const QString &string);

    bool queryToCsv(const QString querystr, QString filename);

    QString getVersionCorrection(QString tablename, QStringList line);
};

#endif // DATAACCESSLAYER_H
