#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  logText.clear();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showLog(QString text) {
  logText.append(text);
  ui->textEdit->setText(logText);
}
