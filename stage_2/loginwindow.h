#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include "product.h"
#include"user.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
signals:
    void sigLogIn(Customer customer);
    void sigLogIn(Shop shop);
    void sigSignUp(Customer customer);
    void sigSignUp(Shop shop);

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_signButton_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
