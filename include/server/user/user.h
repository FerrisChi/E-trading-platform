#ifndef USER_H
#define USER_H

#include <cstring>
#include <iostream>

#include "utils/execption.h"
#include "utils/json.hpp"
#include "utils/user_utils.h"
using json = nlohmann::json;

class User {
 public:
  int id;
  std::string nickname;
  UserType type;
  User(std::string nickname, std::string password, std::string type) {
    if (FindUserName(nickname))
      throw InvalidUserDataException("User name has been used.");
    this->nickname = nickname;
    this->password = password;
    if (type == "customer")
      this->type = UserType::Customer;
    else if (type == "shop")
      this->type = UserType::Shop;
    else
      throw InvalidUserDataException("User type can only be customer or shop.");
  }
  virtual ~User(){};

  virtual UserType GetUserType() = 0;

 private:
  std::string password;
  double balance;
  bool FindUserName(std::string nickname);
};

#endif