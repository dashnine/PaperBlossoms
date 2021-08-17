#include "dblocalisationeditordialog.h"
#include "ui_dblocalisationeditordialog.h"
#include <QSqlField>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QStandardItemModel>

DBLocalisationEditorDialog::DBLocalisationEditorDialog(DatabaseDependency* deps,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBLocalisationEditorDialog)
{
    ui->setupUi(this);

    this->deps = deps;


    this->model = new QSqlTableModel();
    model->setTable("i18n");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("string"));
    model->setHeaderData(1, Qt::Horizontal, tr("string_tr"));

    this->simodel = new QStandardItemModel;

    foreach (QStringList row, deps->dal->ql_gettrtemplate()) {

        //generate row and add it to model
        QList<QStandardItem*> itemrow;
        itemrow << new QStandardItem(row.at(0));
        itemrow << new QStandardItem(row.at(1));
        simodel->appendRow(itemrow);
    }

    ui->descTableView->setModel(this->model);
    //ui->optionComboBox->addItems(dal->qsl_getdescribablenames());
    ui->apply_pushbutton->setEnabled(false);
    ui->descTableView->resizeColumnToContents(1);
    connect(model,SIGNAL(dataChanged (const QModelIndex &, const QModelIndex &)),this,SLOT(dataChanged()));
    //ui->optionComboBox->setCurrentIndex(-1);
    //ui->label->setVisible(false);

    //ui->descTableView->horizontalHeader()->swapSections(1,2);
    ui->descTableView->horizontalHeader()->setStretchLastSection(true);
    ui->descTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //Horrible hack to make row resizing work right.  Make this not so, plz.
    QTimer::singleShot(1, ui->descTableView, SLOT(resizeRowsToContents()));
}

DBLocalisationEditorDialog::~DBLocalisationEditorDialog()
{
    delete ui;
    delete model;
}

void DBLocalisationEditorDialog::doFinish(bool accepted)
{

    if(accepted){
        model->submitAll();
    }
    else{
        model->revertAll();
    }

}


void DBLocalisationEditorDialog::on_apply_pushbutton_clicked()
{

    model->submitAll();
    for(int i=0; i<model->rowCount(); ++i){
        ui->descTableView->showRow(i);
    }
    ui->apply_pushbutton->setEnabled(false);
    //ui->label->setVisible(true);

}

void DBLocalisationEditorDialog::dataChanged()
{
    ui->apply_pushbutton->setEnabled(true);
    //ui->label->setVisible(true);

}


void DBLocalisationEditorDialog::on_buttonBox_accepted()
{
    qDebug()<<"accepted";
    this->accept();
}

void DBLocalisationEditorDialog::on_buttonBox_rejected()
{
    this->reject();
}
