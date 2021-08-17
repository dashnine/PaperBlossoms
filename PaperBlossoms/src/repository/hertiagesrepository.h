#ifndef HERTIAGESREPOSITORY_H
#define HERTIAGESREPOSITORY_H

#include "abstractrepository.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QList>
#include <QMetaEnum>
#include <QMap>
#include <QStringList>
#include <QSqlTableModel>

class HertiagesRepository : AbstractRepository {
public:
    HertiagesRepository(DataAccessLayer *dal) : AbstractRepository(dal) {};

    QStringList qsl_getancestors(const QString source);

    QStringList qsl_getancestormods(const QString ancestor);

    QStringList qsl_getancestorseffects(const QString ancestor);

    QStringList qsl_getheritageranges(const QString heritage);

    QStringList qsl_getancestorranges(const QString ancestor);

    QMap<QString, int> qm_heritagehonorglorystatus(const QString heritage);
};

#endif // HERTIAGESREPOSITORY_H
