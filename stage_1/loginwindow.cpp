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
    this->close();
    emit sigLogIn(ui->nickname->text().toStdString(), ui->password->text().toStdString());
}


void LoginWindow::on_cancelButton_clicked()
{
    this->close();
}


void LoginWindow::on_signButton_clicked()
{
    int type = ui->shopButton->isChecked();
    emit sigSignUp(ui->nickname->text().toStdString(), ui->password->text().toStdString(), type);
}

