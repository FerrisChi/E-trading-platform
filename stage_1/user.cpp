#include "user.h"


void User::UpdateUser(int index) {
    std::fstream fs("data/user.data",std::fstream::in | std::fstream::out | std::fstream::binary);
    fs.seekp(50*index, fs.beg);
    fs.write((char *)&this->id, sizeof(int));
    int type = this->type == UserType::Customer ? 0 : 1;
    fs.write((char *)&type, sizeof(int));
    fs.write((char *)&this->chart_num, sizeof(int));
    fs.write((char *)&this->order_num, sizeof(int));
    fs.write((char *)&this->balance, sizeof(double));
    fs.write(this->nickname, 13);
    fs.write(this->password, 13);
    fs.close();
    qDebug()<<this->id<<(int)(this->type)<<this->chart_num<<this->order_num<<this->balance<<this->nickname<<this->password;
}

// assign the User with buffer
User::User(const int index) {
  std::ifstream ifs("data/user.data",std::ifstream::in|std::ifstream::binary);
  ifs.seekg(50*index, ifs.beg);
  ifs.read((char *)&this->id, sizeof(int));
  ifs.read((char *)&this->type, sizeof(int));
  ifs.read((char *)&this->chart_num, sizeof(int));
  ifs.read((char *)&this->order_num, sizeof(int));
  ifs.read((char *)&this->balance, sizeof(double));
  ifs.read(this->nickname, MAX_CHAR_LENGTH);
  ifs.read(this->password, MAX_CHAR_LENGTH);
  ifs.close();
  qDebug()<<this->id<<(int)(this->type)<<this->chart_num<<this->order_num<<this->balance<<this->nickname<<this->password;
}

double User::Consume(double amount) {
    if(this->balance < amount) return -1;
    this->balance -= amount;
    return this->balance;
}

void User::ModifyPassword(std::string newpassword) {
    strcpy(this->password, newpassword.c_str());
}

User& User:: operator=(const User& usr){
    this->id = usr.id;
    this->type = usr.type;
    this->chart_num = usr.chart_num;
    this->order_num = usr.order_num;
    this->balance = usr.balance;
    strcpy(this->nickname, usr.nickname);
    strcpy(this->password, usr.password);
    return *this;
}

Customer::Customer(const int index): User(index) {
    this->type = UserType::Customer;
}

Shop::Shop(const int index): User(index) {
    this->type = UserType::Shop;
}
