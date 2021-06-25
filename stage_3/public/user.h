#ifndef USER_H
#define USER_H

#include <QDataStream>
#include <QDebug>
#include <QObject>
#include <cstring>
#include <fstream>
#include <iostream>

#define USER_BLOCK_SIZE 50
#define MAX_STRING_LENGTH 12
#define MAX_CHAR_LENGTH (MAX_STRING_LENGTH + 1)
enum class UserType { Customer = 0, Shop = 1, Unknown = -1 };
/*
data/user.data ( USER DATA BLOCK )
                                 1  1  1  1  1  1
   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  (byte)
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |     id    |    type   | chart_num | order_num |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |       balance         |       nickname        |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |    nickname  |            password            |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |     |
 +--+--+
id: 4(int)
type: 4(int)
chart_num: 4(int)
order_num: 4(int)
balance: 8(double)
nickname: 13(13*char: 12 + '\0')
password: 13(13*char: 12 + '\0')
(total 50 bytes)
*/

class User {
 public:
  UserType type;
  char nickname[MAX_CHAR_LENGTH];
  int id;

  User(){};
  User(const int index);
  User(int id, std::string nickname, std::string password);
  virtual ~User(){};

  virtual UserType GetUserType() const = 0;

  // consum {amount} money
  double Consume(double amount);

  // recharge {amount} money
  inline double Recharge(double amount) {
    this->balance += amount;
    return this->balance;
  }

  // get balance
  inline double GetBalance() const { return this->balance; }

  // modify password
  void ModifyPassword(std::string newpassword);
  // write to "path" file, {pos} position
  void write(std::string path, int pos) const;
  // write to "user.data" file, [this.id] position
  void write() const;
  // print info
  void print() const;

 protected:
  double balance;
  char password[MAX_CHAR_LENGTH];
  int chart_num;
  int order_num;

  // friend function
  // overload the input and output functions
  friend QDataStream& operator<<(QDataStream& out, User& user);
  friend QDataStream& operator>>(QDataStream& in, User& user);
};

class Customer : public User {
 public:
  Customer(){};
  Customer(const int index) : User(index) {}
  Customer(int id, std::string nickname, std::string password);
  ~Customer(){};
  UserType GetUserType() const { return UserType::Customer; }

  // check add one product to chart
  bool AddToChart(int maxSize);
  // remoce one product from chart
  bool RemoveFromChart();

  // check if two customer's password matches
  inline bool MatchPassword(Customer customer) const {
    return !strcmp(customer.password, password);
  }

  // friend function
  // overload the input and output functions
  friend QDataStream& operator<<(QDataStream& out,
                                 Customer& customer);  // Reload <<
  friend QDataStream& operator>>(QDataStream& in, Customer& customer);
};

class Shop : public User {
 public:
  Shop(){};
  Shop(const int index) : User(index) {}
  Shop(int id, std::string nickname, std::string password);
  ~Shop(){};
  UserType GetUserType() const { return UserType::Shop; }

  // check if two shop's password matches
  inline bool MatchPassword(Shop shop) const {
    return !strcmp(shop.password, password);
  }

  // friend function
  // overload the input and output functions
  friend QDataStream& operator<<(QDataStream& out, Shop& shop);  // Reload <<
  friend QDataStream& operator>>(QDataStream& in, Shop& shop);
};

#endif  // USER_H
