#include "modifyproductwindow.h"

#include "ui_modifyproductwindow.h"

ModifyProductWindow::ModifyProductWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ModifyProductWindow) {
  ui->setupUi(this);
  ui->stock->setValidator(new QIntValidator(0, 999, this));
  ui->price->setValidator(new QDoubleValidator(0, 100000, 10, this));
  ui->discount->setValidator(new QDoubleValidator(0, 1, 10, this));
  ui->feature->setValidator(new QIntValidator(0, 100, this));
}

ModifyProductWindow::~ModifyProductWindow() { delete ui; }

// -------------------------
// SLOT FUNCTION

void ModifyProductWindow::init(Product pdt) {
  nowPdt = pdt;
  ui->name->setText(pdt.name);
  ui->description->setText(pdt.description);
  ui->stock->setText(QString::number(pdt.stock));
  ui->price->setText(QString::number(pdt.price, 'f', 2));
  ui->discount->setText(QString::number(pdt.discount, 'f', 2));
  if (pdt.type == ProductType::Book) {
    ui->featureLabel->setText("版次");
  } else if (pdt.type == ProductType::Food) {
    ui->featureLabel->setText("保质期");
  } else {
    ui->featureLabel->setText("尺码");
  }
  ui->feature->setText(QString::asprintf("%d", pdt.feature));
  this->show();
}

void ModifyProductWindow::on_cancelButton_clicked() { this->close(); }

void ModifyProductWindow::on_clearButton_clicked() {
  ui->name->clear();
  ui->description->clear();
  ui->stock->clear();
  ui->discount->clear();
  ui->price->clear();
}

void ModifyProductWindow::on_okButton_clicked() {
  try {
    if (ui->name->text().isEmpty())
      throw std::logic_error("Name cannot be null");
    if (ui->stock->text().isEmpty())
      throw std::logic_error("Stock cannot be null");
    if (ui->price->text().isEmpty())
      throw std::logic_error("Price cannot be null");
    if (ui->discount->text().isEmpty())
      throw std::logic_error("Discount cannot be null");
    std::string name = ui->name->text().toStdString();
    std::string description = ui->description->text().toStdString();
    int stock = ui->stock->text().toInt();
    int feature;
    double price = ui->price->text().toDouble();
    double discount = ui->discount->text().toDouble();
    if (name.length() > MAX_NAME_SIZE) throw std::logic_error("Name too long");
    if (description.length() > MAX_DESC_SIZE)
      throw std::logic_error("Description too long");
    if (nowPdt.type == ProductType::Book) {
      if (ui->feature->text().isEmpty())
        throw std::logic_error("Edition cannot be null");
    } else if (nowPdt.type == ProductType::Food) {
      if (ui->feature->text().isEmpty())
        throw std::logic_error("Expire date cannot be null");
    } else {
      if (ui->feature->text().isEmpty())
        throw std::logic_error("CLoth size cannot be null");
    }
    feature = ui->feature->text().toInt();
    if (feature < 1 || feature > 999)
      throw std::logic_error("Feature between 1 and 999");
    Product product(nowPdt.id, nowPdt.type, nowPdt.belong_id, stock, feature,
                    price, discount, name, description);
    emit sigModifyProduct(product);
    this->close();
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}
