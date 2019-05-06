#include "edituserdescriptionsdialog.h"
#include "ui_edituserdescriptionsdialog.h"
#include "dataaccesslayer.h"
#include <QSqlField>
#include <QSqlRecord>

EditUserDescriptionsDialog::EditUserDescriptionsDialog(DataAccessLayer* dal, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditUserDescriptionsDialog)
{
    ui->setupUi(this);
    this->dal = dal;

    this->model = new QSqlTableModel();
    model->setTable("user_descriptions");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Field"));
    model->setHeaderData(1, Qt::Horizontal, tr("Description"));
    model->setHeaderData(2, Qt::Horizontal, tr("Short Desc"));

    ui->descTableView->setModel(this->model);
    ui->optionComboBox->addItems(dal->qsl_getdescribablenames());
}

EditUserDescriptionsDialog::~EditUserDescriptionsDialog()
{
    delete ui;
}

void EditUserDescriptionsDialog::doFinish(bool accepted)
{
    if(accepted){
        model->submitAll();
    }
    else{
        model->revertAll();
    }
}

void EditUserDescriptionsDialog::on_pushButton_clicked()
{
    QSqlField namefield("name",QVariant::String);
    namefield.setValue(ui->optionComboBox->currentText().toHtmlEscaped());  // casts QString to int
    QSqlField descfield("description",QVariant::String);
    descfield.setValue(ui->textEdit->toPlainText().toHtmlEscaped());  // casts QString to int
    QSqlField sdescfield("short_desc",QVariant::String);
    sdescfield.setValue(ui->lineEdit->text().toHtmlEscaped());  // casts QString to int

    QSqlRecord record;
    record.append(namefield);
    record.append(descfield);
    record.append(sdescfield);

    model->insertRecord(-1,record);
}
