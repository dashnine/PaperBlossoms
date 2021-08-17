#include "techniquesrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

QStringList TechniquesRepository::qsl_gettechbytyperank(const QString type, const int rank) {
    QStringList out;
    QSqlQuery query;
    query.prepare("select name_tr from techniques where category = :type and rank <= :rank");
    query.bindValue(0, type);
    query.bindValue(1, rank);
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList TechniquesRepository::qsl_getmahoninjutsu(const int rank) {
    QStringList out;
    QSqlQuery query;
    query.prepare("select name_tr from techniques where category IN ('Mahō', 'Ninjutsu') and rank <= :rank");
    query.bindValue(0, rank);
    query.exec();
    while (query.next()) {
        const QString name = query.value(0).toString();
        //        qDebug() << name;
        out << name;
    }
    return out;
}

QStringList TechniquesRepository::qsl_gettechbyname(const QString name) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT distinct name_tr, category, subcategory, rank,                                         "
            "       reference_book, reference_page,restriction_tr,                                         "
            "       short_desc, description                                                             "
            "FROM techniques                                                                            "
            "WHERE name_tr = :name                                                                         "
    );
    query.bindValue(0, name);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
        out << query.value(1).toString();
        out << query.value(2).toString();
        out << query.value(3).toString();
        out << query.value(4).toString();
        out << query.value(5).toString();
        out << query.value(6).toString();
        out << query.value(7).toString();
        out << query.value(8).toString();
    }
    return out;

}

QList <QStringList> TechniquesRepository::ql_getalltechniques() {
    QList <QStringList> out;
    QSqlQuery query;
    query.prepare(
            "SELECT distinct name_tr, category, subcategory, rank,                                      "
            "       xp, reference_book, reference_page,restriction_tr                                   "
            "FROM techniques                                                                            "
    );
    query.exec();
    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        row << query.value(3).toString();
        row << query.value(4).toString();
        row << query.value(5).toString();
        row << query.value(6).toString();
        row << query.value(7).toString();
        out << row;

    }
    return out;
}

void TechniquesRepository::qsm_gettechniquetable(QSqlQueryModel *const model, const QString rank, const QString school,
                                                 const QString title, const bool norestrictions) {
    //technique query
    //assembles the technique options from four sources:
    //      Techs, sorted by category, that are allowed due to rank, filtered by restriciton (school, clan)
    //      Techs allowed due to individual special access (title and curric)
    //      Techs allowed due to tech group special acces (title and curric)
    //      NOTE: checks for subcategory spec access (i.e. water shuji) AND category special access (i.e. Kata)

    const int trank = i_gettitletechgrouprank(title);

    QSqlQuery query;
    if (norestrictions == false) {
        query.prepare(


                "SELECT distinct name_tr, category, subcategory, rank,                                         "
                "       xp, reference_book, reference_page,restriction_tr                                          "
                "FROM techniques                                                                            "//First, get title special
                "WHERE                                                                                      "//  group techs
                //---------------------Special access groups and katagroups from title-------------------//
                "(rank <= ? and subcategory in                                                              " //0 trank
                " (SELECT name from title_advancements                                                      "
                "   WHERE title_tr = ?                                                                         " //1 title
                "   AND type = 'technique_group'                                                            "
                "   AND special_access = 1                                                                  "
                "  )  )                                                                                     "
                "OR                                                                                         " //title Katas (cat v subcat)
                "(rank <= ? and category in                                                                 " //2 trank //cat is group
                " (SELECT name from title_advancements                                                      "
                "   WHERE title_tr = ?                                                                         " //3 title
                "   and type = 'technique_group'                                                            "
                "   AND special_access = 1                                                                  "
                "  ) )                                                                                      "
                //----------------------Special access groups and katagroups from curriculum---------------//
                "OR ( rank <= ? and subcategory in                                                          " //4 rank
                " (SELECT advance from curriculum                                                           "
                "   WHERE school_tr = ?                                                                        " //5 school //subcat is group
                "   AND rank = ? and type = 'technique_group'                                               " //6 rank
                "   AND special_access = 1                                                                  "
                " )                                                                                         "
                "OR  rank <= ? and category in                                                              " //7 rank
                " (SELECT advance from curriculum                                                           "
                "   WHERE school_tr = ?                                                                        " //8 school //subcat is group
                "   AND rank = ? and type = 'technique_group'                                               " //9 rank
                "   AND special_access = 1                                                                  "
                " )  )                                                                                      "
                //------------------------special access indiv tech from curric and title------------------//
                "OR name_tr in (                                                                               "
                "  SELECT advance_tr from curriculum                                                           "
                "   WHERE school_tr = ?                                                                        " //10 school //tech
                "   AND rank = ?                                                                            " //11 rank
                "   AND type = 'technique'                                                                  "
                "   AND special_access = 1                                                                  "
                "  )                                                                                        "
                "OR name_tr in (                                                                               "           //title tech
                "  SELECT name_tr from title_advancements                                                      "
                "   WHERE title_tr = ?                                                                         " //12 title
                "   AND type = 'technique'                                                                  "
                "   AND special_access = 1                                                                  "
                "  )                                                                                        "
                //--------------------------normal tech groups access, restrictions apply-------------------//
                "OR                                                                                         "
                "(rank <= ?                                                                                 " //13 rank   //tech group
                "   AND (category in                                                                         "
                "   (SELECT technique from school_techniques_available                                      "
                "       WHERE school_tr = ?)                                                                   " //14 school
                "   OR subcategory in                                                                         "
                "   (SELECT technique from school_techniques_available                                      "
                "       WHERE school_tr = ?))                                                                   " //15 school
                ")                                                                                          "
                //--------------------------MAHO (and patterns and scrolls FOR EVERYONE!--------------------//
                "OR                                                                                         "
                "((rank <= ? OR rank = NULL)                                                                " //16 rank
                "   AND category IN ('Mahō', 'Item Patterns', 'Signature Scrolls')                          "
                ")                                                                                          "
                "ORDER BY category, rank, name                                                              "
                "");

        //also get title tech special access

        query.bindValue(0, trank);
        query.bindValue(1, title);
        query.bindValue(2, trank);
        query.bindValue(3, title);
        query.bindValue(4, rank);
        query.bindValue(5, school);
        query.bindValue(6, rank);
        query.bindValue(7, rank);
        query.bindValue(8, school);
        query.bindValue(9, rank);
        query.bindValue(10, school);
        query.bindValue(11, rank);
        query.bindValue(12, title);
        query.bindValue(13, rank);
        //query.bindValue(14, clan);
        //query.bindValue(15, school);
        query.bindValue(14, school);
        query.bindValue(15, school);
        query.bindValue(16, rank);
        query.exec();
        qDebug() << getLastExecutedQuery(query);
        while (query.next()) {
            const QString cname = query.value(0).toString();
        }
        model->setQuery(query);
    } else { //norestrictions == true
        query.prepare(


                "SELECT distinct name_tr, category, subcategory, rank,                                      "
                "       xp, reference_book, reference_page,restriction_tr                                   "
                "FROM techniques                                                                            "
                "ORDER BY category, rank, name                                                              "
        );
        query.exec();
        qDebug() << getLastExecutedQuery(query);
        while (query.next()) {
            const QString cname = query.value(0).toString();
        }
        model->setQuery(query);
    }

}

