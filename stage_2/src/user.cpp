#include "user.h"

// read from "data/user.data" [index]
User::User(const int index) {
  std::ifstream ifs("data/user.data",std::ifstream::in|std::ifstream::binary);
  ifs.seekg(50 * index, ifs.beg);
  ifs.read((char *)&this->id, sizeof(int));
  ifs.read((char *)&this->type, sizeof(int));
  ifs.read((char *)&this->chart_num, sizeof(int));
  ifs.read((char *)&this->order_num, sizeof(int));
  ifs.read((char *)&this->balance, sizeof(double));
  ifs.read(this->nickname, MAX_CHAR_LENGTH);
  ifs.read(this->password, MAX_CHAR_LENGTH);
  ifs.close();
}

User::User(int id, std::string nickname, std::string password) {
    this->id = id;
    this->chart_num = 0;
    this->order_num = 0;
    this->balance = 0;
    strncpy(this->nickname, nickname.c_str(), MAX_CHAR_LENGTH);
    strncpy(this->password, password.c_str(), MAX_CHAR_LENGTH);
}

// write into file from pos
void User::write(std::string path, int pos) {
    std::fstream fs(path, std::fstream::in | std::fstream::out | std::fstream::binary);
    fs.seekp(pos, fs.beg);
    fs.write((char *)&this->id, sizeof(int));
    int type = this->type == UserType::Customer ? 0 : 1;
    fs.write((char *)&type, sizeof(int));
    fs.write((char *)&this->chart_num, sizeof(int));
    fs.write((char *)&this->order_num, sizeof(int));
    fs.write((char *)&this->balance, sizeof(double));
    fs.write(this->nickname, 13);
    fs.write(this->password, 13);
    fs.close();
}

// defalut write to "data/user.data"
void User::write() {
    this->write("data/user.data", this->id * USER_BLOCK_SIZE);
}

void User::print() {
    qDebug()<<"User Information: "<<id<<(int)type<<chart_num<<order_num<<balance<<nickname<<password;
}

double User::Consume(double amount) {
    this->balance -= amount;
    return this->balance;
}

void User::ModifyPassword(std::string newpassword) {
    strncpy(this->password, newpassword.c_str(), MAX_CHAR_LENGTH);
}

Customer::Customer(int id, std::string nickname, std::string password) : User(id, nickname, password){
    this->type = UserType::Customer;
}

Shop::Shop(int id, std::string nickname, std::string password) : User(id, nickname, password){
    this->type = UserType::Shop;
}
