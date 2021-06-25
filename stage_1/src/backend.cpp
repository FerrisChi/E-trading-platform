#include "backend.h"

static int Product_num;
static int User_num;

Backend::Backend(QApplication *a)
{
    std::ifstream ifs;
    ifs.open("data/general.data", std::ifstream::in | std::ifstream::binary);
    ifs.read((char *)&User_num, sizeof(int));
    ifs.read((char *)&Product_num, sizeof(int));
    ifs.close();
    qDebug()<<QString("Init System:")<<User_num<<Product_num;
    nowState = UserType::Unknown;

    connect(&this->mainWindow, &MainWindow::sigResetBackend, this, &Backend::ResetBackend);
    connect(&this->mainWindow, &MainWindow::sigLogout, this, &Backend::Logout);
    connect(&this->mainWindow.loginWindow, &LoginWindow::sigLogIn, this, &Backend::LoginUser);
    connect(&this->mainWindow.loginWindow, &LoginWindow::sigSignUp, this, &Backend::AddUser);
    connect(&this->mainWindow, &MainWindow::sigModifyPassword, this, &Backend::ModifyPassword);
    connect(&this->mainWindow, &MainWindow::sigRecharge, this, &Backend::Recharge);
    connect(&this->mainWindow, (void (MainWindow::*)(int, double))&MainWindow::sigSetDiscount, this, (void (Backend::*)(int, double))&Backend::SetDiscount);
    connect(&this->mainWindow, (void (MainWindow::*)(ProductType, double))&MainWindow::sigSetDiscount, this, (void (Backend::*)(ProductType, double))&Backend::SetDiscount);
    connect(&this->mainWindow, (void (MainWindow::*)(std::string))&MainWindow::sigSearchProduct, this, (void (Backend::*)(std::string))&Backend::SearhProduct);
    connect(&this->mainWindow, (void (MainWindow::*)())&MainWindow::sigSearchProduct, this, (void (Backend::*)())&Backend::SearhProduct);

    connect(&this->mainWindow.addProductWindow, &AddProductWindow::sigAddProduct, this, &Backend::AddProduct);
    connect(&this->mainWindow.modifyProductWindow, &ModifyProductWindow::sigModifyProduct, this, &Backend::ModifyProduct);


    ft.setFamily("MS Shell Dlg 2");
    a->setFont(ft);
    mainWindow.init();
}

void Backend::ResetBackend() {
    std::fstream fs;
    fs.open("data/general.data",std::fstream::in| std::fstream::out | std::fstream::trunc);
    int tmpi=0;
    fs.write((char *)&tmpi, sizeof(int));
    tmpi=0;
    fs.write((char *)&tmpi, sizeof(int));
    fs.close();
    fs.open("data/user.data", std::fstream::out | std::fstream::trunc);
    fs.close();
    fs.open("data/product.data", std::fstream::out | std::fstream::trunc);
    fs.close();
    this->mainWindow.close();
}

void Backend::AddUser(std::string nickname, std::string password, int type) {
  try {
    if (nickname.length() > 12)
        throw std::logic_error("Nickname too long.");
    if (password.length() > 12)
        throw std::logic_error("Password too long.");

    std::ifstream ifs;
    char nk[MAX_CHAR_LENGTH];
    int i=0;
    int id = User_num++, chart_num = 0, order_num = 0;
    double balance = 0;
    std::ofstream ofs;

    // check if exsited.
    ifs.open("data/user.data",std::ifstream::in | std::ifstream::binary);
    for(;i<User_num;i++){
        ifs.seekg(50*i+24, ifs.beg);
        ifs.read(nk, 13);
        if(!strcmp(nk, nickname.c_str())){
            ifs.close();
            throw std::logic_error("Nickname exist.");
        }
    }
    ifs.close();

    // change general config
    std::fstream fs;
    fs.open("data/general.data",std::fstream::in | std::fstream::out | std::fstream::binary);
    fs.write((char *)&User_num, sizeof(int));
    fs.close();

    ofs.open("data/user.data",
             std::ofstream::app | std::ofstream::out | std::ofstream::binary);
    ofs.write((char *)&id, sizeof(int));
    ofs.write((char *)&type, sizeof(int));
    ofs.write((char *)&chart_num, sizeof(int));
    ofs.write((char *)&order_num, sizeof(int));
    ofs.write((char *)&balance, sizeof(double));
    ofs.write(nickname.c_str(), 13);
    ofs.write(password.c_str(), 13);
    ofs.close();
    qDebug()<<"Sign Up: "<<id<<type<<chart_num<<order_num<<balance<<nickname.c_str()<<password.c_str();
    QMessageBox::information(NULL, "Sign up", "Sign up successfully.");
  }  catch (std::exception &e) {
    QMessageBox::critical(NULL, "Sign up Error", QString(e.what()));
  }
}

