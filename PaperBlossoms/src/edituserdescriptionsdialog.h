#ifndef EDITUSERDESCRIPTIONSDIALOG_H
#define EDITUSERDESCRIPTIONSDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include "dataaccesslayer.h"

namespace Ui {
class EditUserDescriptionsDialog;
}

class EditUserDescriptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserDescriptionsDialog(DataAccessLayer* dal, QWidget *parent = nullptr);
    ~EditUserDescriptionsDialog();
    void doFinish(bool accepted);

private slots:
    void on_pushButton_clicked();


    void on_apply_pushbutton_clicked();

    void dataChanged();

    void on_delete_pushButton_clicked();

private:
    Ui::EditUserDescriptionsDialog *ui;
    QSqlTableModel* model;
    DataAccessLayer* dal;
};

#endif // EDITUSERDESCRIPTIONSDIALOG_H
