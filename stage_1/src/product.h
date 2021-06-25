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

class Product : public QObject
{
    Q_OBJECT
 private:
  int id;

 public:
  Product(){};
  Product(const int index);
  Product(const Product &pdt);
  ~Product(){};

  Product& operator=(const Product& pdt);

  ProductType type;
  int stock;
  int belong_id;
  double price;
  double discount;
  char name[16];
  char description[56];

  inline int getID(){return id;}

  virtual double getPrice(){
      return price;
  }

//  void UpdateProduct(int index);
  void print();
};

class BookProduct : public Product
{
    Q_OBJECT

private:
    int edition;

public:
    BookProduct(){};
    BookProduct(const int index);

    double getPrice() {
        return this->price;
    }
    inline int getFeature(){ return this->edition;}
};

class FoodProduct : public Product
{
    Q_OBJECT
private:
    int expire;
public:
    FoodProduct(){};
    FoodProduct(const int index);

    double getPrice() {
        return this->price;
    }
    inline int getFeature(){ return this->expire;}
};

class ClothProduct : public Product
{
    Q_OBJECT
private:
    int size;
public:
    ClothProduct(){};
    ClothProduct(const int index);

    double getPrice() {
        return this->price;
    }
    inline int getFeature(){ return this->size;}
};
#endif // PRODUCT_H
