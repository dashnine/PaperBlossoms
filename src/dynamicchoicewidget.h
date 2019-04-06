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

#ifndef DYNAMICCHOICEWIDGET_H
#define DYNAMICCHOICEWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QMap>
#include "dataaccesslayer.h"

namespace Ui {
class DynamicChoiceWidget;
}

class DynamicChoiceWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString selections MEMBER m_selections NOTIFY selectionsChanged)

public:
    explicit DynamicChoiceWidget( QWidget *parent = 0);
    ~DynamicChoiceWidget();

    void addCBox(QStringList options, QString value= "");
    QStringList getCurrent();

    void updateSelections(QStringList currentList);
    QString getSelections() const;
    void setSelections(QString selections);
    void clear();
signals:
    void dataChanged(QStringList);
    void selectionsChanged(const QString &newText);

private slots:
    void dataEntered();

private:
    Ui::DynamicChoiceWidget *ui;

    QFrame* baseFrame;
    QGridLayout* frameLayout;
    QMap<QString,bool> m_showFieldMap;//map determining whether or not field is visible

    bool m_isValid;
    void validateBox();

    QString m_selections = "";
    enum COLUMNS { Label,ComboBox,LineEdit};
};

#endif // DYNAMICCHOICEWIDGET_H
