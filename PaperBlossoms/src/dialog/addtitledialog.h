#ifndef ADDTITLEDIALOG_H
#define ADDTITLEDIALOG_H

#include <QDialog>
#include "../dependency/databasedependency.h"
#include "../character.h"

namespace Ui {
class AddTitleDialog;
}

class AddTitleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTitleDialog(DatabaseDependency* deps, Character *character, QWidget *parent = 0);
    ~AddTitleDialog();

    QString getResult() const;
private slots:
    void on_title_combobox_currentIndexChanged(const QString &arg1);

private:
    Ui::AddTitleDialog *ui;
    DatabaseDependency* deps;
    Character* character;

    QString title;
};

#endif // ADDTITLEDIALOG_H
