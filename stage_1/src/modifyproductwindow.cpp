#include "modifyproductwindow.h"
#include "ui_modifyproductwindow.h"

ModifyProductWindow::ModifyProductWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModifyProductWindow)
{
    ui->setupUi(this);
    ui->stock->setValidator(new QIntValidator(0, 999, this));
    ui->price->setValidator(new QDoubleValidator(0, 100000, 10, this));
    ui->discount->setValidator(new QDoubleValidator(0, 1, 10, this));
}

ModifyProductWindow::~ModifyProductWindow()
{
    delete ui;
}

// -------------------------
// SLOT FUNCTION

void ModifyProductWindow::init(Product pdt) {
    nowPdt = pdt;
    ui->name->setText(pdt.name);
    ui->description->setText(pdt.description);
    ui->stock->setText(QString::number(pdt.stock));
    ui->price->setText(QString::number(pdt.price, 'f', 2));
    ui->discount->setText(QString::number(pdt.discount, 'f', 2));
    this->show();
}

void ModifyProductWindow::on_cancelButton_clicked()
{
    this->close();
}


void ModifyProductWindow::on_clearButton_clicked()
{
    ui->name->clear();
    ui->description->clear();
    ui->stock->clear();
    ui->discount->clear();
    ui->price->clear();
}

void ModifyProductWindow::on_okButton_clicked()
{
    try {
        std::string name = ui->name->text().toStdString();
        std::string description = ui->description->text().toStdString();
        int stock = ui->stock->text().toInt();
        double price = ui->price->text().toDouble();
        double discount = ui->discount->text().toDouble();
        if(name.length()>MAX_NAME_SIZE) throw std::logic_error("Name too long");
        if(description.length()>MAX_DESC_SIZE) throw std::logic_error("Description too long");
        emit sigModifyProduct(nowPdt.getID(),name, description, stock, price, discount);
        this->close();
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }



}

