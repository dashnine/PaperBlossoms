#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../enums.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include <QSqlRecord>
#include <QBuffer>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtCore/QTextCodec>
#else
    #include <QtCore5Compat/QTextCodec>
#endif


void MainWindow::on_actionExport_User_Tables_triggered() {
    qDebug() << QString("Homepath = ") + QDir::homePath();
    const QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose an export directory..."),
                                                               QDir::homePath());
    if (fileName.isEmpty())
        return;
    else {
        bool success = true;
                foreach(QString tablename, deps->dal->user_tables) {
                success &= deps->dal->tableToCsv(fileName, tablename);
            }
        if (!success) {
            QMessageBox::information(this, tr("Error Exporting Data"),
                                     "One or more errors occured exporting user data. Please check write permissions in the target folder.");

        } else {
            QMessageBox::information(this, tr("Export Complete"), "User data export completed.");

        }

    }
}

void MainWindow::on_actionExport_to_XML_triggered() {

    qDebug() << QString("Homepath = ") + QDir::homePath();
    QString cname = this->curCharacter.family + " " + curCharacter.name;
    if (cname.isEmpty())
        cname = "untitled";
    cname.remove(QRegularExpression("[^a-zA-Z\\d\\s]"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As..."),
                                                    QDir::homePath() + "/" + cname + ".xml",
                                                    tr("Paper Blossoms Character XML (*.xml)"));
    if (fileName.isEmpty())
        return;
    else {
        qDebug() << QString("Filename = ") + fileName;

        QFile file(fileName);

        if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
            QMessageBox::information(this, tr("Unable to open XML file for writing."), file.errorString());
            return;
        }

        //build the root
        QDomDocument document;
        QDomElement root = document.createElement("Character");
        document.appendChild(document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
        document.appendChild(root);

        //add root nodes
        QDomElement name = document.createElement("Name");
        name.setAttribute("value", curCharacter.name);
        root.appendChild(name);
        QDomElement family = document.createElement("Family");
        family.setAttribute("value", curCharacter.family);
        root.appendChild(family);
        QDomElement clan = document.createElement("Clan");
        clan.setAttribute("value", curCharacter.clan);
        root.appendChild(clan);
        QDomElement school = document.createElement("School");
        school.setAttribute("value", curCharacter.school);
        root.appendChild(school);
        QDomElement titles = document.createElement("Titles");
                foreach(QString title, curCharacter.titles) {
                QDomElement titlenode = document.createElement("Title");
                titlenode.setAttribute("value", title);
                titles.appendChild(titlenode);
            }
        root.appendChild(titles);
        QDomElement ninjo = document.createElement("Ninjo");
        ninjo.setAttribute("value", curCharacter.ninjo);
        root.appendChild(ninjo);
        QDomElement giri = document.createElement("Giri");
        giri.setAttribute("value", curCharacter.giri);
        root.appendChild(giri);
        QDomElement abilities = document.createElement("Abilities");
                foreach(QStringList abilityrow, curCharacter.abilities) {
                QDomElement abilnode = document.createElement("Ability");
                abilnode.setAttribute("name", abilityrow.at(Abilities::NAME));
                abilnode.setAttribute("source", abilityrow.at(Abilities::SOURCE));
                abilnode.setAttribute("ref_book", abilityrow.at(Abilities::REF_BOOK));
                abilnode.setAttribute("ref_page", abilityrow.at(Abilities::REF_PAGE));
                abilnode.setAttribute("description", abilityrow.at(Abilities::DESCRIPTION));
                abilities.appendChild(abilnode);
            }
        root.appendChild(abilities);
        QDomElement skills = document.createElement("Skills");
        //get skills
        for (int r = 0; r < skillmodel.rowCount(); ++r) {
            QStringList row;
            for (int c = 0; c < skillmodel.columnCount(); ++c) {
                //if(!skillmodel.item(r,c)->text().isEmpty()){
                row << skillmodel.item(r, c)->text();
                //}
            }
            QDomElement skillnode = document.createElement("Skill");
            skillnode.setAttribute("name", row.at(0));
            skillnode.setAttribute("value", row.at(1));
            skillnode.setAttribute("group", row.at(2));
            skills.appendChild(skillnode);
        }
        root.appendChild(skills);
        //get rings
        QDomElement rings = document.createElement("Rings");
        QDomElement air = document.createElement("Air");
        air.setAttribute("value", curCharacter.baserings[deps->dal->translate("Air")] +
                                  curCharacter.ringranks[deps->dal->translate("Air")]);
        rings.appendChild(air);
        QDomElement earth = document.createElement("Earth");
        earth.setAttribute("value", curCharacter.baserings[deps->dal->translate("Earth")] +
                                    curCharacter.ringranks[deps->dal->translate("Earth")]);
        rings.appendChild(earth);
        QDomElement fire = document.createElement("Fire");
        fire.setAttribute("value", curCharacter.baserings[deps->dal->translate("Fire")] +
                                   curCharacter.ringranks[deps->dal->translate("Fire")]);
        rings.appendChild(fire);
        QDomElement water = document.createElement("Water");
        water.setAttribute("value", curCharacter.baserings[deps->dal->translate("Water")] +
                                    curCharacter.ringranks[deps->dal->translate("Water")]);
        rings.appendChild(water);
        QDomElement voidring = document.createElement("Void");
        voidring.setAttribute("value", curCharacter.baserings[deps->dal->translate("Void")] +
                                       curCharacter.ringranks[deps->dal->translate("Void")]);
        rings.appendChild(voidring);
        root.appendChild(rings);
        //other character basics
        QDomElement socialnode = document.createElement("Social");
        socialnode.setAttribute("honor", curCharacter.honor);
        socialnode.setAttribute("glory", curCharacter.glory);
        socialnode.setAttribute("status", curCharacter.status);
        root.appendChild(socialnode);
        QDomElement wealth = document.createElement("Wealth");
        wealth.setAttribute("koku", curCharacter.koku);
        wealth.setAttribute("bu", curCharacter.bu);
        wealth.setAttribute("zeni", curCharacter.zeni);
        root.appendChild(wealth);
        QDomElement derived = document.createElement("Derived");
        derived.setAttribute("focus", ui->focus_label->text()); //TODO - separate these from the UI
        derived.setAttribute("vigilance", ui->vigilance_label->text());
        derived.setAttribute("endurance", ui->endurance_label->text());
        derived.setAttribute("composure", ui->composure_label->text());
        root.appendChild(derived);
        QDomElement curricstat = document.createElement("RankStatus");
        curricstat.setAttribute("curricstatus", ui->curric_status_label->text()); //TODO - separate these from the UI
        curricstat.setAttribute("titlestatus", ui->title_status_label->text());
        root.appendChild(curricstat);

        //curriculumtable
        QDomElement curriculum = document.createElement("Curriculum");
        for (int r = 0; r < curriculummodel.rowCount(); ++r) {
            QStringList row;
            for (int c = 0; c < curriculummodel.columnCount(); ++c) {
                row << curriculummodel.record(r).value(c).toString();
            }
            QDomElement node = document.createElement("Option");
            node.setAttribute("rank", row.at(0));
            node.setAttribute("advance", row.at(1));
            node.setAttribute("type", row.at(2));
            node.setAttribute("special_access", row.at(3));
            curriculum.appendChild(node);
        }
        root.appendChild(curriculum);
        //titletable
        QDomElement titletable = document.createElement("Title");
        for (int r = 0; r < titlemodel.rowCount(); ++r) {
            QStringList row;
            for (int c = 0; c < titlemodel.columnCount(); ++c) {
                row << titlemodel.item(r, c)->text();
            }
            if (row.at(Title::SOURCE) == this->incompleteTitle) {
                QDomElement node = document.createElement("Option");
                node.setAttribute("advance", row.at(Title::ADVANCE));
                node.setAttribute("type", row.at(Title::TYPE));
                node.setAttribute("special_access", row.at(Title::SPEC));
                node.setAttribute("rank", row.at(Title::TRANK));
                titletable.appendChild(node);
            }
        }
        root.appendChild(titletable);
        //tech
        QDomElement techtable = document.createElement("Techniques");
        for (int r = 0; r < techModel.rowCount(); ++r) {
            QStringList row;
            for (int c = 0; c < techModel.columnCount(); ++c) {
                if (techModel.item(r, c) != NULL) {
                    row << techModel.item(r, c)->text();
                }
            }
            QDomElement node = document.createElement("Technique");
            node.setAttribute("name", row.at(Tech::NAME));
            node.setAttribute("type", row.at(Tech::TYPE));
            node.setAttribute("subtype", row.at(Tech::SUBTYPE));
            node.setAttribute("rank", row.at(Tech::RANK));
            node.setAttribute("book", row.at(Tech::BOOK));
            node.setAttribute("page", row.at(Tech::PAGE));
            node.setAttribute("restriction", row.at(Tech::RESTRICTION));
            node.setAttribute("short_desc", row.at(Tech::SHORT_DESC));
            node.setAttribute("description", row.at(Tech::DESCRIPTION));
            techtable.appendChild(node);
        }
        root.appendChild(techtable);
        //personal traits
        QDomElement personaltable = document.createElement("PersonalTraits");
        for (int r = 0; r < dis_advmodel.rowCount(); ++r) {
            QStringList row;
            for (int c = 0; c < dis_advmodel.columnCount(); ++c) {
                row << dis_advmodel.item(r, c)->text();
            }
            QDomElement node = document.createElement("Technique");
            node.setAttribute("type", row.at(Adv_Disadv::TYPE));
            node.setAttribute("name", row.at(Adv_Disadv::NAME));
            node.setAttribute("ring", row.at(Adv_Disadv::RING));
            node.setAttribute("desc", row.at(Adv_Disadv::DESC));
            node.setAttribute("short_desc", row.at(Adv_Disadv::SHORT_DESC));
            node.setAttribute("book", row.at(Adv_Disadv::BOOK));
            node.setAttribute("page", row.at(Adv_Disadv::PAGE));
            node.setAttribute("types", row.at(Adv_Disadv::TYPES));
            techtable.appendChild(node);
        }
        root.appendChild(personaltable);
        //personal traits
        QDomElement eqtable = document.createElement("Equipment");
        for (int r = 0; r < equipmodel.rowCount(); ++r) {
            QStringList row;
            for (int c = 0; c < equipmodel.columnCount(); ++c) {
                row << equipmodel.item(r, c)->text();
            }
            QDomElement node = document.createElement("Equipment");
            node.setAttribute("type", row.at(Equipment::TYPE));
            node.setAttribute("name", row.at(Equipment::NAME));
            node.setAttribute("desc", row.at(Equipment::DESC));
            node.setAttribute("short_desc", row.at(Equipment::SHORT_DESC));
            node.setAttribute("book", row.at(Equipment::BOOK));
            node.setAttribute("page", row.at(Equipment::PAGE));
            node.setAttribute("price", row.at(Equipment::PRICE));
            node.setAttribute("unit", row.at(Equipment::UNIT));
            node.setAttribute("rarity", row.at(Equipment::RARITY));
            node.setAttribute("qualities", row.at(Equipment::QUALITIES));
            node.setAttribute("w_category", row.at(Equipment::W_CATEGORY));
            node.setAttribute("w_skill", row.at(Equipment::W_SKILL));
            node.setAttribute("w_grip", row.at(Equipment::W_GRIP));
            node.setAttribute("w_minrange", row.at(Equipment::W_MINRANGE));
            node.setAttribute("w_maxrange", row.at(Equipment::W_MAXRANGE));
            node.setAttribute("w_dam", row.at(Equipment::W_DAM));
            node.setAttribute("w_dls", row.at(Equipment::W_DLS));
            node.setAttribute("a_physres", row.at(Equipment::A_PHYSRES));
            node.setAttribute("a_superres", row.at(Equipment::A_SUPERRES));
            eqtable.appendChild(node);
        }
        root.appendChild(eqtable);

        QDomElement heritage = document.createElement("Heritage");
        heritage.setAttribute("value", curCharacter.heritage);
        root.appendChild(heritage);

        QDomElement notes = document.createElement("Notes");
        notes.setAttribute("value", curCharacter.notes);
        root.appendChild(notes);

        QDomElement advances = document.createElement("Advances");
                foreach(QString advance_string, curCharacter.advanceStack) {
                QDomElement advancenode = document.createElement("Advance");
                advancenode.setAttribute("value", advance_string);
                advances.appendChild(advancenode);
            }
        root.appendChild(advances);

        QDomElement totalxp = document.createElement("TotalXP");
        totalxp.setAttribute("value", curCharacter.totalXP);
        root.appendChild(totalxp);

        QDomElement xpspent = document.createElement("XPSpent");
        xpspent.setAttribute("value", ui->xpSpentLabel->text()); //TODO - separate this from UI
        root.appendChild(xpspent);

        QByteArray byteArray;
        QString base64 = "";
        QBuffer buffer(&byteArray); // use buffer to store pixmap into byteArray
        buffer.open(QIODevice::WriteOnly);
        if (!curCharacter.portrait.isNull()) {
            curCharacter.portrait.save(&buffer, "PNG");
            base64 = QString(byteArray.toBase64());
        }
        QDomElement portrait = document.createElement("Portrait");
        portrait.setAttribute("base64image", base64);
        root.appendChild(portrait);

        //now, output the document to the file
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);
        stream << document.toString();
        file.close();
    }

}

void MainWindow::on_actionExport_User_Descriptions_Table_triggered() {
    qDebug() << QString("Homepath = ") + QDir::homePath();
    QString cname = this->curCharacter.family + " " + curCharacter.name;
    if (cname.isEmpty())
        cname = "untitled";
    cname.remove(QRegularExpression("[^a-zA-Z\\d\\s]"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export User Descriptions..."),
                                                    QDir::homePath() + "/user_descriptions.csv", tr("CSV (*.csv)"));
    if (fileName.isEmpty())
        return;
    else {
        qDebug() << QString("Filename = ") + fileName;

        bool success = true;
        success &= deps->dal->tableToCsv(fileName, "user_descriptions", false);
        if (!success) {
            QMessageBox::information(this, tr("Error Exporting Data"),
                                     "One or more errors occured exporting user data. Please check write permissions for the target file.");

        } else {
            QMessageBox::information(this, tr("Export Complete"), "User description data export completed.");

        }

    }
}

