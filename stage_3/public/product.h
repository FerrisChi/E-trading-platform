#ifndef PRODUCT_H
#define PRODUCT_H

#include <QDebug>
#include <QObject>
#include <fstream>
#include <iostream>

#define PRODUCT_BLOCK_SIZE 100
#define MAX_NAME_SIZE 15
#define MAX_DESC_SIZE 47

enum ProductType { Book = 1, Food = 2, Cloth = 3 };
const std::string ProductTypeStr[4] = {"", "图书", "食品", "服饰"};
/*
data/product.data ( PRODUCTS DATA BLOCK )
                                 1  1  1  1  1  1
   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  (byte)
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |     id    | belong_id |   stock   |    type   |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |           price       |        discount       |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |                     name                      |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |  feature  |                 description
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
             |
 +--+--+--+--+
id: 4(int)
belong_id: 4(int)
stock: 4(int)
type: 4(int)
price: 8(double)
discount: 8(double)
name: 16(16*char)
feature: 4(int)
description: 48(48*char)
(total: 100 bytes)
*/

class Product {
 public:
  Product();
  Product(const int index);
  Product(const Product& product);
  Product(int id, ProductType type, int belong_id, int stock, int feature,
          double price, double discount, std::string name,
          std::string description);
  ~Product(){};

  Product& operator=(const Product& pdt);

  ProductType type;
  int id;
  int stock;
  int belong_id;
  int feature;
  double price;
  double discount;
  char name[16];
  char description[56];

  virtual double getPrice() const { return price; }
  // write to "path" file, {pos} position
  void write(std::string path, int pos) const;
  // write to "product.data" [this.id] position
  void write() const;
  // print info
  void print() const;

  // friend function
  // overload the input and output functions
  friend QDataStream& operator<<(QDataStream& out, Product& product);
  friend QDataStream& operator>>(QDataStream& in, Product& product);
};

class BookProduct : public Product {
 private:
  int edition;

 public:
  BookProduct() { feature = edition; }
  BookProduct(const int index);
  BookProduct(Product product);

  double getPrice() const { return this->price; }
  // get book edition
  inline int getFeature() const { return this->edition; }
};

class FoodProduct : public Product {
 private:
  int expire;

 public:
  FoodProduct() { feature = expire; }
  FoodProduct(const int index);
  FoodProduct(Product product);

  double getPrice() const { return this->price; }
  // get food expire date
  inline int getFeature() const { return this->expire; }
};

class ClothProduct : public Product {
 private:
  int size;

 public:
  ClothProduct() { feature = size; }
  ClothProduct(const int index);
  ClothProduct(Product product);

  double getPrice() const { return this->price; }
  // get cloth size
  inline int getFeature() const { return this->size; }
};
#endif  // PRODUCT_H
