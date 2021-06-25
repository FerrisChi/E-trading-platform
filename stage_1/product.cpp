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
    int tmp;
    ifs.read((char *)&tmp, sizeof(int));
    ifs.read(this->description, MAX_DESC_SIZE + 1);
}

Product::Product(const Product& pdt) {
    this->id = pdt.id;
    this->belong_id = pdt.belong_id;
    this->stock = pdt.stock;
    this->type = pdt.type;
    this->price = pdt.price;
    this->discount = pdt.discount;
    strcpy(this->name, pdt.name);
    strcpy(this->description, pdt.description);
}

Product& Product:: operator=(const Product& pdt){
    this->id = pdt.id;
    this->type = pdt.type;
    this->price = pdt.price;
    this->discount = pdt.discount;
    this->belong_id = pdt.belong_id;
    this->stock = pdt.stock;
    strcpy(this->name, pdt.name);
    strcpy(this->description, pdt.description);
    return *this;
}

void Product::print() {
    qDebug()<<this->id<<this->belong_id<<this->stock<<(int)this->type<<this->price<<this->discount<<this->name<<this->description;
}

//void Product::UpdateProduct(int index) {
//    std::fstream fs("data/product.data",std::fstream::in | std::fstream::out | std::fstream::binary);
//    fs.seekp(PRODUCT_BLOCK_SIZE *index, fs.beg);
//    fs.write((char *)&this->id, sizeof(int));
//    fs.write((char *)&this->belong_id, sizeof(int));
//    fs.write((char *)&this->stock, sizeof(int));
//    int type = (int)this->type;
//    fs.write((char *)&type, sizeof(int));
//    fs.write((char *)&this->price, sizeof(double));
//    fs.write((char *)&this->discount, sizeof(double));
//    fs.write(this->name, 16);
//    fs.write(this->description, 16);
//    fs.close();
//    qDebug()<<this->id<<(int)(this->belong_id)<<this->stock<<type<<this->price<<this->discount<<this->name<<this->description;
//}

BookProduct::BookProduct(const int index) : Product(index) {
    std::ifstream ifs("data/product.data",std::ifstream::in|std::ifstream::binary);
    ifs.seekg(PRODUCT_BLOCK_SIZE  * index + 48, ifs.beg);
    ifs.read((char*)&this->edition, sizeof(int));
    ifs.close();
}

FoodProduct::FoodProduct(const int index) : Product(index) {
    std::ifstream ifs("data/product.data",std::ifstream::in|std::ifstream::binary);
    ifs.seekg(PRODUCT_BLOCK_SIZE  * index + 48, ifs.beg);
    ifs.read((char*)&this->expire, sizeof(int));
    ifs.close();
}

ClothProduct::ClothProduct(const int index) : Product(index) {
    std::ifstream ifs("data/product.data",std::ifstream::in|std::ifstream::binary);
    ifs.seekg(PRODUCT_BLOCK_SIZE  * index + 48, ifs.beg);
    ifs.read((char*)&this->size, sizeof(int));
    ifs.close();
}
