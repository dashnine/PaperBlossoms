#include "schoolsrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

int SchoolsRepository::i_getschoolhonor(const QString school) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT honor FROM schools WHERE name_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();
    }
    return out;
}

QStringList SchoolsRepository::qsl_getschoolability(const QString school) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT school_ability_name_tr, reference_book, reference_page, school_ability_description FROM schools WHERE name_tr = ?");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
        out << "School Ability";
        out << query.value(1).toString();
        out << query.value(2).toString();
        out << query.value(3).toString();
    }
    return out;
}

QStringList SchoolsRepository::qsl_getschoolmastery(const QString school) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT mastery_ability_name_tr, reference_book, reference_page, mastery_ability_description FROM schools WHERE name_tr = ?");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
        out << "School Mastery";
        out << query.value(1).toString();
        out << query.value(2).toString();
        out << query.value(3).toString();
    }
    return out;
}

//adjusted for PoW
QStringList SchoolsRepository::qsl_getschools(const QString clan, const bool allclans, const QString type) {
    QStringList out;
    QSqlQuery query;
    if (!allclans) {
        if (type == "Samurai") {
            query.prepare("SELECT name_tr FROM schools WHERE clan_tr = :clan");
            query.bindValue(0, clan);
        } else if (type == "Gaijin") {
            query.prepare(
                    QString("SELECT name_tr FROM schools WHERE clan_tr = :clan OR clan_tr = :ronin")); // in this case, clan  == region's subtype (Gaijin group, e.g. Ujik)
            query.bindValue(0, clan);
            query.bindValue(1, QString("Rōnin"));
        } else {
            query.prepare(
                    QString("SELECT name_tr FROM schools WHERE clan_tr = :clan")); // in this case, clan  == region
            query.bindValue(0, clan);
        }
    } else {
        query.prepare("SELECT name_tr FROM schools");
    }
    query.exec();
    while (query.next()) {
        const QString result = query.value(0).toString();
        //        qDebug() << result;
        out << result;
    }
    qDebug() << getLastExecutedQuery(query);

    return out;
}

QString SchoolsRepository::qs_getschooldesc(const QString school) {
    QString out;
    QSqlQuery query;
    query.prepare("SELECT description FROM schools WHERE name_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        const QString result = query.value(0).toString();
        //        qDebug() << result;
        out = result;
    }
    return out;
}

QString SchoolsRepository::qs_getschooladvdisadv(const QString school) {
    QString out;
    QSqlQuery query;
    query.prepare("SELECT advantage_disadvantage FROM schools WHERE name_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        const QString result = query.value(0).toString();
        //        qDebug() << result;
        out = result;
    }
    return out;

}

QString SchoolsRepository::qs_getschoolref(const QString school) {
    QSqlQuery query;
    query.prepare("SELECT reference_book, reference_page FROM schools WHERE name_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        QString ref = query.value(0).toString() + " ";
        ref += query.value(1).toString();
        //        qDebug() << desc;
        return ref;
    }
    qWarning() << "ERROR - School" + school + " not found while searching for desc.";
    return "";
}

int SchoolsRepository::i_getschoolskillcount(const QString school) {
    QSqlQuery query;
    query.prepare("SELECT starting_skills_size FROM schools WHERE name_tr = :school");
    query.bindValue(0, school);
    query.exec();
    int count = 0;
    while (query.next()) {
        count = query.value(0).toString().toInt();
        //        qDebug() << count;
    }
    return count;
}

QStringList SchoolsRepository::qsl_getschoolskills(const QString school) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_tr FROM school_starting_skills WHERE school_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        //        qDebug() << cname;
        out << cname;
    }
    return out;
}


QStringList SchoolsRepository::qsl_getschooltechsetids(const QString school) {
    QSqlQuery query;
    QStringList out;
    query.prepare("SELECT distinct set_id FROM school_starting_techniques WHERE school_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
    }
    return out;

}

