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

#include "ringviewer.h"
#include "ui_ringviewer.h"

RingViewer::RingViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RingViewer)
{
    ui->setupUi(this);
    ui->airring_label->setVisible(false);
    ui->earthring_label->setVisible(false);
    ui->firering_label->setVisible(false);
    ui->voidring_label->setVisible(false);
    ui->waterring_label->setVisible(false);
    ui->airring_label->setStyleSheet("QLabel { color : white; }");
    ui->earthring_label->setStyleSheet("QLabel { color : white; }");
    ui->firering_label->setStyleSheet("QLabel { color : white; }");
    ui->voidring_label->setStyleSheet("QLabel { color : white; }");
    ui->waterring_label->setStyleSheet("QLabel { color : white; }");

    //QFontDatabase::addApplicationFont(":/images/resources/Bradley Hand Bold.ttf");
#ifdef Q_OS_MAC
    const QFont scriptfont = QFont("Bradley Hand", 20, QFont::Bold);
#else
    const QFont scriptfont = QFont("Segoe Script",16, QFont::Bold);
#endif

    ui->airring_label->setFont(scriptfont);
    ui->earthring_label->setFont(scriptfont);
    ui->firering_label->setFont(scriptfont);
    ui->voidring_label->setFont(scriptfont);
    ui->waterring_label->setFont(scriptfont);
}

RingViewer::~RingViewer()
{
    delete ui;
}

void RingViewer::setRings(const QMap<QString, int> ringmap){
    ui->airring_label->setVisible(true);
    ui->earthring_label->setVisible(true);
    ui->firering_label->setVisible(true);
    ui->voidring_label->setVisible(true);
    ui->waterring_label->setVisible(true);
    ui->airring_label->setText(QString::number(ringmap["Air"]));
    ui->earthring_label->setText(QString::number(ringmap["Earth"]));
    ui->firering_label->setText(QString::number(ringmap["Fire"]));
    ui->voidring_label->setText(QString::number(ringmap["Void"]));
    ui->waterring_label->setText(QString::number(ringmap["Water"]));
}

void RingViewer::setBackgroundWhite(){
    ui->label->setStyleSheet("background:white");
}

void RingViewer::setBackgroundClear(){
    ui->label->setStyleSheet("");

}

