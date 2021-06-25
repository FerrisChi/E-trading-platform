#ifndef SERVER_H
#define SERVER_H

#include <backend.h>

#include <QObject>
#include <QtNetwork>

class Server : public QTcpServer {
  Q_OBJECT
 public:
  explicit Server(QObject *parent = nullptr);
  Backend BN;

 public slots:
  void incomingConnection(qintptr socketDescriptor);
};

#endif  // SERVER_H
