#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <QDebug>


#define PRODUCT_BLOCK_SIZE 100
#define MAX_NAME_SIZE 15
#define MAX_DESC_SIZE 47

enum class ProductType { Book = 1, Food = 2, Cloth = 3};
const std::string ProductTypeStr[4] = {"","图书","食品","服饰"};
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

class Product
{
 public:
  Product(){
      stock = 0;
  };

  // read from disk
  Product(const int index);
  Product(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description);
  ~Product(){};

  ProductType type;
  int id;
  int stock;
  int belong_id;
  int feature;
  double price;
  double discount;
  char name[16];
  char description[56];

  virtual double getPrice(){
      return price;
  }

  // write to "path" file
  void write(std::string path, int pos);
  void write();
  void print();
};

class BookProduct : public Product
{
private:
    int edition;

public:
    BookProduct(){
        feature = edition;
    }
    BookProduct(const int index);
    BookProduct(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description);

    double getPrice() {
        return this->price;
    }
    inline int getFeature(){ return this->edition;}
};

class FoodProduct : public Product
{
private:
    int expire;
public:
    FoodProduct(){
        feature = expire;
    }
    FoodProduct(const int index);
    FoodProduct(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description);

    double getPrice() {
        return this->price;
    }
    inline int getFeature(){ return this->expire;}
};

class ClothProduct : public Product
{
private:
    int size;
public:
    ClothProduct(){
        feature = size;
    }
    ClothProduct(const int index);
    ClothProduct(int id, int belongID, int stock, double price, double discount, std::string name, int feature, std::string description);

    double getPrice() {
        return this->price;
    }
    inline int getFeature(){ return this->size;}
};
#endif // PRODUCT_H
