#include "backend.h"

static int Product_num;
static int User_num;

//------------------------------------------------------------
// PUBLIC FUNCTION
void Backend::SetGeneralConfig() {
    std::fstream fs;
    fs.open("data/general.data",std::fstream::in | std::fstream::out | std::fstream::binary);
    fs.write((char *)&User_num, sizeof(int));
    fs.write((char *)&Product_num, sizeof(int));
    fs.close();
}

void Backend::GetGeneralConfig() {
    std::ifstream ifs;
    ifs.open("data/general.data", std::ifstream::in | std::ifstream::binary);
    ifs.read((char *)&User_num, sizeof(int));
    ifs.read((char *)&Product_num, sizeof(int));
    ifs.close();
}


//--------------------------------------------------------
// SLOT FUNCTION
Backend::Backend(QApplication *a)
{
    GetGeneralConfig();
    qDebug()<<QString("Init System:")<<User_num<<Product_num;

    connect(&this->mainWindow, &MainWindow::sigResetBackend, this, &Backend::ResetBackend);
    connect(&this->mainWindow, &MainWindow::sigModifyPassword, this, &Backend::ModifyPassword);
    connect(&this->mainWindow, &MainWindow::sigRecharge, this, &Backend::Recharge);
    connect(&this->mainWindow, (void (MainWindow::*)(int, double))&MainWindow::sigSetDiscount, this, (void (Backend::*)(int, double))&Backend::SetDiscount);
    connect(&this->mainWindow, (void (MainWindow::*)(int, ProductType, double))&MainWindow::sigSetDiscount, this, (void (Backend::*)(int, ProductType, double))&Backend::SetDiscount);
    connect(&this->mainWindow, (void (MainWindow::*)(std::string))&MainWindow::sigSearchProduct, this, (void (Backend::*)(std::string))&Backend::SearhProduct);
    connect(&this->mainWindow, (void (MainWindow::*)(int))&MainWindow::sigSearchProduct, this, (void (Backend::*)(int shopID))&Backend::SearhProduct);
    connect(&this->mainWindow, &MainWindow::sigAddToChart, this, &Backend::AddToChart);
    connect(&this->mainWindow, &MainWindow::sigGetChart, this, &Backend::GetChart);
    connect(&this->mainWindow, &MainWindow::sigModifyChart, this, &Backend::ModifyChart);
    connect(&this->mainWindow, &MainWindow::sigAddOrder, this, &Backend::AddOrder);
    connect(&this->mainWindow, &MainWindow::sigPayOrder, this, &Backend::PayOrder);
    connect(&this->mainWindow, &MainWindow::sigFreeOrder, this, &Backend::FreeOrder);


    connect(&this->mainWindow.loginWindow, (void (LoginWindow::*)(Customer))&LoginWindow::sigLogIn, this, (void (Backend::*)(Customer))&Backend::LoginUser);
    connect(&this->mainWindow.loginWindow, (void (LoginWindow::*)(Shop))&LoginWindow::sigLogIn, this, (void (Backend::*)(Shop))&Backend::LoginUser);
    connect(&this->mainWindow.loginWindow, (void (LoginWindow::*)(Customer))&LoginWindow::sigSignUp, this, (void (Backend::*)(Customer))&Backend::AddUser);
    connect(&this->mainWindow.loginWindow, (void (LoginWindow::*)(Shop))&LoginWindow::sigSignUp, this, (void (Backend::*)(Shop))&Backend::AddUser);
    connect(&this->mainWindow.addProductWindow, &AddProductWindow::sigAddProduct, this, &Backend::AddProduct);
    connect(&this->mainWindow.modifyProductWindow, &ModifyProductWindow::sigModifyProduct, this, &Backend::ModifyProduct);


    ft.setFamily("MS Shell Dlg 2");
    a->setFont(ft);
    mainWindow.init();
}

void Backend::ResetBackend() {
    User_num = 0;
    Product_num = 0;
    SetGeneralConfig();
    std::fstream fs;
    fs.open("data/user.data", std::fstream::out | std::fstream::trunc);
    fs.close();
    fs.open("data/product.data", std::fstream::out | std::fstream::trunc);
    fs.close();
    fs.open("data/chart.data", std::fstream::out | std::fstream::trunc);
    fs.close();
}

