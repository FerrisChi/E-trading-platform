#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_okButton_clicked()
{
    if(ui->nickname->text().isEmpty()) throw std::logic_error("Nickname cannot be empty");
    if(ui->password->text().isEmpty()) throw std::logic_error("Password cannot be empty");
    this->close();
    int type = ui->shopButton->isChecked();
    std::string nickname = ui->nickname->text().toStdString();
    std::string password = ui->password->text().toStdString();
    if(!type) { // Customer
        Customer customer(0, nickname, password);
        emit sigLogIn(customer);
    } else { // Shop
        Shop shop(0, nickname, password);
        emit sigLogIn(shop);
    }
}


void LoginWindow::on_cancelButton_clicked()
{
    this->close();
}


void LoginWindow::on_signButton_clicked()
{
    if(ui->nickname->text().isEmpty()) throw std::logic_error("Nickname cannot be empty");
    if(ui->password->text().isEmpty()) throw std::logic_error("Password cannot be empty");
    int type = ui->shopButton->isChecked();
    std::string nickname = ui->nickname->text().toStdString();
    std::string password = ui->password->text().toStdString();
    if(!type) { // Customer
        Customer customer(0,nickname, password);
        emit sigSignUp(customer);
    } else { // Shop
        Shop shop(0,nickname, password);
        emit sigSignUp(shop);
    }

}