int TechniquesRepository::i_gettitletechgrouprank(const QString title) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT rank                                                 " //select main list
                  "FROM title_advancements                                     " // from table
                  "WHERE title_tr = ?                                             "
    );
    query.bindValue(0, title);
    query.exec();
    while (query.next()) {
        if (!query.value(0).isNull())
            out = query.value(0).toInt();
        //        qDebug() << out;
    }
    return out;
}

QStringList TechniquesRepository::qsl_gettechbygroup(const QString group, const int minrank, int maxrank) {
    //have to use Like here, since the subcategory for Kata is 'General Kata' or 'Close Combat Kata'
    QStringList out;
    //QString grouplike = '%'+group+'%';
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM techniques WHERE category = ? and rank <= ? and rank >= ? "
                  "UNION "
                  "SELECT name_tr FROM techniques WHERE subcategory = ? and rank <= ? and rank >= ?  "
    );
    query.bindValue(0, group);
    query.bindValue(1, maxrank);
    query.bindValue(2, minrank);
    query.bindValue(3, group);
    query.bindValue(4, maxrank);
    query.bindValue(5, minrank);
    query.exec();
    //qDebug() << getLastExecutedQuery(query);

    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }
    return out;
}

QString TechniquesRepository::qs_gettechtypebyname(const QString tech) {
    //NOTE - gets the category of a given teck or tech subcategory
    QString out;
    QSqlQuery query;
    query.prepare("SELECT category FROM techniques WHERE name_tr LIKE ?                "
                  "UNION SELECT category from techniques where subcategory_tr LIKE ?   ");
    query.bindValue(0, tech);
    query.bindValue(1, tech);
    query.exec();
    while (query.next()) {
        QString out = query.value(0).toString();
        if (!out.isEmpty()) return out;
    }
    return "";
}

QString TechniquesRepository::qs_gettechtypebygroupname(const QString tech) {
    //NOTE - gets the category of a given teck or tech subcategory
    QString out;
    QSqlQuery query;
    query.prepare("SELECT category FROM techniques WHERE category_tr LIKE ?            "
                  "UNION SELECT category from techniques where subcategory_tr LIKE ?   ");
    query.bindValue(0, tech);
    query.bindValue(1, tech);
    query.exec();
    while (query.next()) {
        QString out = query.value(0).toString();
        if (!out.isEmpty()) return out;
    }
    return "";
}

QStringList TechniquesRepository::qsl_gettechniquessubtypes() {
    QSqlQuery query;
    query.prepare("SELECT subcategory FROM base_techniques GROUP BY subcategory");
    query.exec();
    QStringList out;
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }

    //    qDebug() << out;
    return out;
}

QStringList
TechniquesRepository::qsl_gettechniquesbysubcategory(const QString subcategory, const int minRank, const int maxRank) {
    QSqlQuery query;
    query.prepare(
            "SELECT name FROM base_techniques WHERE subcategory = :subcategory AND rank <= :minRank AND rank >= :maxRank");
    query.bindValue(0, subcategory);
    query.bindValue(1, minRank);
    query.bindValue(2, maxRank);
    query.exec();
    QStringList out;
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }

    //    qDebug() << out;
    return out;
}