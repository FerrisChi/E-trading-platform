#include "product.h"

Product::Product() {
  id = 0;
  stock = 0;
  belong_id = 0;
  feature = 0;
  price = 0;
  discount = 0;
};

Product::Product(int id, ProductType type, int belong_id, int stock,
                 int feature, double price, double discount, std::string name,
                 std::string description) {
  this->id = id;
  this->belong_id = belong_id;
  this->price = price;
  this->type = type;
  this->discount = discount;
  this->stock = stock;
  strncpy(this->name, name.c_str(), MAX_NAME_SIZE + 1);
  this->feature = feature;
  strncpy(this->description, description.c_str(), MAX_DESC_SIZE + 1);
}

// read from disk
Product::Product(const int index) {
  std::ifstream ifs("../data/product.data",
                    std::ifstream::in | std::ifstream::binary);
  ifs.seekg(PRODUCT_BLOCK_SIZE * index, ifs.beg);
  ifs.read((char *)&this->id, sizeof(int));
  ifs.read((char *)&this->belong_id, sizeof(int));
  ifs.read((char *)&this->stock, sizeof(int));
  ifs.read((char *)&this->type, sizeof(int));
  ifs.read((char *)&this->price, sizeof(double));
  ifs.read((char *)&this->discount, sizeof(double));
  ifs.read(this->name, MAX_NAME_SIZE + 1);
  ifs.read((char *)&this->feature, sizeof(int));
  ifs.read(this->description, MAX_DESC_SIZE + 1);
  ifs.close();
}

Product::Product(const Product &product) {
  this->id = product.id;
  this->belong_id = product.belong_id;
  this->price = product.price;
  this->type = product.type;
  this->discount = product.discount;
  this->stock = product.stock;
  strncpy(this->name, product.name, MAX_NAME_SIZE + 1);
  this->feature = product.feature;
  strncpy(this->description, product.description, MAX_DESC_SIZE + 1);
}

Product &Product::operator=(const Product &product) {
  this->id = product.id;
  this->belong_id = product.belong_id;
  this->price = product.price;
  this->type = product.type;
  this->discount = product.discount;
  this->stock = product.stock;
  strncpy(this->name, product.name, MAX_NAME_SIZE + 1);
  this->feature = product.feature;
  strncpy(this->description, product.description, MAX_DESC_SIZE + 1);
  return *this;
}

void Product::print() const {
  qDebug() << "Product: " << id << belong_id << stock << (int)type << price
           << discount << name << feature << description;
}

// write to ${path} file
void Product::write(std::string path, int pos) const {
  std::fstream fs(path,
                  std::fstream::in | std::fstream::out | std::fstream::binary);
  fs.seekp(pos, fs.beg);
  fs.write((char *)&this->id, sizeof(int));
  fs.write((char *)&this->belong_id, sizeof(int));
  fs.write((char *)&this->stock, sizeof(int));
  int type = (int)this->type;
  fs.write((char *)&type, sizeof(int));
  fs.write((char *)&this->price, sizeof(double));
  fs.write((char *)&this->discount, sizeof(double));
  fs.write(this->name, 16);
  fs.write((char *)&this->feature, sizeof(int));
  fs.write(this->description, 48);
  fs.close();
  //    this->print();
}

void Product::write() const {
  this->write("../data/product.data", PRODUCT_BLOCK_SIZE * this->id);
}

BookProduct::BookProduct(const int index) : Product(index) {
  this->type = ProductType::Book;
  this->edition = feature;
}

BookProduct::BookProduct(Product product) : Product(product) {
  this->type = ProductType::Book;
  this->edition = feature;
}

FoodProduct::FoodProduct(const int index) : Product(index) {
  this->type = ProductType::Food;
  this->expire = feature;
}

FoodProduct::FoodProduct(Product product) : Product(product) {
  this->type = ProductType::Food;
  this->expire = feature;
}

ClothProduct::ClothProduct(const int index) : Product(index) {
  this->type = ProductType::Cloth;
  this->size = feature;
}

ClothProduct::ClothProduct(Product product) : Product(product) {
  this->type = ProductType::Cloth;
  this->size = feature;
}

/*-----------------------------------------|
|  overload the input and output functions |
|-----------------------------------------*/
QDataStream &operator<<(QDataStream &out, Product &product) {
  out << product.id << product.belong_id << product.stock << (int)product.type
      << product.price << product.discount << QString(product.name)
      << product.feature << QString(product.description);
  return out;
}

QDataStream &operator>>(QDataStream &in, Product &product) {
  QString name, description;
  int type;
  in >> product.id >> product.belong_id >> product.stock >> type >>
      product.price >> product.discount >> name >> product.feature >>
      description;
  strncpy(product.name, name.toStdString().c_str(), MAX_NAME_SIZE + 1);
  strncpy(product.description, description.toStdString().c_str(),
          MAX_DESC_SIZE + 1);
  if (type == 1)
    product.type = ProductType::Book;
  else if (type == 2)
    product.type = ProductType::Food;
  else
    product.type = ProductType::Cloth;
  return in;
}
