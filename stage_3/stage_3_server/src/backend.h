#ifndef BACKEND_H
#define BACKEND_H

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QtNetwork>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>

#include "../public/chart.h"
#include "../public/product.h"
#include "../public/user.h"
#include "../public/utils.h"
#include "mainwindow.h"

#define MAX_CLIENT_SIZE 5

class Backend : public QObject {
  Q_OBJECT

 private:
  // Network
  QTcpServer *tcpServer;
  QTcpSocket *tcpSocket;
  QTcpSocket *clientSockets[MAX_CLIENT_SIZE];

  // Cache
  Customer svCustomer;
  Shop svShop;
  Chart svChart;
  QVector<Product> svProductList;

  void ResetBackend();
  void TestTCP(QString qstr);

  void AddUser(Customer customer);
  void AddUser(Shop shop);
  void LoginUser(Customer customer);
  void LoginUser(Shop shop);

  void ModifyPassword(int userID, int type, std::string password);
  void ModifyProduct(Product product);
  void Recharge(int userID, int type, double amount);
  void Consume(int userID, int type, double amount);
  void AddProduct(Product product);
  void SearhProduct(std::string description);
  void SearhProduct(int shopID);  // search products belong to shopID
  void SetDiscount(int index, double discount);
  void SetDiscount(int shopID, int type, double discount);
  void AddToChart(int customerID, int productID, int number);
  void GetChart(int customerID);

  // modify chartID(th) product in chart( i.e. product[chartID].stock <- num )
  void ModifyChart(int customerID, int chartID, int num);

  // freeze products in order
  void AddOrder(Chart order);

  // pay current order
  void PayOrder(Chart order);

  // release products in order
  void FreeOrder(Chart order);

 public:
  Backend(){};
  ~Backend(){};

  void init();

  // Network
  uint8_t clientNumber;
  uint8_t nowClientID;
  QVector<QTcpSocket *> tcpSocketList;

  void sendClient(uint8_t op, uint8_t msgFlag, uint8_t userFlag,
                  uint8_t chartFlag, uint8_t productFlag);

  // UI
  MainWindow mainWindow;

  void GetGeneralConfig();
  void SetGeneralConfig();

 public slots:
  //  void acceptConnection();
  void readClient();
  void disconnetClient();
};

#endif  // BACKEND_H
