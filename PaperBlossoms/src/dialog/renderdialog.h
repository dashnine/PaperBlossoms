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

#ifndef RENDERDIALOG_H
#define RENDERDIALOG_H

#include <QDialog>
#include "../pboutputdata.h"
#include <QPrinter>
#include <QTemporaryFile>

namespace Ui {
class RenderDialog;
}

class RenderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenderDialog(PBOutputData *charData, QWidget *parent = 0);
    ~RenderDialog();

private slots:

    void on_printButton_clicked();
    void on_cancelButton_clicked();

    void on_hideskill_checkbox_toggled(const bool checked);

    void on_hideportrait_checkbox_toggled(const bool checked);

    void on_browserButton_clicked();

private:
    Ui::RenderDialog *ui;
    PBOutputData* m_character;

    QString m_curHtml;
    QString m_template;
    QByteArray m_img;
    QByteArray m_ringimg;
    void setTemplate(const QString filename);
    QString generateHtml();

    QPrinter printer;
    QTemporaryFile* tempFile;
    QString newlineToBR(QString text);
    const int MAXSIZE = 500; //max pixels for portrait

};

#endif // RENDERDIALOG_H
