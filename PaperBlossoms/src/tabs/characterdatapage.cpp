#include "../mainwindow.h"
#include <QDebug>

void MainWindow::on_glory_spinBox_valueChanged(const int arg1) {
    curCharacter.glory = arg1;
    m_dirtyDataFlag = true;
}

void MainWindow::on_honor_spinBox_valueChanged(const int arg1) {
    curCharacter.honor = arg1;
    m_dirtyDataFlag = true;
}

void MainWindow::on_status_spinBox_valueChanged(const int arg1) {
    curCharacter.status = arg1;
    m_dirtyDataFlag = true;
}