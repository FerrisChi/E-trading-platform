#include "addproductwindow.h"
#include "ui_addproductwindow.h"

AddProductWindow::AddProductWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddProductWindow)
{
    ui->setupUi(this);
    ui->stock->setValidator(new QIntValidator(1, 1000, this));
    ui->price->setValidator(new QDoubleValidator(0, 1000000, 10, this));
    ui->bookFeature->setValidator(new QIntValidator(1,10000, this));
    ui->clothFeature->setValidator(new QIntValidator(1,10000, this));
    ui->foodFeature->setValidator(new QIntValidator(1,10000, this));
}

AddProductWindow::~AddProductWindow()
{
    delete ui;
}

void AddProductWindow::init(int shopID) {
    ui->shopID->setText(QString::asprintf("%d", shopID));
    this->show();
}

void AddProductWindow::on_clearButton_clicked()
{
    ui->name->setText("");
    ui->description->setText("");
    ui->stock->setText("");
    ui->price->setText("");
    ui->bookFeature->setText("");
    ui->clothFeature->setText("");
    ui->foodFeature->setText("");
}


void AddProductWindow::on_okButton_clicked()
{
    try {
        if(ui->name->text().isEmpty()) throw std::logic_error("Name cannot be null");
        if(ui->stock->text().isEmpty()) throw std::logic_error("Stock cannot be null");
        if(ui->price->text().isEmpty()) throw std::logic_error("Price cannot be null");
        int belongID = ui->shopID->text().toInt();
        std::string name = ui->name->text().toStdString();
        std::string description = ui->description->text().toStdString();
        if(name.length() > MAX_NAME_SIZE) throw std::logic_error("Name too long");
        if(description.length() > MAX_DESC_SIZE) throw std::logic_error("Description too long");
        int type, feature;
        if(ui->bookButton->isChecked()){
            type = 1;
            if(ui->bookFeature->text().isEmpty()) throw std::logic_error("Edition cannot be null");
            feature = ui->bookFeature->text().toInt();
        }
        else if(ui->foodButton->isChecked()){
            type = 2;
            if(ui->foodFeature->text().isEmpty()) throw std::logic_error("Expire data cannot be null");
            feature = ui->foodFeature->text().toInt();
        }
        else{
            type = 3;
            if(ui->clothFeature->text().isEmpty()) throw std::logic_error("Cloth size cannot be null");
            feature = ui->clothFeature->text().toInt();
        }
        if(feature < 1 || feature > 999) throw  std::logic_error("Feature between 1 and 999");
        double price = ui->price->text().toDouble();
        int stock = ui->stock->text().toInt();
        emit sigAddProduct(belongID, stock, type, price, name, feature, description);
        this->close();
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void AddProductWindow::on_cancelButton_clicked()
{
    this->close();
}
