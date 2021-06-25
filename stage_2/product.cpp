#include "product.h"

Product::Product(const int index) {
    std::ifstream ifs("data/product.data",std::ifstream::in|std::ifstream::binary);
    ifs.seekg(PRODUCT_BLOCK_SIZE  * index, ifs.beg);
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

Product::Product(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description) {
    this->id = id;
    this->belong_id = belongID;
    this->price = price;
    this->discount = discount;
    this->stock = stock;
    strncpy(this->name, name.c_str(), MAX_NAME_SIZE + 1);
    this->feature = feature;
    strncpy(this->description, description.c_str(), MAX_DESC_SIZE + 1);
}

void Product::print() {
    qDebug()<<"Product: "<<id<<belong_id<<stock<<(int)type<<price<<discount<<name<<feature<<description;
}

void Product::write(std::string path, int pos){
    std::fstream fs(path, std::fstream::in | std::fstream::out | std::fstream::binary);
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
}

void Product::write() {
    this->write("data/product.data", PRODUCT_BLOCK_SIZE * this->id);
}

BookProduct::BookProduct(const int index) : Product(index) {
    this->type = ProductType::Book;
    this->edition = feature;
}

BookProduct::BookProduct(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description) : Product(id, belongID, stock, price, discount, name, feature, description) {
    this->type = ProductType::Book;
    this->edition = feature;
}

FoodProduct::FoodProduct(const int index) : Product(index) {
    this->type = ProductType::Food;
    this->expire = feature;
}

FoodProduct::FoodProduct(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description) : Product(id, belongID, stock, price, discount, name, feature, description) {
    this->type = ProductType::Food;
    this->expire = feature;
}

ClothProduct::ClothProduct(const int index) : Product(index) {
    this->type = ProductType::Cloth;
    this->size = feature;
}

ClothProduct::ClothProduct(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description) : Product(id, belongID, stock, price, discount, name, feature, description) {
    this->type = ProductType::Cloth;
    this->size = feature;
}
