#ifndef SHOPER_H
#define SHOPER_H

#include "server/user/user.h"

class Shoper : public User {
 private:
 public:
  Shoper(std::string nickname, std::string password, std::string type)
      : User(nickname, password, type){};
  ~Shoper() {}
  UserType GetUserType() { return this->type; }
};

#endif