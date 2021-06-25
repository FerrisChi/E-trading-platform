#ifndef ADDPRODUCTWINDOW_H
#define ADDPRODUCTWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QValidator>

#include "../public/product.h"

namespace Ui {
class AddProductWindow;
}

class AddProductWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit AddProductWindow(QWidget *parent = nullptr);
  ~AddProductWindow();

 public slots:
  void init(int shopID);

 signals:
  void sigAddProduct(Product p);

 private slots:
  void on_clearButton_clicked();

  void on_okButton_clicked();

  void on_cancelButton_clicked();

 private:
  Ui::AddProductWindow *ui;
};

#endif  // ADDPRODUCTWINDOW_H
