#ifndef DBLOCALISATIONEDITORDIALOG_H
#define DBLOCALISATIONEDITORDIALOG_H

#include <QDialog>
#include "dataaccesslayer.h"
#include <QStandardItemModel>

namespace Ui {
class DBLocalisationEditorDialog;
}

class DBLocalisationEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBLocalisationEditorDialog(DataAccessLayer *dal, QWidget *parent = nullptr);
    ~DBLocalisationEditorDialog();
    void doFinish(bool accepted);

private slots:


    void on_apply_pushbutton_clicked();

    void dataChanged();



    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DBLocalisationEditorDialog *ui;
    QSqlTableModel* model;
    QStandardItemModel* simodel;
    DataAccessLayer* dal;

};

#endif // DBLOCALISATIONEDITORDIALOG_H
