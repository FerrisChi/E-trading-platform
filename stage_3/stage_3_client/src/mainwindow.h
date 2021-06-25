#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QtNetwork>

#include "../public/chart.h"
#include "../public/product.h"
#include "../public/user.h"
#include "../public/utils.h"
#include "addproductwindow.h"
#include "loginwindow.h"
#include "modifyproductwindow.h"

#define MAX_CLIENT_SIZE 5

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  // UI windows
  ModifyProductWindow modifyProductWindow;
  AddProductWindow addProductWindow;
  LoginWindow loginWindow;

  // Cache
  int payState;
  UserType loginState;
  Customer customer;
  Shop shop;
  Chart chart;
  Chart order;
  double pay;
  QVector<Product> searchList;
  QVector<Product> manageList;

 public slots:
  // handle data received from server
  void ReciveData();
  // send request data to server
  void SendData(QByteArray sendMsg);

 signals:
  void sigModifyWindow(Product pdt);
  void sigLoginWindow();
  void sigAddProductWindow(int shopID);

 private slots:

  // send reset message
  void on_resetBackendButton_clicked();

  // show login page
  void on_loginButton_clicked();

  // send modify password message
  void on_modifypasswordButton_clicked();

  // send recharge message
  void on_chargeButton_clicked();

  // show person page
  void on_myButton_clicked();

  // show main page
  void on_mainPageButton_clicked();

  // show order/manage page
  void on_orderPageButton_clicked();

  // send add production message
  void on_addProductButton_clicked();

  // send search product message
  void on_searchButton_clicked();

  // show modify product page
  void on_manageTableView_doubleClicked(const QModelIndex &index);

  // send discount message
  void on_discountButton_clicked();

  // send add product to chart message
  void on_addToChartButton_clicked();

  // send modify chart message
  void on_modifyChartButton_clicked();

  // send give order message
  void on_orderButton_clicked();

  // send pay message
  void on_payButton_clicked();

  // check pay status and send free order message
  void on_MainWindow_destroyed();

  // print now user info
  void on_pushButton_clicked();

  // print now shopping chart info
  void on_pushButton_2_clicked();

  // print now order info
  void on_pushButton_3_clicked();

  // send cancel order message
  void on_cancelButton_clicked();

  // show all product
  void on_allProductButton_clicked();

  // send test message
  void on_testTCPButton_clicked();

  // send add product message
  void AddProduct(Product p);

  // send modify product message
  void ModifyProduct(Product p);

  // send customer login message
  void Login(Customer customer);

  void Login(Shop shop);

  void Signup(Customer customer);

  void Signup(Shop shop);

 private:
  Ui::MainWindow *ui;
  QStandardItemModel *searchModel;
  QStandardItemModel *chartModel;
  QStandardItemModel *manageModel;
  QStandardItemModel *orderModel;
  QStandardItem *chartItem[5];

  QTcpSocket *tcpSocket;

  uint8_t clientID;

  // initialize main UI
  void uiInit();

  // set Customer state
  void setCustomer();

  // set Shop state
  void setShop();

  // set Search page
  void setSerchList();

  // set manage page
  void setManageList();

  // set shopping chart page
  void setChart();

  // set order
  void setOrder();

  // set paying status
  // 0: error; 1: payed; 2: canceled
  void setPayState(int flag);

  // search product with {description} in name or description
  void searchProduct(QString description);

  // search product poccessed by shopID
  void searchProduct(int shopID);
};
#endif  // MAINWINDOW_H
