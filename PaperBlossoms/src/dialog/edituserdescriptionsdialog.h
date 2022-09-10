#ifndef EDITUSERDESCRIPTIONSDIALOG_H
#define EDITUSERDESCRIPTIONSDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include "../dependency/databasedependency.h"

namespace Ui {
class EditUserDescriptionsDialog;
}

class EditUserDescriptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditUserDescriptionsDialog(DatabaseDependency* deps, QWidget *parent = nullptr);
    ~EditUserDescriptionsDialog();
    void doFinish(bool accepted);

private slots:
    void on_pushButton_clicked();


    void on_apply_pushbutton_clicked();

    void dataChanged();

    void on_delete_pushButton_clicked();

    void on_optionComboBox_currentIndexChanged(int index);

private:
    Ui::EditUserDescriptionsDialog *ui;
    QSqlTableModel* model;
    DatabaseDependency* deps;
};

#endif // EDITUSERDESCRIPTIONSDIALOG_H
