#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../dialog/addadvancedialog.h"
#include "../dialog/addtitledialog.h"
#include "../enums.h"
#include <QSqlRecord>
#include <QDialog>
#include <QString>
#include <QDebug>

void MainWindow::on_xpSpinBox_valueChanged(const int arg1) {
    curCharacter.totalXP = arg1;
    m_dirtyDataFlag = true;
}

void MainWindow::on_curriculum_tableView_doubleClicked(const QModelIndex &index) {
    QSqlRecord clickedrow = curriculummodel.record(index.row());
    QString advance = clickedrow.value("advance_tr").toString();
    QString type = clickedrow.value("type").toString();

    AddAdvanceDialog addadvancedialog(deps, &curCharacter, type, advance);
    const int result = addadvancedialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting advance.";
        m_dirtyDataFlag = true;
        curCharacter.advanceStack.append(addadvancedialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_title_tableview_doubleClicked(const QModelIndex &index) {
    QModelIndex srcindex = titleProxyModel.mapToSource(index);
    QString title = titlemodel.item(srcindex.row(), Title::SOURCE)->text();
    QString type = titlemodel.item(srcindex.row(), Title::TYPE)->text();
    QString advance = titlemodel.item(srcindex.row(), Title::ADVANCE)->text();


    AddAdvanceDialog addadvancedialog(deps, &curCharacter, type, advance);
    const int result = addadvancedialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting advance.";
        m_dirtyDataFlag = true;
        curCharacter.advanceStack.append(addadvancedialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_addTitle_pushButton_clicked() {
    AddTitleDialog addtitledialog(deps, &curCharacter);
    const int result = addtitledialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting title.";
        m_dirtyDataFlag = true;
        curCharacter.titles.append(addtitledialog.getResult());
        //special handlers for adv/disadv in titles
        if (addtitledialog.getResult() == "The Damned") {
            if (!curCharacter.adv_disadv.contains("Ferocity")) curCharacter.adv_disadv.append("Ferocity");
        } else if (addtitledialog.getResult() == "Moon Cultist") {
            curCharacter.adv_disadv.append("Dark Secret");
        }
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_removeTitle_pushButton_clicked() {
    if (curCharacter.titles.count() <= 0) return;

    curCharacter.titles.removeFirst();
    populateUI();
    m_dirtyDataFlag = true;
}

void MainWindow::on_addadvance_button_clicked() {
    AddAdvanceDialog addadvancedialog(deps, &curCharacter);
    const int result = addadvancedialog.exec();
    if (result == QDialog::Accepted) {
        qDebug() << "Accepted: getting advance.";
        m_dirtyDataFlag = true;
        curCharacter.advanceStack.append(addadvancedialog.getResult());
        populateUI();
    } else {
        qDebug() << "Not accepted; discarding changes.";
    }
}

void MainWindow::on_remove_pushButton_clicked() {
    if (curCharacter.advanceStack.count() > 0) {
        //curCharacter.advanceStack.pop_back();

        if (!ui->advance_tableView->currentIndex().isValid()) return;
        const int row = ui->advance_tableView->currentIndex().row();
        curCharacter.advanceStack.removeAt(row); //TODO: TESTING -- is this accurate?
        populateUI();
        m_dirtyDataFlag = true;

    }
}

int MainWindow::isInCurriculum(const QString value, const QString type, const int currank) {
    //   advheaders << "Type"<<"Advance"<<"Track"<<"Cost";

    QStringList skills;
    QStringList techniques;


    for (int i = 0; i < curriculummodel.rowCount(); ++i) {
        const QSqlRecord record = curriculummodel.record(i);
        //if(record.value("rank").toInt()!=curCharacter.rank) continue; //only get items in current rank;
        if (record.value("rank").toInt() != currank) continue; //only get items in current rank;
        int minrank = 1;
        int maxrank = currank;
        QVariant varMinRank = record.value("min_allowable_rank");
        QVariant varMaxRank = record.value("max_allowable_rank");
        if (!varMinRank.isNull())
            minrank = varMinRank.toInt();
        if (!varMaxRank.isNull())
            maxrank = varMaxRank.toInt();
        if (record.value("type").toString() == "skill_group") {
            QStringList groupskills = deps->skillsRepository->qsl_getskillsbygroup(
                    record.value("advance_tr").toString());
            skills.append(groupskills);
        } else if (record.value("type").toString() == "skill") {
            skills << record.value("advance_tr").toString();
        } else if (record.value("type").toString() == "technique") {
            techniques << record.value("advance_tr").toString();
        } else if (record.value("type").toString() == "technique_group") {
            QStringList grouptech = deps->techniquesRepository->qsl_gettechbygroup(
                    deps->dal->untranslate(record.value("advance_tr").toString()), minrank, maxrank);
            techniques.append(grouptech);
        }

        if (type == "Skill") {
            if (skills.contains(value)) return true;
        } else if (type == "Technique") {
            if (techniques.contains(value)) return true;
        }

    }
    return false;
}

int MainWindow::isInTitle(const QString value, const QString adv_type, const QString title) {
    //   advheaders << "Type"<<"Advance"<<"Track"<<"Cost";

    QStringList skills;
    QStringList techniques;
    QStringList rings;


    for (int i = 0; i < titlemodel.rowCount(); ++i) {
        const QString titlename = titlemodel.item(i, 0)->text();
        const QString advance = titlemodel.item(i, 1)->text();
        const QString type = titlemodel.item(i, 2)->text();
        const QString spec_acc = titlemodel.item(i, 3)->text();
        const QString rank = titlemodel.item(i, 4)->text();

        int minrank = 1;
        int maxrank = rank.toInt();

        if (titlename != title) continue; //only get items in current title;
        if (type == "skill_group") {
            const QStringList groupskills = deps->skillsRepository->qsl_getskillsbygroup(advance);
            skills.append(groupskills);
        } else if (type == "skill") {
            skills << advance;
        } else if (type == "technique") {
            techniques << advance;
        } else if (type == "technique_group") {
            const QStringList grouptech = deps->techniquesRepository->qsl_gettechbygroup(
                    deps->dal->untranslate(advance), minrank, maxrank); //special -- uses title rank
                    techniques.append(grouptech);
        } else if (type == "ring") {
            rings << advance;
        }

        if (adv_type == "Skill") {
            if (skills.contains(value)) return true;
        } else if (adv_type == "Technique") {
            if (techniques.contains(value)) return true;
        } else if (adv_type == "Ring") { // dunno if this can ever be true, but prepping for ishikin
            if (rings.contains(value)) return true;
        }

    }
    return false;
}

QPair<int, int> MainWindow::recalcRank() {
    //   advheaders << "Type"<<"Advance"<<"Track"<<"Cost";
    int curricXP = 0;
    int rank = 1;

    foreach (const QString advance, curCharacter.advanceStack) {
        QStringList itemrow;
        foreach(const QString cell, advance.split("|")) {
            itemrow << cell;
        }
        if (itemrow.at(2) == "Curriculum") {
            if (isInCurriculum(itemrow.at(1), itemrow.at(0), rank)) {
                curricXP += itemrow.at(3).toInt();
            } else {
                curricXP += qRound(double(itemrow.at(3).toInt()) / 2.0);
            }
        }
        switch (rank) { //chart from page 98
            case 1:
                if (curricXP >= 20) {
                    rank++;
                    curricXP = 0;
                }
                break;
            case 2:
                if (curricXP >= 24) {
                    rank++;
                    curricXP = 0;
                }
                break;
            case 3:
                if (curricXP >= 32) {
                    rank++;
                    curricXP = 0;
                }
                break;
            case 4:
                if (curricXP >= 44) {
                    rank++;
                    curricXP = 0;
                }
                break;
            case 5:
                if (curricXP >= 60) {
                    rank++;
                    curricXP = 0;
                }
                break;
            default:
                break;
        }
    }
    return QPair<int, int>(rank, curricXP);
}