#include "upbringingsrepository.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QList>
#include <QMetaEnum>
#include <QStringList>
#include <QSqlTableModel>
#include <QDebug>

int UpbringingsRepository::i_getupbringingstatusmod(const QString upbringing) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT status_modification FROM upbringings WHERE name_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();
    }
    return out;
}

QString UpbringingsRepository::qs_getupbringingitem(const QString upbringing) { //some upbringings add a free item
    QString out = "";
    QSqlQuery query;
    query.prepare("SELECT starting_item FROM upbringings WHERE name_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        out = query.value(0).toString();
    }
    return out;
}

int UpbringingsRepository::i_getupbringingkoku(const QString upbringing) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT koku FROM upbringings WHERE name_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();

    }
    return out;
}

int UpbringingsRepository::i_getupbringingbu(const QString upbringing) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT bu FROM upbringings WHERE name_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();

    }
    return out;
}

int UpbringingsRepository::i_getupbringingzeni(const QString upbringing) {
    int out = 0;
    QSqlQuery query;
    query.prepare("SELECT zeni FROM upbringings WHERE name_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        out = query.value(0).toInt();

    }
    return out;
}

QStringList UpbringingsRepository::qsl_getupbringings() {
    QStringList out;
    //family query
    QSqlQuery query;
    query.prepare("SELECT name_tr FROM upbringings ORDER BY name_tr");
    query.exec();
    while (query.next()) {
        const QString fname = query.value(0).toString();
        out << fname;
        //        qDebug() << fname;
    }
    return out;
}


QString UpbringingsRepository::qs_getupbringingdesc(const QString upbringing) {
    QSqlQuery query;
    query.prepare("SELECT description FROM upbringings WHERE name_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        const QString desc = query.value(0).toString();
        //        qDebug() << desc;
        return desc;
    }
    qWarning() << "ERROR - Family" + upbringing + " not found while searching for desc.";
    return "";
}

QString UpbringingsRepository::qs_getupbringingref(const QString upbringing) {
    QSqlQuery query;
    query.prepare("SELECT reference_book, reference_page FROM upbringings WHERE name_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        QString ref = query.value(0).toString() + " ";
        ref += query.value(1).toString();
        //        qDebug() << desc;
        return ref;
    }
    qWarning() << "ERROR - Family" + upbringing + " not found while searching for desc.";
    return "";
}


QStringList UpbringingsRepository::qsl_getupbringingrings(const QString upbringing) {
    //bonus query - rings, skills
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT ring_tr FROM upbringing_rings WHERE upbringing_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        //        qDebug() << cname;
        if (cname == "any") {
            out << translate("Air");
            out << translate("Earth");
            out << translate("Fire");
            out << translate("Water");
            out << translate("Void");
        } else {
            out << cname;
        }
    }
    //model->setQuery(query);
    return out;
}

QStringList UpbringingsRepository::qsl_getupbringingskillsbyset(const QString upbringing, const int setID) {
    QStringList out;
    QSqlQuery query;
    query.prepare(
            "SELECT skill_tr FROM upbringing_skill_increases WHERE upbringing_tr = :upbringing AND set_id = :setID");
    query.bindValue(0, upbringing);
    query.bindValue(1, setID);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();

        if (cname == "any") {
            QSqlQuery secondQuery;
            secondQuery.prepare("SELECT skill_tr FROM skills ");
            secondQuery.exec();
            while (secondQuery.next()) {
                out << secondQuery.value(0).toString();
            }

            return out;
        } else {
            out << cname;
        }
    }
    return out;
}

QStringList UpbringingsRepository::qsl_getupbringingskills2(const QString upbringing) {
    QStringList out;
    QSqlQuery query;
    query.prepare("SELECT skill_tr FROM upbringing_skill_2 WHERE upbringing_tr = :upbringing");
    query.bindValue(0, upbringing);
    query.exec();
    while (query.next()) {
        const QString cname = query.value(0).toString();
        out << cname;
    }
    return out;
}