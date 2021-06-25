#include <QApplication>

#include "mainwindow.h"
#include "server.h"

MainWindow *mainWindow = NULL;
// Log
void logOutput(QtMsgType type, const QMessageLogContext &context,
               const QString &msg) {
  QString logText;
  switch (type) {
    case QtMsgType::QtDebugMsg:
      logText.append("Log:");
      break;

    case QtMsgType::QtWarningMsg:
      logText.append("Warning:");
      break;

    case QtMsgType::QtCriticalMsg:
      logText.append("Critical:");
      break;

    case QtMsgType::QtFatalMsg:
      logText.append("Fatal:");
      break;

    case QtMsgType::QtInfoMsg:
      logText.append("Info:");
      break;
  }
  logText.append(msg);
  logText.append(" [" +
                 QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
                 ']' + '\n');
  mainWindow->showLog(logText);
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QFont ft;
  ft.setFamily("MS Shell Dlg 2");
  a.setFont(ft);
  a.setApplicationDisplayName("E-trading-paltform");
  Server server;
  mainWindow = &(server.BN.mainWindow);
  qInstallMessageHandler(logOutput);
  server.BN.init();
  return a.exec();
}
