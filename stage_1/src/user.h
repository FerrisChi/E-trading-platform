#ifndef USER_H
#define USER_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <cstring>

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

class User : public QObject
{
    Q_OBJECT

 public:
  UserType type;
  char nickname[MAX_CHAR_LENGTH];
  int id;
  User(){};
  User(const int index);
  virtual ~User(){};

  virtual UserType GetUserType() = 0;

  double Consume(double amount);

  inline double Recharge(double amount) {
    this->balance += amount;
    return this->balance;
  }

  inline double GetBalance() { return this->balance; }

  User& operator=(const User& usr);

  void ModifyPassword(std::string newpassword);
  void UpdateUser(int index);

 private:
  int chart_num;
  int order_num;

  double balance;

  char password[MAX_CHAR_LENGTH];
};


class Customer: public User{
    Q_OBJECT

public:
    Customer(){};
    Customer(const int index);
    ~Customer(){};
    UserType GetUserType(){
        return UserType::Customer;
    }
};

class Shop: public User{
    Q_OBJECT

public:
    Shop(){};
    Shop(const int index);
    ~Shop(){};
    UserType GetUserType(){
        return UserType::Shop;
    }
};

#endif // USER_H