void Backend::AddUser(Customer customer) {
  try {
    // check if exsited.
    for(int i=0;i<User_num;i++){
        Customer tmp(i);
        if(!strcmp(customer.nickname, tmp.nickname)) throw std::logic_error("Nickname exist.");
    }

    customer.id = User_num++;

    // change general config
    SetGeneralConfig();

    // add user

    customer.write();

    // add chart
    Chart chart(customer.id, 0);
    chart.Write();

    QMessageBox::information(NULL, "Sign up", "Sign up successfully.");
  }  catch (std::exception &e) {
    QMessageBox::critical(NULL, "Sign up Error", QString(e.what()));
  }
}

void Backend::AddUser(Shop shop) {
  try {
    // check if exsited.
    for(int i=0;i<User_num;i++){
        Shop tmp(i);
        if(!strcmp(shop.nickname, tmp.nickname)) throw std::logic_error("Nickname exist.");
    }

    shop.id = User_num++;
    // change general config
    SetGeneralConfig();

    // add user
    shop.write();

    // add chart
    Chart chart(shop.id, 0);
    chart.Write();

    QMessageBox::information(NULL, "Sign up", "Sign up successfully.");
  }  catch (std::exception &e) {
    QMessageBox::critical(NULL, "Sign up Error", QString(e.what()));
  }
}

