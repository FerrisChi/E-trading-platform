#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationDisplayName("E-trading-platform");
  QFont ft;
  ft.setFamily("MS Shell Dlg 2");
  a.setFont(ft);
  MainWindow w;
  w.show();
  return a.exec();
}
