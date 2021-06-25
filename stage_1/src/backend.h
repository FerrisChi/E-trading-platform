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

class Backend : public QObject
{
    Q_OBJECT

 private:

    UserType nowState;
    Customer customer;
    Shop shop;
    int cusIndex;
    int shopIndex;
    Product *products;

    // UI interfaces
    QFont ft;
    MainWindow mainWindow;

 private slots:
    void ResetBackend();
    void AddUser(std::string nickname, std::string password, int type);
    void LoginUser(std::string nickname, std::string password);
    void Logout();
    void ModifyPassword(std::string password);
    void ModifyProduct(int index, std::string name, std::string description, int stock, double price, double discount);
    void Recharge(double amount);
    void Consume(double amount);
    void AddProduct(std::string name, std::string description, int type, double price, int stock, int feature);
    void SearhProduct(std::string description);
    void SearhProduct();
    void SetDiscount(int index, double discount);
    void SetDiscount(ProductType type, double discount);


 public:
  Backend(QApplication *a);
  ~Backend(){};

};

#endif // BACKEND_H
