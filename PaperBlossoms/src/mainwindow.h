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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dataaccesslayer.h"
#include "character.h"
#include <QStringListModel>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "clicklabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QPair<QString, int> recalcTitle(const QList<int> xp_list);
private slots:
    void on_actionNew_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_triggered();

    void on_addadvance_button_clicked();

    void on_remove_pushButton_clicked();

    void on_addTitle_pushButton_clicked();

    void on_koku_spinBox_valueChanged(const int arg1);

    void on_bu_spinBox_valueChanged(const int arg1);

    void on_zeni_spinBox_valueChanged(const int arg1);



    void on_add_weapon_pushButton_clicked();

    void on_add_armor_pushButton_clicked();

    void on_add_other_pushButton_clicked();

    void on_name_lineEdit_textChanged(const QString &arg1);

    void on_image_label_clicked();


    void on_addDistinction_pushButton_clicked();

    void on_addPassion_pushButton_clicked();

    void on_addAdversity_pushButton_clicked();

    void on_addAnxiety_pushButton_clicked();

    void on_remDistinction_pushButton_clicked();

    void on_remPassion_pushButton_clicked();

    void on_remAdversity_pushButton_clicked();

    void on_remAnxiety_pushButton_clicked();

    void on_actionGenerate_Character_Sheet_triggered();


    void on_ninjo_textEdit_textChanged();

    void on_giri_textEdit_textChanged();

    void on_notes_textEdit_textChanged();

    void on_xpSpinBox_valueChanged(const int arg1);

    void on_glory_spinBox_valueChanged(const int arg1);

    void on_honor_spinBox_valueChanged(const int arg1);

    void on_status_spinBox_valueChanged(const int arg1);

    void on_actionAbout_triggered();

    void on_actionExport_User_Tables_triggered();

    void on_actionImport_User_Data_Tables_triggered();

    void on_actionOpen_Application_Data_Directory_triggered();

    void on_actionExit_triggered();


    void on_removeweapon_pushbutton_clicked();

    void on_removearmor_pushbutton_clicked();

    void on_removepersonaleffect_pushbutton_clicked();

    void on_actionExport_to_XML_triggered();

private:
    Ui::MainWindow *ui;
    DataAccessLayer* dal;
    Character curCharacter;
    void populateUI();
    bool m_dirtyDataFlag;
    const int SAVE_FILE_VERSION = 1;
    const int MIN_FILE_VERSION = 1;
    const int MAX_FILE_VERSION = 1;

    QStandardItemModel skillmodel;
    QStandardItemModel advanceStack;
    QSqlQueryModel curriculummodel;
    QStandardItemModel titlemodel;
    QStandardItemModel equipmodel;
    QStandardItemModel dis_advmodel;
    QStandardItemModel techModel;


    QPair<int, int> recalcRank();
    int isInCurriculum(const QString value, const QString type, const int currank);
    int isInTitle(const QString value, const QString type, const QString title);
    QSortFilterProxyModel titleProxyModel;


    QSortFilterProxyModel weaponProxyModel;
    QSortFilterProxyModel armorProxyModel;
    QSortFilterProxyModel perseffProxyModel;

    QSortFilterProxyModel distinctionsProxyModel;
    QSortFilterProxyModel passionsProxyModel;
    QSortFilterProxyModel adversitiesProxyModel;
    QSortFilterProxyModel anxietiesProxyModel;

    QString incompleteTitle;

    void setColumnsHidden();
    void closeEvent(QCloseEvent * const event);
};

#endif // MAINWINDOW_H
