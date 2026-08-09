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

#include "renderdialog.h"
#include "ui_renderdialog.h"
#include "../pboutputdata.h"
#include <QBuffer>
#include <QFile>
#include <QMessageBox>
#include "../enums.h"
#include <QPrintDialog>
#include <QPrinter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QWebEngineSettings>
#include <QTextStream>

RenderDialog::RenderDialog(PBOutputData* charData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/resources/sakura.png"));
    this->m_character = charData;
    tempFile = new QTemporaryFile;
    //qDebug()<<QDir::tempPath();

//#ifndef Q_OS_MAC
//#endif

    m_curHtml = "";
    m_template = "";
    m_character = charData;

    QString filename = "";
    //filename += QCoreApplication::applicationDirPath();
    //filename+="/PB_TEMPLATE.html";
    filename+=":/templates/PB_TEMPLATE.html";
    setTemplate(filename);

    m_img = "";
    QByteArray byteArray;
    QBuffer buffer(&byteArray); // use buffer to store pixmap into byteArray
    buffer.open(QIODevice::WriteOnly);
    QImage scaledportrait;
    if(!m_character->portrait.isNull()){ //scale down absurdly large images to more rational sizes for printing.
        int w = m_character->portrait.size().width();
        int h = m_character->portrait.size().height();
        if(w>=h && w > MAXSIZE){
            scaledportrait = m_character->portrait.scaledToWidth(500,Qt::SmoothTransformation);
        }
        else if(h>w && h > MAXSIZE){
            scaledportrait = m_character->portrait.scaledToHeight(500,Qt::SmoothTransformation);
        }
        else{
            QImage scaledportrait = m_character->portrait;

        }
        scaledportrait.save(&buffer, "PNG",0);
        m_img += byteArray.toBase64();
    }

    QByteArray byteArray2;
    m_ringimg ="";
    QBuffer buffer2(&byteArray2); // use buffer to store pixmap into byteArray
    buffer.open(QIODevice::WriteOnly);
    if(!m_character->rings.isNull()){
        m_character->rings.save(&buffer2, "PNG");
        m_ringimg += byteArray2.toBase64();
    }


    // configure the web view
    ui->webView->setContextMenuPolicy(Qt::NoContextMenu);
    ui->webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, false);

    const QString html = generateHtml();
    ui->webView->setHtml(html);

}



RenderDialog::~RenderDialog()
{
    delete tempFile;
    delete ui;
}

void RenderDialog::setTemplate(const QString filename){
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        qDebug()<<filename + " unable to be opened.";
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    const QDataStream stream(&file);
    QString html_template = "";
    while (!file.atEnd()) {
        QString input_line = file.readLine();
        html_template += input_line;
    }
    m_template = html_template;
}

