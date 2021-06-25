#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent) {
  this->listen(QHostAddress::Any, 8888);
  BN.clientNumber = 0;
}

void Server::incomingConnection(qintptr socketDescriptor) {
  QTcpSocket *tcpclientsocket =
      new QTcpSocket(this);  //只要有新的连接就生成一个新的通信套接字
  tcpclientsocket->setSocketDescriptor(socketDescriptor);

  BN.tcpSocketList.push_back(tcpclientsocket);

  //接收到tcpclientsocket发送过来的更新界面的信号
  connect(tcpclientsocket, &QTcpSocket::readyRead, &this->BN,
          &Backend::readClient);
  connect(tcpclientsocket, &QTcpSocket::disconnected, &this->BN,
          &Backend::disconnetClient);

  //    BN.sendClient(CONNECT, BN.clientNumber++, 2, 2, 2);
}
