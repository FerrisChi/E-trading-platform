#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

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
    void sigLogIn(std::string nickname, std::string password);
    void sigSignUp(std::string nickname, std::string password, int type);

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_signButton_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