QString RenderDialog::generateHtml() {
    QString html = m_template;
    QString skilltable = "";
    QString abiltable = "";
    QString abiltable2 = "";
    QString titletable = "";
    QString currictable = "";
    QString secstattable = "";
    QString derattrtable = "";
    QString wealthtable = "";
    QString armortable = "";
    QString weaponammotable = "";
    QString geartable = "";
    QString techTable = "";
    QString distTable = "";
    QString adverTable = "";
    QString passTable = "";
    QString anxiTable = "";
    QString techTable2 = "";

    QString advdisadvtable = "";
    QString titlelisttable = "";


    foreach(const QStringList skillLine,m_character->skills){
        const QString skill = skillLine[0];
        const QString rank = skillLine[1];
        const QString group = skillLine[2];
        if(rank == "0" && ui->hideskill_checkbox->isChecked()) continue;
        skilltable+= "<div class=\"divTableRow\">"
                "<div class=\"divTableCell\">" + skill.toHtmlEscaped() + "</div>" +
                "<div class=\"divTableCell\">" + rank.toHtmlEscaped() + "</div>" +
                "<div class=\"divTableCell\">" + group.toHtmlEscaped() + "</div>" +
                "</div>";

    }

    //reptable->wealthtable
    wealthtable+= "<div class=\"divTableCell\">"+m_character->koku+"</div>";
    wealthtable+= "<div class=\"divTableCell\">"+m_character->bu+"</div>";
    wealthtable+= "<div class=\"divTableCell\">"+m_character->zeni+"</div>";
    //secstattable
    {
        secstattable+="<div class=\"divTableCell\">"+m_character->honor.toHtmlEscaped()+"</div>";
        secstattable+="<div class=\"divTableCell\">"+m_character->glory.toHtmlEscaped()+"</div>";
        secstattable+="<div class=\"divTableCell\">"+m_character->status.toHtmlEscaped()+"</div>";
        secstattable+="</div>";
    }
    //apttable->derattrtable
    {
        derattrtable+="<div class=\"divTableCell\">"+m_character->focus.toHtmlEscaped()+"</div>";
        derattrtable+="<div class=\"divTableCell\">"+m_character->vigilance.toHtmlEscaped()+"</div>";
        derattrtable+="<div class=\"divTableCell\">"+m_character->endurance.toHtmlEscaped()+"</div>";
        derattrtable+="<div class=\"divTableCell\">""</div>";
        derattrtable+="<div class=\"divTableCell\">"+m_character->composure.toHtmlEscaped()+"</div>";
        derattrtable+="<div class=\"divTableCell\">""</div>";
        derattrtable+="</div>";
    }
    foreach(const QStringList equipment, m_character->weapons){
        const QString weapon = equipment[Equipment::NAME];
        const QString type = equipment[Equipment::W_CATEGORY];
        const QString ref= equipment[Equipment::BOOK] + " " + equipment[Equipment::PAGE];
        const QString grip = equipment[Equipment::W_GRIP];
        const QString skill = equipment[Equipment::W_SKILL];
        const QString range = equipment[Equipment::W_MINRANGE] + "-" + equipment[Equipment::W_MAXRANGE];
        const QString dam = equipment[Equipment::W_DAM];
        const QString dls = equipment[Equipment::W_DLS];
        const QString qualities = equipment[Equipment::QUALITIES];
        weaponammotable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+weapon.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+type.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+ref.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+grip.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+skill.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+range.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+dam.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+dls.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+qualities.toHtmlEscaped() + "</div>"+
                                "</div>";
    }

    foreach(const QStringList ability, m_character->abilities){
        if(ability.count()<=0) continue;
        const QString aname = ability[Abilities::NAME];
        const QString asource = ability[Abilities::SOURCE];
        const QString aref = ability[Abilities::REF_BOOK] + " " + ability[Abilities::REF_PAGE];
        const QString adesc = ability[Abilities::DESCRIPTION];
        abiltable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+aname.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+asource.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+aref.toHtmlEscaped() + "</div>"+
                                "</div>";
        abiltable2+=

                "<div style=\"float:right; width: 100%; margin-bottom: 10px;page-break-inside: avoid\">                                                  "
                "   <div class=\"divTable redTable\" style=\"width: 100%\">                                                                     "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">"+aname.toHtmlEscaped()+"</div>                                       "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "   </div>                                                                                                                      "
                "   <div class=\"divTable redTable\" style=\" width: 100%;\">                                                                   "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">"+asource.toHtmlEscaped()+"</div>                                                                   "
                "               <div class=\"divTableHead\">"+aref.toHtmlEscaped()+"</div>                                                                 "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "    <div class=\"divTableBody\">                                                                                               "
                "    </div>                                                                                                                     "
                "</div>                                                                                                                         "
                "<div class=\"divTable redTable\" style=\"width: 100%\">                                                                        "
                "   <div class=\"divTableRow\"><div class=\"divTableCell\" style=\"height:100px;\">"+newlineToBR(adesc.toHtmlEscaped())+"</div>             "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "<p>";
                    /*
                "<div style=\"float:right; width: 100%; margin-bottom: 10px\">      "
                "    <div class=\"divTable redTable\" style=\"width: 100%;\">       "
                "        <div class=\"divTableHeading\">                            "
                "            <div class=\"divTableRow\">                            "
                "                <div class=\"divTableHead\">                       "
                "                    Abilities                                      "
                "                </div>                                             "
                "            </div>                                                 "
                "        </div>                                                     "
                "    </div>                                                         "
                "    <div class=\"divTable redTable\" style=\" width: 100%;\">      "
                "        <div class=\"divTableHeading\">                            "
                "            <div class=\"divTableRow\">                            "
                "                <div class=\"divTableHead\">                       "
                "                    "+aname.toHtmlEscaped()+"                      "
                "                </div>                                             "
                "                <div class=\"divTableHead\">                       "
                "                   "+asource.toHtmlEscaped()+"                     "
                "                </div>                                             "
                "            </div>                                                 "
                "        </div>                                                     "
                "        <div class=\"divTableBody\">                               "
                "            "+adesc.toHtmlEscaped()+"                              "
                "        </div>                                                     "
                "    </div>                                                         "
                "</div>                                                             "
                "<p>"
                "</p>";
                        */

    }

    foreach(const QStringList technique, m_character->techniques){
        if(technique.count()<=0) continue;
        const QString tname = technique[Tech::NAME];
        const QString ttype = technique[Tech::TYPE];
        const QString tsubtype = technique[Tech::SUBTYPE];
        const QString trank = technique[Tech::RANK];
        const QString tref = technique[Tech::BOOK]+ " " + technique[Tech::PAGE];
        const QString tdesc = tref+" "+technique[Tech::DESCRIPTION];
        techTable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+tname.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+ttype.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+tsubtype.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+tref.toHtmlEscaped() + "</div>"+
                                "</div>";

        techTable2 +=
        "<div style=\"float:left; width: 43%; margin: 10px;page-break-inside: avoid\">                                                 "
        "   <div class=\"divTable redTable\" style=\"width: 100%\">                                           "
        "       <div class=\"divTableHeading\">                                                               "
        "           <div class=\"divTableRow\">                                                               "
        "               <div class=\"divTableHead\">"+tname.toHtmlEscaped()+"</div>                                       "
        "           </div>                                                                                    "
        "       </div>                                                                                        "
        "   </div>                                                                                            "
        "   <div class=\"divTable redTable\" style=\" width: 100%;\">                                         "
        "       <div class=\"divTableHeading\">                                                               "
        "           <div class=\"divTableRow\">                                                               "
        "               <div class=\"divTableHead\">RANK "+trank.toHtmlEscaped()+" </div>                     "
        "               <div class=\"divTableHead\">"+ttype.toHtmlEscaped()+" ("+tsubtype.toHtmlEscaped()+")</div>  "
        "           </div>                                                                                    "
        "       </div>                                                                                        "
        "    <div class=\"divTableBody\">                                                                     "
        "    </div>                                                                                           "
        "</div>                                                                                               "
        "<div class=\"divTable redTable\" style=\"width: 100%\">                                              "
        "   <div class=\"divTableRow\"><div class=\"divTableCell\" style=\"height:215px;\">"+newlineToBR(tdesc.toHtmlEscaped())+"</div>  "
        "</div>                                                                                               "
        "</div>                                                                                               "
        "</div>                                                                                               "
        "<p>";
    }


    foreach(const QStringList str, m_character->distinctions){
        const QString adname = str[Adv_Disadv::NAME];
        const QString adring = str[Adv_Disadv::RING];
        const QString adref = str[Adv_Disadv::BOOK]+ " " + str[Adv_Disadv::PAGE];
        const QString addesc = adref + ":\n" + str[Adv_Disadv::DESC];
        distTable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+adname.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adring.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+addesc.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adref.toHtmlEscaped() + "</div>"+
                                "</div>";
        advdisadvtable+=
                "<div style=\"float:left; width: 43%; margin: 10px;page-break-inside: avoid\">                                                  "
                "   <div class=\"divTable redTable\" style=\"width: 100%\">                                                                     "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">"+adname.toHtmlEscaped()+" ("+adring+")</div>                                       "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "   </div>                                                                                                                      "
                "   <div class=\"divTable redTable\" style=\" width: 100%;\">                                                                   "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">Distinction</div>                                                                   "
                "               <div class=\"divTableHead\">Reroll 2 dice</div>                                                                 "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "    <div class=\"divTableBody\">                                                                                               "
                "    </div>                                                                                                                     "
                "</div>                                                                                                                         "
                "<div class=\"divTable redTable\" style=\"width: 100%\">                                                                        "
                "   <div class=\"divTableRow\"><div class=\"divTableCell\" style=\"height:150px;\">"+newlineToBR(addesc.toHtmlEscaped())+"</div>             "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "<p>";

    }

    foreach(const QStringList str, m_character->adversities){
        const QString adname = str[Adv_Disadv::NAME];
        const QString adring = str[Adv_Disadv::RING];
        const QString adref = str[Adv_Disadv::BOOK]+ " " + str[Adv_Disadv::PAGE];
        const QString addesc = adref + ":\n" + str[Adv_Disadv::DESC];
        distTable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+adname.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adring.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+addesc.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adref.toHtmlEscaped() + "</div>"+
                                "</div>";
        advdisadvtable+=
                "<div style=\"float:left; width: 43%; margin: 10px;page-break-inside: avoid\">                                                  "
                "   <div class=\"divTable redTable\" style=\"width: 100%\">                                                                     "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">"+adname.toHtmlEscaped()+" ("+adring+")</div>                                       "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "   </div>                                                                                                                      "
                "   <div class=\"divTable redTable\" style=\" width: 100%;\">                                                                   "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">Adversity</div>                                                                     "
                "               <div class=\"divTableHead\">Reroll 2 successes; gain Void Point on fail</div>                           "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "    <div class=\"divTableBody\">                                                                                               "
                "    </div>                                                                                                                     "
                "</div>                                                                                                                         "
                "<div class=\"divTable redTable\" style=\"width: 100%\">                                                                        "
                "   <div class=\"divTableRow\"><div class=\"divTableCell\" style=\"height:150px;\">"+newlineToBR(addesc.toHtmlEscaped())+"</div>             "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "<p>";

    }

    foreach(const QStringList str, m_character->passions){
        const QString adname = str[Adv_Disadv::NAME];
        const QString adring = str[Adv_Disadv::RING];
        const QString adref = str[Adv_Disadv::BOOK]+ " " + str[Adv_Disadv::PAGE];
        const QString addesc = adref + ":\n" + str[Adv_Disadv::DESC];
        distTable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+adname.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adring.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+addesc.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adref.toHtmlEscaped() + "</div>"+
                                "</div>";
        advdisadvtable+=
                "<div style=\"float:left; width: 43%; margin: 10px;page-break-inside: avoid\">                                                  "
                "   <div class=\"divTable redTable\" style=\"width: 100%\">                                                                     "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">"+adname.toHtmlEscaped()+" ("+adring+")</div>                                       "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "   </div>                                                                                                                      "
                "   <div class=\"divTable redTable\" style=\" width: 100%;\">                                                                   "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">Passion</div>                                                                       "
                "               <div class=\"divTableHead\">Recover 3 strife</div>                                                              "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "    <div class=\"divTableBody\">                                                                                               "
                "    </div>                                                                                                                     "
                "</div>                                                                                                                         "
                "<div class=\"divTable redTable\" style=\"width: 100%\">                                                                        "
                "   <div class=\"divTableRow\"><div class=\"divTableCell\" style=\"height:150px;\">"+newlineToBR(addesc.toHtmlEscaped())+"</div>             "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "<p>";

    }

    foreach(const QStringList str, m_character->anxieties){
        const QString adname = str[Adv_Disadv::NAME];
        const QString adring = str[Adv_Disadv::RING];
        const QString adref = str[Adv_Disadv::BOOK]+ " " + str[Adv_Disadv::PAGE];
        const QString addesc = adref + ":\n" + str[Adv_Disadv::DESC];
        distTable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+adname.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adring.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+addesc.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+adref.toHtmlEscaped() + "</div>"+
                                "</div>";
        advdisadvtable+=
                "<div style=\"float:left; width: 43%; margin: 10px;page-break-inside: avoid\">                                                  "
                "   <div class=\"divTable redTable\" style=\"width: 100%\">                                                                     "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">"+adname.toHtmlEscaped()+" ("+adring+")</div>                                       "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "   </div>                                                                                                                      "
                "   <div class=\"divTable redTable\" style=\" width: 100%;\">                                                                   "
                "       <div class=\"divTableHeading\">                                                                                         "
                "           <div class=\"divTableRow\">                                                                                         "
                "               <div class=\"divTableHead\">Anxiety</div>                                                                       "
                "               <div class=\"divTableHead\">Suffer 3 strife; 1/scene gain a Void Point</div>                                    "
                "           </div>                                                                                                              "
                "       </div>                                                                                                                  "
                "    <div class=\"divTableBody\">                                                                                               "
                "    </div>                                                                                                                     "
                "</div>                                                                                                                         "
                "<div class=\"divTable redTable\" style=\"width: 100%\">                                                                        "
                "   <div class=\"divTableRow\"><div class=\"divTableCell\" style=\"height:150px;\">"+newlineToBR(addesc.toHtmlEscaped())+"</div>             "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "</div>                                                                                                                         "
                "<p>";

    }


    foreach(const QStringList str, m_character->curriculum){
        const QString crank = str[Curric::RANK];
        QString cadvance = str[Curric::ADVANCE];
                if(str[Curric::SPEC]=="1")cadvance +="*";
        const QString ctype = str[Curric::TYPE];
        currictable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+crank.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+cadvance.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+ctype.toHtmlEscaped() + "</div>"+
                                "</div>";
    }
    foreach(const QStringList str, m_character->curTitle){
        const QString csource = str[Title::SOURCE];
        QString cadvance = str[Title::ADVANCE];
                if(str[Title::SPEC]=="1")cadvance +="*";
        const QString ctype = str[Title::TYPE];
        titletable+= "<div class=\"divTableRow\">"
                                "<div class=\"divTableCell\">"+csource.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+cadvance.toHtmlEscaped() + "</div>"+
                                "<div class=\"divTableCell\">"+ctype.toHtmlEscaped() + "</div>"+
                                "</div>";
    }
    foreach(const QStringList armor,m_character->armor){
        armortable+= "<div class=\"divTableRow\"><div class=\"divTableCell\">"+(armor[Equipment::NAME] + " Physical :"+
                armor[Equipment::A_PHYSRES] + " Supernatural: "+ armor[Equipment::A_SUPERRES] + " " +
                "("+armor[Equipment::QUALITIES]).toHtmlEscaped()+")</div></div>";
    }

    bool onedone = false;
    foreach(const QStringList gearLine,m_character->personaleffects){
            geartable+=gearLine[Equipment::NAME] + ", ";
            onedone = true;
    }
    if(onedone) geartable.chop(2);

    onedone = false;
    foreach(const QString tline,m_character->titles){
            titlelisttable+=tline + ", ";
            onedone = true;
    }
    if(onedone) titlelisttable.chop(2);

    //set visibility
    QString portVis = "";
    if(ui->hideportrait_checkbox->isChecked())
             portVis = "display:none";
    else
            portVis = "";

    html.replace("$CHAR_NAME", m_character->family.toHtmlEscaped() + " " + m_character->name.toHtmlEscaped());
    html.replace("$CLAN_NAME", m_character->clan.toHtmlEscaped());
    html.replace("$SCHOOL_NAME", m_character->school.toHtmlEscaped());
    html.replace("$NINJO", newlineToBR(m_character->ninjo.toHtmlEscaped()));
    html.replace("$GIRI", newlineToBR(m_character->giri.toHtmlEscaped()));
    html.replace("$NOTES", newlineToBR(m_character->notes.toHtmlEscaped()));
    html.replace("$HERITAGE", m_character->heritage.toHtmlEscaped());
    html.replace("$PORTIMG", m_img); //base64! calculated in constructor
    html.replace("$RINGIMG", m_ringimg); //base64! calculated in constructor

    html.replace("$SKILLTABLE",skilltable);
    html.replace("$APTTABLE",derattrtable);
    html.replace("$CASHTABLE",wealthtable);
    html.replace("$SECSTATTABLE",secstattable);
    html.replace("$ARMORTABLE",armortable);
    html.replace("$WEAPONTABLE",weaponammotable);
    html.replace("$GEARTABLE",geartable);
    html.replace("$TECHTABLE",techTable);
    html.replace("$DISTTABLE",distTable);
    html.replace("$ADVERTABLE",adverTable);
    html.replace("$PASSTABLE",passTable);
    html.replace("$ANXITABLE",anxiTable);
    html.replace("$CURRTABLE",currictable);
    html.replace("$TITLETABLE",titletable);
    html.replace("$TITLELISTTABLE",titlelisttable);
    html.replace("$ABILTABLE",abiltable2);
    html.replace("$TECHBLOCKS",techTable2);
    html.replace("$TRAITBLOCKS",advdisadvtable);

    html.replace("$TITLESTATUSTEXT",m_character->titleStatus);
    html.replace("$CURRICSTATUSTEXT",m_character->curricStatus);

    html.replace("$PRTVIS",portVis);

    /*
     //create test HTML file
    QFile file("test.html");
    //QTemporaryFile file;
    if (!file.open(QIODevice::WriteOnly |QIODevice::Truncate))
    {
        qDebug()<<"test.html unable to be opened.";
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
    }
    else{

        QTextStream stream(&file);

        stream<< html << endl;

        file.close();
    }
    */

    delete tempFile; //clear the old file
    tempFile = new QTemporaryFile(QDir::tempPath() + "/XXXXXX.printfile.html");
    if (!tempFile->open())
    {
        qDebug()<<"tempfile unable to be opened.";
        QMessageBox::information(this, tr("Unable to open file"), tempFile->errorString());
    }
    else{

        QTextStream stream(tempFile);

        stream<< html << Qt::endl;

        tempFile->close();
    }


    return html;

}

