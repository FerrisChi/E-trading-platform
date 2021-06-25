#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include "product.h"
#include "user.h"
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

    int login_state; // 0: NuLL; 1: Customer; 2: Shop

    QVector<Product>searchList;
    QVector<Product>manageList;

    void init();
    void setLogin(User* usr);
    void setLogout();
    void setRemaining(double amount);
    void setNickname(QString Qstr);
    void setSerchList(QVector<Product> productList);
    void setManageList(QVector<Product> productList);

signals:
    void sigLogIn(std::string nickname, std::string password);
    void sigSignUp(std::string nickname, std::string password, int type);
    void sigResetBackend();
    void sigModifyWindow(Product pdt);
    void sigLoginWindow();
    void sigAddProductWindow();
    void sigLogout();
    void sigModifyPassword(std::string password);
    void sigRecharge(double amount);
    void sigSearchProduct(std::string description);
    void sigSearchProduct(); // search productions of current shop
    void sigSetDiscount(int index, double discount);
    void sigSetDiscount(ProductType type, double discount);

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

private:
    Ui::MainWindow *ui;
    QStandardItemModel* searchModel;
    QStandardItemModel* chartModel;
    QStandardItemModel* manageModel;
};
#endif // MAINWINDOW_H