QStringList SchoolsRepository::qsl_getschoolequipsetids(const QString school) {
    QSqlQuery query;
    QStringList out;
    query.prepare("SELECT distinct set_id FROM school_starting_outfit WHERE school_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
    }
    return out;

}

//returns a qlist of qstringlists.  Each list starts with a selection count, followed by a list of options
QList <QStringList> SchoolsRepository::ql_getlistsoftech(const QString school) {
    //get the number of batches
    const QStringList techids = qsl_getschooltechsetids(school);
    QList <QStringList> out;
    foreach(
    const QString id, techids) {

        QSqlQuery query;
        query.prepare(
                "SELECT set_size, technique_tr FROM school_starting_techniques WHERE school_tr = :school and set_id = :id");
        query.bindValue(0, school);
        query.bindValue(1, id);
        query.exec();
        //TODO - better way of handling these lists? feels hacky
        //first entry: number of things to select.
        //remaining entries: selection set for that row
        //UI will create NUMBER comboboxes containing SELECTIONSET items.
        int choice_count = 0;
        QStringList techlist;
        while (query.next()) {
            QStringList list;
            choice_count = query.value(0).toString().toInt();
            techlist << query.value(1).toString();
        }
        QStringList row;
        row.append(QString::number(choice_count));
        row.append(techlist);
        out << row;

    }
    //    qDebug() << out;
    return out;

}

//returns a qlist of qstringlists.  Each list starts with a selection count, followed by a list of options
QList <QStringList> SchoolsRepository::ql_getlistsofeq(const QString school) {

    //get the number of batches
    QStringList ids = qsl_getschoolequipsetids(school);
    QList <QStringList> out;
    foreach(QString
    id, ids) {

        QSqlQuery query;
        query.prepare(
                "SELECT set_size, equipment_tr FROM school_starting_outfit WHERE school_tr = :school and set_id = :id");
        query.bindValue(0, school);
        query.bindValue(1, id);
        query.exec();
        //TODO - better way of handling these lists? feels hacky
        //first entry: number of things to select.
        //remaining entries: selection set for that row
        //UI will create NUMBER comboboxes containing SELECTIONSET items.
        int choice_count = 0;
        QStringList eqlist;
        while (query.next()) {
            QStringList list;
            choice_count = query.value(0).toString().toInt();
            eqlist << query.value(1).toString();
        }
        QStringList row;
        row.append(QString::number(choice_count));
        row.append(eqlist);
        out << row;

    }
    //    qDebug() << out;
    return out;

}

QStringList SchoolsRepository::qsl_getschoolrings(const QString school) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT ring_tr FROM school_rings WHERE school_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        const QString result = query.value(0).toString();
        //        qDebug() << result;
        out << result;
    }
    return out;

}

QList <QStringList> SchoolsRepository::qsl_getschoolcurriculum(const QString school) {
    QList <QStringList> out;
    QSqlQuery query;
    query.prepare(
            "SELECT rank, advance_tr, type, special_access, min_allowable_rank, max_allowable_rank                  " //select main list
            "FROM curriculum                                             " // from table
            "WHERE school_tr = ?                                            "
    );
    query.bindValue(0, school);
    query.exec();

    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        row << query.value(3).toString();
        row << query.value(4).toString();
        row << query.value(5).toString();
        out << row;

    }
    return out;

}

QStringList SchoolsRepository::qsl_gettechallowedbyschool(QString school) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT technique from school_techniques_available WHERE school_tr = :school");
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        out << query.value(0).toString();
        //        qDebug() << name;
    }
    return out;

}

void SchoolsRepository::qsm_getschoolcurriculum(QSqlQueryModel *const model, const QString school) {

    QSqlQuery query;
    query.prepare(
            "SELECT rank, advance_tr, type, special_access, min_allowable_rank, max_allowable_rank                  " //select main list
            "FROM curriculum                                             " // from table
            "WHERE school_tr = ?                                            "
    );
    query.bindValue(0, school);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        //        qDebug() << cname;
    }
    model->setQuery(query);

}
