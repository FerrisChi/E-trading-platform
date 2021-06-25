#ifndef ADDPRODUCTWINDOW_H
#define ADDPRODUCTWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QValidator>
#include "product.h"

namespace Ui {
class AddProductWindow;
}

class AddProductWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddProductWindow(QWidget *parent = nullptr);
    ~AddProductWindow();

public slots:
    void init(int shopID);

signals:
    void sigAddProduct(int belongID, int stock, int type, double price, std::string name, int feature, std::string description);

private slots:
    void on_clearButton_clicked();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddProductWindow *ui;
};

#endif // ADDPRODUCTWINDOW_H
