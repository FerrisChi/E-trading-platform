#ifndef USER_H
#define USER_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <cstring>

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

class User
{

 public:
  UserType type;
  char nickname[MAX_CHAR_LENGTH];
  char password[MAX_CHAR_LENGTH];
  int id;
  int chart_num;
  int order_num;
  double balance;
  User(){};
  User(const int index);
  User(int id, std::string nickname, std::string password);
  virtual ~User(){};

  virtual UserType GetUserType() = 0;

  double Consume(double amount);

  inline double Recharge(double amount) {
    this->balance += amount;
    return this->balance;
  }

  inline double GetBalance() { return this->balance; }

//  User& operator=(const User& usr);

  void ModifyPassword(std::string newpassword);
  void write(std::string path, int pos);
  void write();
  void print();
};


class Customer: public User{

public:
    Customer(){};
    Customer(const int index): User(index) {}
    Customer(int id, std::string nickname, std::string password);
    ~Customer(){};
    UserType GetUserType(){
        return UserType::Customer;
    }
};

class Shop: public User{

public:
    Shop(){};
    Shop(const int index): User(index) {}
    Shop(int id, std::string nickname, std::string password);
    ~Shop(){};
    UserType GetUserType(){
        return UserType::Shop;
    }
};

#endif // USER_H
