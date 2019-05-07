#include "edituserdescriptionsdialog.h"
#include "ui_edituserdescriptionsdialog.h"
#include "dataaccesslayer.h"
#include <QSqlField>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTimer>

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
    ui->apply_pushbutton->setEnabled(false);
    ui->descTableView->resizeColumnToContents(2);
    connect(model,SIGNAL(dataChanged (const QModelIndex &, const QModelIndex &)),this,SLOT(dataChanged()));
    ui->optionComboBox->setCurrentIndex(-1);
    ui->label->setVisible(false);

    ui->descTableView->horizontalHeader()->swapSections(1,2);
    ui->descTableView->horizontalHeader()->setStretchLastSection(true);
    ui->descTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //Horrible hack to make row resizing work right.  Make this not so, plz.
    QTimer::singleShot(1, ui->descTableView, SLOT(resizeRowsToContents()));
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
    //TODO: Move this into DAL?
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
    ui->apply_pushbutton->setEnabled(true);
    ui->descTableView->resizeColumnToContents(2);

    //clear the data
    ui->lineEdit->clear();
    ui->textEdit->setText("");
    ui->optionComboBox->setCurrentIndex(-1);

}

void EditUserDescriptionsDialog::on_apply_pushbutton_clicked()
{
    model->submitAll();
    for(int i=0; i<model->rowCount(); ++i){
        ui->descTableView->showRow(i);
    }
    ui->apply_pushbutton->setEnabled(false);
    ui->label->setVisible(true);
}

void EditUserDescriptionsDialog::dataChanged()
{
    ui->apply_pushbutton->setEnabled(true);
    //ui->label->setVisible(true);

}

void EditUserDescriptionsDialog::on_delete_pushButton_clicked()
{
    const QModelIndex curIndex = ui->descTableView->currentIndex();

    if(curIndex.isValid()){
        if(QMessageBox::Cancel==QMessageBox::information(this, tr("Warning: Unreversable Change"), "Warning: This will immediately save all changes. This action cannot be undone. Continue?",QMessageBox::Yes|QMessageBox::Cancel)){
            return;
        }
        //ui->descTableView->hideRow(curIndex.row());
        model->removeRow(curIndex.row());
        model->submitAll();
        ui->apply_pushbutton->setEnabled(false);

    }
    ui->label->setVisible(true);

}

void EditUserDescriptionsDialog::on_optionComboBox_currentIndexChanged(int index)
{
    ui->pushButton->setEnabled(index>=0);
}
