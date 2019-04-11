#ifndef ADDTITLEDIALOG_H
#define ADDTITLEDIALOG_H

#include <QDialog>
#include "dataaccesslayer.h"
#include "character.h"

namespace Ui {
class AddTitleDialog;
}

class AddTitleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTitleDialog(DataAccessLayer *dal, Character *character, QWidget *parent = 0);
    ~AddTitleDialog();

    QString getResult() const;
private slots:
    void on_title_combobox_currentIndexChanged(const QString &arg1);

private:
    Ui::AddTitleDialog *ui;
    DataAccessLayer* dal;
    Character* character;

    QString title;
};

#endif // ADDTITLEDIALOG_H
