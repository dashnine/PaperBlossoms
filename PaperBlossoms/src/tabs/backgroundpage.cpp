#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../dialog/edituserdescriptionsdialog.h"
#include <QDialog>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

void MainWindow::on_family_lineEdit_textEdited(const QString &arg1) {
    curCharacter.family = arg1;
    m_dirtyDataFlag = true;
}

void MainWindow::on_name_lineEdit_textChanged(const QString &arg1) {
    curCharacter.name = arg1;
    ui->character_name_label->setText(curCharacter.family + " " + curCharacter.name + ", " + curCharacter.school);
    m_dirtyDataFlag = true;
}

void MainWindow::on_ninjo_textEdit_textChanged() {
    curCharacter.ninjo = ui->ninjo_textEdit->toPlainText();
    m_dirtyDataFlag = true;
}

void MainWindow::on_giri_textEdit_textChanged() {
    curCharacter.giri = ui->giri_textEdit->toPlainText();
    m_dirtyDataFlag = true;
}

void MainWindow::on_notes_textEdit_textChanged() {
    curCharacter.notes = ui->notes_textEdit->toPlainText();
    m_dirtyDataFlag = true;
}

void MainWindow::on_actionDescription_Editor_triggered() {
    EditUserDescriptionsDialog dialog(deps);
    const int result = dialog.exec();
    if (result == QDialog::Accepted) {
        dialog.doFinish(true);
        qDebug() << "Accepted: data comitted";

    } else {
        dialog.doFinish(false);
        qDebug() << "Rejected: data discarded";
    }
}

void MainWindow::on_image_label_clicked() {
    const QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            QDir::homePath(),
            tr("All files (*.*);;JPEG (*.jpg *.jpeg);;PNG (*.png)")
    );

    if (!fileName.isEmpty()) {
        curCharacter.portrait.load(fileName);
        if (curCharacter.portrait.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        const QPixmap p = QPixmap::fromImage(curCharacter.portrait);
        ui->image_label->setPixmap(p.scaled(ui->image_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));


        ui->image_label->adjustSize();
    }
    m_dirtyDataFlag = true;
}