void Backend::LoginUser(std::string nickname, std::string password) {
  try {
    if (nickname.length() > 12)
        throw std::logic_error("Nickname too long.");
    if(password.length() > 12)
        throw std::logic_error("Password too long.");

    char nk[MAX_CHAR_LENGTH];
    char ps[MAX_CHAR_LENGTH];
    std::ifstream ifs;
    ifs.open("data/user.data", ifs.in | ifs.binary);
    for (int i = 0; i < User_num; i++) {
      ifs.seekg(24 + 50 * i, ifs.beg);
      ifs.read(nk, 13);
      if (strcmp(nickname.c_str(), nk))
        continue;
      else {
        ifs.read(ps, 13);
        if (strcmp(password.c_str(), ps))
            throw std::logic_error("Wrong password.");
        else{ // find!
            int type;
            ifs.seekg(50*i+4, ifs.beg);
            ifs.read((char*)&type, sizeof(int));
            ifs.close();
            if(!type) {
                customer = Customer(i);
                nowState = UserType::Customer;
                cusIndex = i;
                mainWindow.setLogin(&customer);
            }
            else {
                shop = Shop(i);
                nowState = UserType::Shop;
                shopIndex = i;
                mainWindow.setLogin(&shop);
            }
            std::string s = "Welcome, " + nickname;
            mainWindow.setNickname(QString::fromStdString(nickname));
            QMessageBox::information(NULL, "Login Successfully", QString::fromStdString(s));
            return;
        }
      }
    }
    ifs.close();
    throw std::logic_error("Nickname not exist.");
  } catch (std::exception &e) {
      QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void Backend::Logout() {
    nowState = UserType::Unknown;
    mainWindow.setLogout();
}

void Backend::ModifyPassword(std::string password) {
    try {
        if(nowState == UserType::Unknown) throw std::logic_error("Not login.");
        if(nowState == UserType::Customer) {
            customer.ModifyPassword(password);
            customer.UpdateUser(cusIndex);
        } else {
            shop.ModifyPassword(password);
            shop.UpdateUser(shopIndex);
        }
        QMessageBox::information(NULL, "Operation", "Modify password successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::ModifyProduct(int index, std::string name, std::string description, int stock, double price, double discount) {
    try {
        std::fstream fs("data/product.data",std::fstream::in|std::fstream::out|std::fstream::binary);
        fs.seekp(index * PRODUCT_BLOCK_SIZE + 8, fs.beg);
        fs.write((char *)&stock, sizeof(int));
        fs.seekp(index * PRODUCT_BLOCK_SIZE + 16, fs.beg);
        fs.write((char *)&price, sizeof(double));
        fs.write((char *)&discount, sizeof(double));
        fs.write(name.c_str(), MAX_NAME_SIZE + 1);
        fs.write(description.c_str(), MAX_DESC_SIZE+ 1);
        fs.close();
        QMessageBox::information(NULL, "Operation", "Modify product successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::Recharge(double amount) {
    try {
        if(nowState == UserType::Unknown) throw std::logic_error("Not login.");
        double nowAmount;
        if(nowState == UserType::Customer) {
            nowAmount = customer.Recharge(amount);
            customer.UpdateUser(cusIndex);
        } else {
            nowAmount = shop.Recharge(amount);
            shop.UpdateUser(shopIndex);
        }
        this->mainWindow.setRemaining(nowAmount);
        QMessageBox::information(NULL, "Operation", "Recharge successfully.");
    } catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::Consume(double amount) {
    try {
        if(nowState == UserType::Unknown) throw std::logic_error("Not login");
        double nowAmount;
        if(nowState == UserType::Customer) {
            nowAmount = customer.Recharge(amount);
            customer.UpdateUser(cusIndex);
        } else {
            nowAmount = shop.Recharge(amount);
            shop.UpdateUser(shopIndex);
        }
        this->mainWindow.setRemaining(nowAmount);
        QMessageBox::information(NULL, "Operation", "Consume successfully.");
    } catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::AddProduct(std::string name, std::string description, int type, double price, int stock, int feature) {
    try {
        if(name.length() > MAX_NAME_SIZE)
            throw std::logic_error("Name too long.");
        if(description.length() > MAX_DESC_SIZE)
            throw std::logic_error("Description too long.");
        std::fstream fs("data/general.data", std::fstream::in|std::fstream::out|std::fstream::binary);
        int id = Product_num++;
        double discount = 1;
        fs.seekp(4);
        fs.write((char *)&Product_num, sizeof(int));
        fs.close();
        std::ofstream ofs("data/product.data", std::ofstream::out|std::ofstream::binary|std::ofstream::app);
        ofs.write((char *)&id, sizeof(int));
        ofs.write((char *)&shop.id, sizeof(int));
        ofs.write((char *)&stock, sizeof(int));
        ofs.write((char *)&type, sizeof(int));
        ofs.write((char *)&price, sizeof(double));
        ofs.write((char *)&discount, sizeof(double));
        ofs.write(name.c_str(), MAX_NAME_SIZE + 1);
        ofs.write((char *)&feature, sizeof(int));
        ofs.write(description.c_str(), MAX_DESC_SIZE+ 1);
        ofs.close();
        QMessageBox::information(NULL, "Operation", "Added successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SetDiscount(int index, double discount) {
    try {
        std::fstream fs("data/product.data", std::fstream::in|std::fstream::out|std::fstream::binary);
        fs.seekp(PRODUCT_BLOCK_SIZE * index + 24, fs.beg);
        fs.write((char *)&discount, sizeof(double));
        fs.close();
        QMessageBox::information(NULL, "Operation", "Set discount successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SetDiscount(ProductType type, double discount) {
    try {
        std::fstream fs("data/product.data", std::fstream::in|std::fstream::out|std::fstream::binary);
        int nowtype, nowid;
        for(int i=0;i<Product_num;i++) {
            fs.seekg(PRODUCT_BLOCK_SIZE * i + 4, fs.beg);
            fs.read((char *)&nowid, sizeof(int));
            fs.seekg(PRODUCT_BLOCK_SIZE * i + 12, fs.beg);
            fs.read((char *)&nowtype, sizeof(int));
            if(nowtype == (int)type && nowid == shop.id){
                fs.seekp(PRODUCT_BLOCK_SIZE * i + 24, fs.beg);
                fs.write((char *)&discount, sizeof(double));
            }
        }
        fs.close();
        QMessageBox::information(NULL, "Operation", "Set discount successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SearhProduct(std::string description) {
    try {
        std::ifstream ifs("data/product.data", std::ifstream::in|std::ifstream::binary);
        char name[20], des[100];
        int cnt = 0;
        QVector<Product> productList;
        Product p;
        for(int i=0;i<Product_num;i++) {
            ifs.seekg(PRODUCT_BLOCK_SIZE * i + 32, ifs.beg);
            ifs.read(name, MAX_NAME_SIZE + 1);
            ifs.read(des, MAX_DESC_SIZE + 1);
            if(strstr(name, description.c_str())!=NULL || strstr(des, description.c_str())!=NULL) {
                cnt++;
                p = Product(i);
                p.print();
                productList.push_back(p);
                qDebug()<<"find product:"<<i<<name;
            }
        }
        qDebug()<<"find product number:"<<cnt;
        mainWindow.setSerchList(productList);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SearhProduct() {
    try {
        std::ifstream ifs("data/product.data", std::ifstream::in|std::ifstream::binary);
        int belong_id, cnt=0;
        QVector<Product> productList;
        Product p;
        for(int i=0;i<Product_num;i++) {
            ifs.seekg(PRODUCT_BLOCK_SIZE * i + 4, ifs.beg);
            ifs.read((char *)&belong_id, sizeof(int));
            if(belong_id == shop.id) {
                cnt++;
                p = Product(i);
                p.print();
                productList.push_back(p);
                qDebug()<<"find product:"<<i;
            }
        }
        qDebug()<<"find product number:"<<cnt;
        mainWindow.setManageList(productList);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}
