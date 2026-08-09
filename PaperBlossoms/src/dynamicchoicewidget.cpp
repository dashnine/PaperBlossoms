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

#include "dynamicchoicewidget.h"
#include "ui_dynamicchoicewidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QDebug>

DynamicChoiceWidget::DynamicChoiceWidget( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynamicChoiceWidget)
{
    ui->setupUi(this);
    baseFrame = new QFrame();
    ui->verticalLayout_2->addWidget(baseFrame);

    frameLayout = new QGridLayout(baseFrame);
    baseFrame->setLayout(frameLayout);
    ui->verticalLayout_2->setContentsMargins(0,0,0,0);
    frameLayout->setContentsMargins(0,0,0,0);
    m_isValid = true;

    //ui->verticalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
    //frameLayout->setSizeConstraint(QLayout::SetMinimumSize);
    baseFrame->resize(baseFrame->sizeHint());
    baseFrame->adjustSize();
    //frameLayout->setSizeConstraint(QLayout::SetFixedSize);
    //ui->verticalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
}

DynamicChoiceWidget::~DynamicChoiceWidget()
{
    delete ui;
}

void DynamicChoiceWidget::addCBox(QStringList options, QString value){
    QLabel* label = new QLabel(value, baseFrame);

    QComboBox* cbox = new QComboBox(baseFrame);

    foreach(QString option, options){
        cbox->addItem(option);
    }
    int count = frameLayout->rowCount();
    if (count == 1 && frameLayout->itemAt(0)==NULL) count = 0; //Layout COME WITH one null row when empty--so use 0.
    frameLayout->addWidget(label,count,COLUMNS::Label);
    frameLayout->addWidget(cbox,count,COLUMNS::ComboBox);
    //this->setMinimumHeight(this->minimumHeight()+30);
    //this->setMinimumHeight(cbox->size().height()+30*getCurrent().count());

    if(cbox->count()<=1) cbox->setEnabled(false);
    else cbox->setCurrentIndex(-1);

    connect(cbox,SIGNAL(currentIndexChanged(int)),this,SLOT(dataEntered()));

    baseFrame->resize(baseFrame->minimumSize());
    baseFrame->adjustSize();
    this->updateGeometry();
    emit dataChanged(getCurrent());
    validateBox();
    updateSelections(getCurrent());
}

void DynamicChoiceWidget::dataEntered(){
    const QComboBox* box = qobject_cast<QComboBox *>(QObject::sender());
    const QLineEdit* lineEdit = qobject_cast<QLineEdit *>(QObject::sender());

    for(int i = 0; i<frameLayout->rowCount();++i){

        QLineEdit* field = 0;
        if( frameLayout->columnCount()>2){ //Label, ComboBox, optional LineEdit as 3rd
            if(frameLayout->itemAtPosition(i,2)!=NULL){
                field = dynamic_cast<QLineEdit*>(frameLayout->itemAtPosition(i,COLUMNS::LineEdit)->widget());
            }
        }
        QString fieldText="";
        if(field!=NULL) fieldText = field->text();

    }

    if(box){
        qDebug()<<"ComboBox changed:" << box->currentText();
    }
    else if (lineEdit){
        qDebug()<<"Non-combobox changed:" << lineEdit->text();
    }
    validateBox();
    updateSelections(getCurrent());
    emit dataChanged(getCurrent());
}

QStringList DynamicChoiceWidget::getCurrent(){
    QStringList result;
    int rowcount = frameLayout->rowCount();
    //qDebug()<<"rowcount="<<rowcount;
    int colcount = frameLayout->columnCount();
    //qDebug()<<"colcount="<<colcount;
    if(colcount == 1 && rowcount == 1) return result;
    for(int i = 0; i<rowcount;++i){

        const QLabel* val = dynamic_cast<QLabel*>(frameLayout->itemAtPosition(i,0)->widget());
        const QComboBox* cbox = dynamic_cast<QComboBox*>(frameLayout->itemAtPosition(i,COLUMNS::ComboBox)->widget());

        QLineEdit* field = 0;
        if( frameLayout->columnCount()>2){
            if(frameLayout->itemAtPosition(i,2)!=NULL){
                    field = dynamic_cast<QLineEdit*>(frameLayout->itemAtPosition(i,COLUMNS::LineEdit)->widget());
            }
        }
        const QString fieldText = (field != NULL ? field->text() : "");
        const QString value = val->text();
        const QString curtext = cbox->currentText();
        result << value + "|" + curtext + "|" + fieldText;

    }
    return result;
}

