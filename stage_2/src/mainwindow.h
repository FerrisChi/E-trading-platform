#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QCheckBox>
#include "product.h"
#include "user.h"
#include "chart.h"
#include "modifyproductwindow.h"
#include "addproductwindow.h"
#include "loginwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ModifyProductWindow modifyProductWindow;
    AddProductWindow addProductWindow;
    LoginWindow loginWindow;

    int payState;
    UserType loginState;
    // customer in cache
    Customer customer;
    // shop in cache
    Shop shop;
    // chart in cache
    Chart chart;
    // order in cache
    Chart order;
    // current pay amount
    double pay;
    // search list in cache
    QVector<Product>searchList;
    // manage list in cache
    QVector<Product>manageList;

    void init();
    void setLogin(Customer customer);
    void setLogin(Shop shop);
    void setRemaining(double amount);
    void setSerchList(QVector<Product> productList);
    void setManageList(QVector<Product> productList);
    void setChart(Chart chart);
    void setOrder();
    void setOrderState(int flag);
    void setPayState(int flag, Customer customer);

signals:
    void sigLogIn(std::string nickname, std::string password);
    void sigSignUp(std::string nickname, std::string password, int type);
    void sigResetBackend();
    void sigModifyWindow(Product pdt);
    void sigLoginWindow();
    void sigAddProductWindow(int shopID);
    void sigLogout();
    void sigModifyPassword(int userID, int type, std::string password);
    void sigRecharge(int userID, int type, double amount);
    void sigSearchProduct(std::string description);
    void sigSearchProduct(int shopID); // search productions of current shop
    void sigSetDiscount(int index, double discount);
    void sigSetDiscount(int shopID, ProductType type, double discount);
    void sigAddToChart(int customerID, int productID, int num);
    void sigGetChart(int customerID);
    void sigModifyChart(int customerID, int chartID, int num);
    void sigAddOrder(Chart order);
    void sigPayOrder(Chart order);
    void sigFreeOrder(Chart order);

private slots:

    void on_resetBackendButton_clicked();

    void on_loginButton_clicked();

    void on_modifypasswordButton_clicked();

    void on_chargeButton_clicked();

    void on_myButton_clicked();

    void on_mainPageButton_clicked();

    void on_orderPageButton_clicked();

    void on_addProductButton_clicked();

    void on_searchButton_clicked();

    void on_manageTableView_doubleClicked(const QModelIndex &index);

    void on_discountButton_clicked();

    void on_addToChartButton_clicked();

    void on_modifyChartButton_clicked();

    void on_orderButton_clicked();

    void on_payButton_clicked();

    void on_MainWindow_destroyed();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_cancelButton_clicked();

    void on_allProductButton_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* searchModel;
    QStandardItemModel* chartModel;
    QStandardItemModel* manageModel;
    QStandardItemModel* orderModel;

    QStandardItem *chartItem[5];
};
#endif // MAINWINDOW_H
