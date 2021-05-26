#include <fstream>
#include <iostream>

#include "server/user/buyer.h"
#include "server/user/shoper.h"
#include "server/user/user.h"
#include "utils/json.hpp"
#include "utils/user_utils.h"
using json = nlohmann::json;

int main(int argv, char **argc) {
  std::cout << "Hello world." << std::endl;
  // std::string nickname, password, type;
  // std::cin >> nickname >> password >> type;

  // User *userA = new Buyer(nickname, password, type);
  // std::cout << userA->nickname;
  // delete userA;

  
  return 0;
}