void DynamicChoiceWidget::updateSelections(const QStringList currentList){
    QString result = "";

    foreach(QString row,currentList){
        result+= row.split('|')[COLUMNS::ComboBox] + "|"; //grab the text versions of the contents
    }
    m_selections = result;
    emit selectionsChanged(result);
}

QString DynamicChoiceWidget::getSelections() const
{
   return m_selections;
}

void DynamicChoiceWidget::setSelections(const QString selections)
{
    m_selections = selections;
}

void DynamicChoiceWidget::validateBox(){
    //qDebug()<<"m_isValid=true";
    m_isValid = true; //set to false if error found
    if(frameLayout->itemAt(0)==NULL) return;
    const QStringList boxData = getCurrent();
    for(int i = 0; i<frameLayout->rowCount();++i){

        //Set field visibility
        //QLabel* val = dynamic_cast<QLabel*>(frameLayout->itemAtPosition(i,0)->widget());
        const QComboBox* cbox = dynamic_cast<QComboBox*>(frameLayout->itemAtPosition(i,COLUMNS::ComboBox)->widget());
        QLineEdit* field = 0;
        if( frameLayout->columnCount()>2){
            if(frameLayout->itemAtPosition(i,2)!=NULL){
                    field = dynamic_cast<QLineEdit*>(frameLayout->itemAtPosition(i,COLUMNS::LineEdit)->widget());
            }
        }

        QString fieldText="";
        const QString txt = cbox->currentText();
        if(field!=NULL) {
            fieldText = field->text();

            field->setVisible(m_showFieldMap[txt]);
        }

        //check for field population
        if(field!=NULL && fieldText=="") 	{
            if(m_showFieldMap[txt]==true){
                qDebug()<<"m_isValid=false - field unpopulated for "<<cbox->currentText();

                m_isValid = false;
            }
            field->setStyleSheet("QLineEdit { background-color: khaki }");
        }
        else if(field!=NULL && fieldText!="")	field->setStyleSheet("QLineEdit { background-color: white }");

        //Check for duplication
        QStringList foundRows;

        for(int i = 0; i<frameLayout->rowCount();++i){

            QLabel* val = dynamic_cast<QLabel*>(frameLayout->itemAtPosition(i,COLUMNS::Label)->widget());
            const QComboBox* cbox = dynamic_cast<QComboBox*>(frameLayout->itemAtPosition(i,COLUMNS::ComboBox)->widget());
            QLineEdit* field = 0;
            if( frameLayout->columnCount()>2){
            if(frameLayout->itemAtPosition(i,2)!=NULL){
                        field = dynamic_cast<QLineEdit*>(frameLayout->itemAtPosition(i,COLUMNS::LineEdit)->widget());
                    }
            }
            QString fieldText="";
            const QString curBoxText = cbox->currentText();
            if(field!=NULL) fieldText = field->text();

            if(curBoxText.isEmpty()){
                qDebug()<<"m_isValid=false no choice is made.";
                m_isValid = false;
                val->setStyleSheet("QLabel { color: red}");
                if(field!=NULL) field->setStyleSheet("QLineEdit { background-color: LightCoral }");
            }
            else{
                val->setStyleSheet("QLabel { color: black}");
                const QString frameRow = (cbox->currentText()+'|'+ fieldText);
                foreach(QString row,foundRows){
                    const QString curRow = row.split('|')[COLUMNS::ComboBox]+'|'+row.split('|')[COLUMNS::LineEdit];
                    if(frameRow == curRow || curBoxText.isEmpty()){
                        qDebug()<<"m_isValid=false  "<<frameRow <<" found twice or no choice is made.";
                        m_isValid = false;
                        val->setStyleSheet("QLabel { color: red}");
                        if(field!=NULL) field->setStyleSheet("QLineEdit { background-color: LightCoral }");
                        break;
                    }
                    else{
                        val->setStyleSheet("QLabel { color: black}");
                    }
                }
                foundRows.append(val->text()+'|'+cbox->currentText()+'|'+ fieldText);
            }
        }
    }

}


void DynamicChoiceWidget::clear(){
    delete baseFrame;
    baseFrame = new QFrame();
    ui->verticalLayout_2->addWidget(baseFrame);

    frameLayout = new QGridLayout(baseFrame);
    baseFrame->setLayout(frameLayout);
    //this->setMinimumHeight(3);

    ui->verticalLayout_2->setContentsMargins(0,0,0,0);
    frameLayout->setContentsMargins(0,0,0,0);

    emit dataChanged(getCurrent());
    validateBox();
    baseFrame->resize(baseFrame->sizeHint());
}
