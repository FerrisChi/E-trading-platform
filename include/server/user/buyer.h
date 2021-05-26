#ifndef BUYER_H
#define BUYER_H

#include "server/user/user.h"

class Buyer : public User {
 private:
 public:
  Buyer(std::string nickname, std::string password, std::string type)
      : User(nickname, password, type){};
  ~Buyer() {}
  UserType GetUserType() { return this->type; }
};

#endif