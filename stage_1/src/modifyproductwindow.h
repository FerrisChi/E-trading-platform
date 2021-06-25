#ifndef MODIFYPRODUCTWINDOW_H
#define MODIFYPRODUCTWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include <QMessageBox>
#include "product.h"

namespace Ui {
class ModifyProductWindow;
}

class ModifyProductWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModifyProductWindow(QWidget *parent = nullptr);
    ~ModifyProductWindow();

signals:
    void sigModifyProduct(int index, std::string name, std::string description, int stock, double price, double discount);


public slots:
    void init(Product pdt);

private slots:
    void on_cancelButton_clicked();

    void on_clearButton_clicked();

    void on_okButton_clicked();

private:
    Ui::ModifyProductWindow *ui;
    Product nowPdt;
};

#endif // MODIFYPRODUCTWINDOW_H