//newlines get lost when injected into HTML.  Convert desc newlines to <br> tags to enable pretty printing.
QString RenderDialog::newlineToBR(QString text){
    text.replace("\r\n","<br>"); //windows, just in case
    text.replace("\n","<br>"); //other
    return text;
}

void RenderDialog::on_printButton_clicked()
{
    //QPrinter printer;
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPageMargins(QMarginsF(0.4,0.4,0.4,0.4),QPageLayout::Inch);
    QPrintDialog *dialog = new QPrintDialog(&printer);
    if ( dialog->exec() == QDialog::Accepted)
            ui->webView->print(&printer);
            //ui->webView->page()->print(&printer, [=](bool){});
}

void RenderDialog::on_cancelButton_clicked()
{
   this->close();
}

void RenderDialog::on_hideskill_checkbox_toggled(const bool checked)
{
    Q_UNUSED(checked);
    ui->webView->setHtml(generateHtml());
}

void RenderDialog::on_hideportrait_checkbox_toggled(const bool checked)
{
    Q_UNUSED(checked);
    ui->webView->setHtml(generateHtml());
}

void RenderDialog::on_browserButton_clicked()
{
    if(tempFile->open() ){
        const QString filename = tempFile->fileName();
        const QUrl url("file:///"+filename);
        QDesktopServices::openUrl ( url );
    }
}