void Backend::LoginUser(Customer tmp) {
  try {
    for (int i = 0; i < User_num; i++) {
      Customer customer(i);
      if (strcmp(customer.nickname, tmp.nickname))
        continue;
      else {
        if (strcmp(customer.password, tmp.password))
            throw std::logic_error("Wrong password.");
        else{ // find
            if(tmp.type != customer.type) throw std::logic_error("Wrong type.");
            mainWindow.setLogin(customer);
            Chart chart(i);
            mainWindow.setChart(chart);
            return;
        }
      }
    }
    throw std::logic_error("Nickname not exist.");
  } catch (std::exception &e) {
      QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void Backend::LoginUser(Shop tmp) {
  try {
    for (int i = 0; i < User_num; i++) {
      Shop shop(i);
      if (strcmp(shop.nickname, tmp.nickname))
        continue;
      else {
        if (strcmp(shop.password, tmp.password))
            throw std::logic_error("Wrong password.");
        else{ // find
            if(tmp.type != shop.type) throw std::logic_error("Wrong type.");
            mainWindow.setLogin(shop);
            return;
        }
      }
    }
    throw std::logic_error("Nickname not exist.");
  } catch (std::exception &e) {
      QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void Backend::ModifyPassword(int userID, int type, std::string password) {
    try {
        if(type == 0) { // Customer
            Customer customer(userID);
            customer.ModifyPassword(password);
            customer.write();
        } else { // Shop
            Shop shop(userID);
            shop.ModifyPassword(password);
            shop.write();
        }
        QMessageBox::information(NULL, "Operation", "Modify password successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::ModifyProduct(Product product) {
    try {
        product.write();
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::Recharge(int userID, int type, double amount) {
    try {
        double nowAmount;
        if(type == 0) { // Customer
            Customer customer(userID);
            nowAmount = customer.Recharge(amount);
            customer.write();
        } else { // Shop
            Shop shop(userID);
            nowAmount = shop.Recharge(amount);
            shop.write();
        }
        mainWindow.setRemaining(nowAmount);
    } catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::Consume(int userID, int type, double amount) {
    try {
        double nowAmount;
        if(type == 0) { // Customer
            Customer customer(userID);
            nowAmount = customer.Consume(amount);
            customer.write();
        } else { // Shop
            Shop shop(userID);
            nowAmount = shop.Consume(amount);
            shop.write();
        }
        this->mainWindow.setRemaining(nowAmount);
        QMessageBox::information(NULL, "Operation", "Consume successfully.");
    } catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::AddProduct(int belongID, int stock, int type, double price, std::string name, int feature, std::string description) {
    try {
        // change general config
        int id = Product_num++;
        SetGeneralConfig();

        // add product
        if(type == 1) { // Book
            BookProduct book(id, belongID, stock, price, 1, name, feature, description);
            book.write();
        } else if(type == 2) { // Food
            FoodProduct food(id, belongID, stock, price, 1, name, feature, description);
            food.write();
        } else if(type == 3) { // Cloth
            ClothProduct cloth(id, belongID, stock, price, 1, name, feature, description);
            cloth.write();
        }

        QMessageBox::information(NULL, "Operation", "Added successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SetDiscount(int index, double discount) {
    try {
        Product p(index);
        p.discount = discount;
        p.write();
        QMessageBox::information(NULL, "Operation", "Set discount successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SetDiscount(int shopID, ProductType type, double discount) {
    try {
        for(int i=0;i<Product_num;i++) {
            Product p(i);
            if(p.type == type && p.belong_id == shopID) {
                p.discount = discount;
                p.write();
            }
        }
        QMessageBox::information(NULL, "Operation", "Set discount successfully.");
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SearhProduct(std::string description) {
    try {
        int cnt = 0;
        QVector<Product> productList;
        for(int i=0;i<Product_num;i++) {
            Product p = Product(i);
            if(strstr(p.name, description.c_str())!=NULL || strstr(p.description, description.c_str())!=NULL) {
                cnt++;
                productList.push_back(p);
//                qDebug()<<"find product:"<<i<<name;
            }
        }
//        qDebug()<<"find product number:"<<cnt;
        mainWindow.setSerchList(productList);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::SearhProduct(int shopID) {
    try {
        int cnt=0;
        QVector<Product> productList;
        for(int i=0;i<Product_num;i++) {
            Product p(i);
            if(p.belong_id == shopID) {
                cnt++;
                productList.push_back(p);
//                qDebug()<<"find product:"<<i;
            }
        }
//        qDebug()<<"find product number:"<<cnt;
        mainWindow.setManageList(productList);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::AddToChart(int customerID, int productID, int num) {
    try {
        // update user config
        Customer customer(customerID);
        customer.chart_num++;
        customer.write();

        // update chart config
        Chart chart = Chart(customerID);
        chart.size++;
        for(int i=0;i<MAX_CHART_SIZE;i++)if(!chart.product[i].stock){
            chart.product[i] = Product(productID);
            chart.product[i].stock = num;
            chart.Write();
            break;
        }
        chart.Write();
        mainWindow.setChart(chart);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::GetChart(int customerID) {
    try {
        Chart chart = Chart(customerID);
        mainWindow.setChart(chart);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void Backend::ModifyChart(int customerID, int chartID, int num) {
    Chart chart(customerID);
    if(!num) { // delete now item
        // update customer config
        Customer customer(customerID);
        customer.chart_num--;
        customer.write();
        chart.size--;
    }
    chart.product[chartID].stock = num;
    chart.Write();
    mainWindow.setChart(chart);
}

void Backend::AddOrder(Chart order) {
    try {
        Product p[MAX_CHART_SIZE];
        for(int i=0;i<order.size;i++) {
            p[i] = Product(order.product[i].id);
            if(p[i].stock < order.product[i].stock) throw std::logic_error("Stock insufficient.");
            p[i].stock -= order.product[i].stock;
        }
        // update product config
        for(int i=0; i<order.size;i++){
            p[i].write();
        }
        mainWindow.setOrderState(1);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
        mainWindow.setOrderState(0);
    }
}

void Backend::PayOrder(Chart order) {
    Customer customer(order.customerID);
    try {
        double amount = 0, productAmount;
        for(int i=0;i<order.size;i++){
            productAmount = order.product[i].getPrice() * order.product[i].discount * order.product[i].stock;
            amount += productAmount;
            // update shop config
            Shop shop(order.product[i].belong_id);
            shop.Recharge(productAmount);
            shop.write();
        }

        if(customer.GetBalance() < amount) throw std::logic_error("Balance insufficient.");
        customer.Consume(amount);
        customer.order_num ++;
        customer.write();
        mainWindow.setPayState(1, customer);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
        mainWindow.setPayState(0, customer);
    }
}

void Backend::FreeOrder(Chart order) {
    try {
        for(int i=0;i<order.size;i++) {
            Product p(order.product[i].id);
            p.stock += order.product[i].stock;
            p.write();
        }
        mainWindow.setPayState(2, NULL);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}
