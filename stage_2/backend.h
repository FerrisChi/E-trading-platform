#ifndef BACKEND_H
#define BACKEND_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <exception>
#include <QDebug>
#include <QApplication>
#include <QMainWindow>
#include <QObject>
#include <QMessageBox>

#include "mainwindow.h"
#include "user.h"
#include "product.h"
#include "chart.h"

class Backend : public QObject
{
    Q_OBJECT

 private:

    // UI interfaces
    QFont ft;
    MainWindow mainWindow;

 private slots:
    void ResetBackend();
    void AddUser(Customer customer);
    void AddUser(Shop shop);
    void LoginUser(Customer customer);
    void LoginUser(Shop shop);

    void ModifyPassword(int userID, int type, std::string password);
    void ModifyProduct(Product product);
    void Recharge(int userID, int type, double amount);
    void Consume(int userID, int type, double amount);
    void AddProduct(int belongID, int stock, int type, double price, std::string name, int feature, std::string description);
    void SearhProduct(std::string description);
    void SearhProduct(int shopID); // search products belong to shopID
    void SetDiscount(int index, double discount);
    void SetDiscount(int shopID, ProductType type, double discount);
    void AddToChart(int customerID, int productID, int number);
    void GetChart(int customerID);
    void ModifyChart(int customerID, int chartID, int num); // modify chartID(th) product in chart( i.e. product[chartID].stock <- num )
    void AddOrder(Chart order); // freeze products in order
    void PayOrder(Chart order); // pay current order
    void FreeOrder(Chart order); // release products in order

 public:
  Backend(QApplication *a);
  ~Backend(){};

  void GetGeneralConfig();
  void SetGeneralConfig();

};

#endif // BACKEND_H
