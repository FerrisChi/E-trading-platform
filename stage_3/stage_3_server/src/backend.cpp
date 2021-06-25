#include "backend.h"

static int Product_num;
static int User_num;

//--------------------------------------------------------
// SLOT FUNCTION

void Backend::readClient() {
  QByteArray block;
  QDataStream in(&block, QIODevice::ReadOnly);
  in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
  for (int i = 0; i < tcpSocketList.size(); i++) {
    block = tcpSocketList[i]->readAll();
    if (!block.isEmpty()) {
      tcpSocket = tcpSocketList[i];
      break;
    }
  }

  int tmpI1, tmpI2, tmpI3;
  double tmpD;
  QString tmpQstr;
  Product tmpProudct;
  Chart tmpChart;
  Shop tmpShop;
  Customer tmpCustomer;

  uint8_t opCode;
  in >> nowClientID >> opCode;
  qDebug() << "Recv" << nowClientID << opCode;
  switch (opCode) {
    // RESET
    case RESET:
      ResetBackend();
      qDebug() << "Backend reseted.";
      break;
    // TEST
    case TEST:
      in >> tmpQstr;
      TestTCP(tmpQstr);
      break;

    // MODIFY
    case MODIFY_PASSWORD:
      in >> tmpI1 >> tmpI2 >> tmpQstr;
      ModifyPassword(tmpI1, tmpI2, tmpQstr.toStdString());
      break;
    case RECHARGE:
      in >> tmpI1 >> tmpI2 >> tmpD;
      Recharge(tmpI1, tmpI2, tmpD);
      break;
    case SET_DISCOUNT_INDEX:
      in >> tmpI1 >> tmpD;
      SetDiscount(tmpI1, tmpD);
      break;
    case SET_DISCOUNT_TYPE:
      in >> tmpI1 >> tmpI2 >> tmpD;
      SetDiscount(tmpI1, tmpI2, tmpD);
      break;
    case MODIFY_CHART:
      in >> tmpI1 >> tmpI2 >> tmpI3;
      ModifyChart(tmpI1, tmpI2, tmpI3);
      break;
    case MODIFY_PRODUCT:
      in >> tmpProudct;
      ModifyProduct(tmpProudct);
      break;

    // GET
    case SEARCH_PRODUCT_DESC:
      in >> tmpQstr;
      SearhProduct(tmpQstr.toStdString());
      break;
    case SEARCH_PRODUCT_ID:
      in >> tmpI1;
      SearhProduct(tmpI1);
      break;
    case GET_CHART:
      in >> tmpI1;
      GetChart(tmpI1);
      break;
    case LOGIN:
      in >> tmpI1;
      qDebug() << "Login called.";
      if (!tmpI1) {
        in >> tmpCustomer;
        tmpCustomer.print();
        LoginUser(tmpCustomer);
      } else {
        in >> tmpShop;
        tmpShop.print();
        LoginUser(tmpShop);
      }
      break;
    // ADD
    case ADD_TO_CHART:
      in >> tmpI1 >> tmpI2 >> tmpI3;
      AddToChart(tmpI1, tmpI2, tmpI3);
      break;
    case ADD_PRODUCT:
      in >> tmpProudct;
      AddProduct(tmpProudct);
      break;
    case ADD_ORDER:
      in >> tmpChart;
      AddOrder(tmpChart);
      break;
    case ADD_USER:
      in >> tmpI1;
      qDebug() << "Sign up called.";
      if (!tmpI1) {
        in >> tmpCustomer;
        tmpCustomer.print();
        AddUser(tmpCustomer);
      } else {
        in >> tmpShop;
        tmpShop.print();
        AddUser(tmpShop);
      }
      break;
    // PAY
    case PAY_ORDER:
      in >> tmpChart;
      PayOrder(tmpChart);
      break;
    case FREE_ORDER:
      in >> tmpChart;
      FreeOrder(tmpChart);
      break;
    default:
      qDebug() << "Cannot resolve message. Abandoned.";
  }
}

// userFlag==0: customer; userFlag==1: shop; userFlag==2: NULL
// chartFlag==1: chart; chartFlag==2: NULL
// productFlag==0: searchList;  productFlag=1: manageList; productFlag==2: NULL
void Backend::sendClient(uint8_t op, uint8_t msgFlag, uint8_t userFlag,
                         uint8_t chartFlag, uint8_t productFlag) {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::WriteOnly);
  out << op << msgFlag << userFlag << chartFlag << productFlag;
  qDebug() << "Send message" << op << msgFlag << userFlag << chartFlag
           << productFlag;
  if (userFlag == 0)
    out << svCustomer;
  else if (userFlag == 1)
    out << svShop;
  if (chartFlag == 1) out << svChart;
  if (productFlag != 2) {
    out << (int)svProductList.size();
    for (int i = 0; i < svProductList.size(); i++) out << svProductList[i];
  }
  tcpSocket->write(sendMsg);
  tcpSocket->waitForBytesWritten();
}

void Backend::disconnetClient() { qDebug() << "Client disconnected."; }

void Backend::TestTCP(QString qstr) { qDebug() << qstr; }

//------------------------------------------------------------
// PUBLIC FUNCTION

void Backend::init() {
    mainWindow.show();
    GetGeneralConfig();
    qDebug() << QString("Init System:") << User_num << Product_num;
}

void Backend::SetGeneralConfig() {
  std::fstream fs;
  fs.open("../data/general.data",
          std::fstream::in | std::fstream::out | std::fstream::binary);
  fs.write((char *)&User_num, sizeof(int));
  fs.write((char *)&Product_num, sizeof(int));
  fs.close();
}

void Backend::GetGeneralConfig() {
  std::ifstream ifs;
  ifs.open("../data/general.data", std::ifstream::in | std::ifstream::binary);
  ifs.read((char *)&User_num, sizeof(int));
  ifs.read((char *)&Product_num, sizeof(int));
  ifs.close();
}

void Backend::ResetBackend() {
  User_num = 0;
  Product_num = 0;
  SetGeneralConfig();
  std::fstream fs;
  fs.open("../data/user.data", std::fstream::out | std::fstream::trunc);
  fs.close();
  fs.open("../data/product.data", std::fstream::out | std::fstream::trunc);
  fs.close();
  fs.open("../data/chart.data", std::fstream::out | std::fstream::trunc);
  fs.close();
}

void Backend::AddUser(Customer customer) {
  uint8_t flag = 2;
  try {
    // check if exsited.
    for (int i = 0; i < User_num; i++) {
      Customer tmp(i);
      if (!strcmp(customer.nickname, tmp.nickname)) {
        flag = 1;
        throw std::logic_error("Nickname exist.");
      }
    }

    customer.id = User_num++;

    // change general config
    SetGeneralConfig();

    // add user

    customer.write();
    svCustomer = customer;

    // add chart
    Chart chart(customer.id, 0);
    chart.Write();

    sendClient(ADD_USER, 200, 0, 2, 2);
  } catch (std::exception &e) {
    sendClient(ADD_USER, flag, 2, 2, 2);
  }
}

void Backend::AddUser(Shop shop) {
  uint8_t flag = 2;
  try {
    // check if exsited.
    for (int i = 0; i < User_num; i++) {
      Shop tmp(i);
      if (!strcmp(shop.nickname, tmp.nickname)) {
        flag = 1;
        throw std::logic_error("Nickname exist.");
      }
    }

    shop.id = User_num++;
    // change general config
    SetGeneralConfig();

    // add user
    shop.write();
    svShop = shop;

    // add chart
    Chart chart(shop.id, 0);
    chart.Write();

    sendClient(ADD_USER, 200, 1, 2, 2);
  } catch (std::exception &e) {
    sendClient(ADD_USER, flag, 2, 2, 2);
  }
}

void Backend::LoginUser(Customer tmp) {
  uint8_t flag = 1;
  try {
    for (int i = 0; i < User_num; i++) {
      Customer customer(i);
      if (strcmp(customer.nickname, tmp.nickname))
        continue;
      else {
        if (!customer.MatchPassword(tmp)) {
          flag = 1;
          throw std::logic_error("Wrong password.");
        } else {  // find
          if (tmp.type != customer.type) {
            flag = 2;
            throw std::logic_error("Wrong type.");
          }
          Chart chart(i);
          svChart = chart;
          svCustomer = customer;
          svCustomer.print();
          svChart.Print();
          sendClient(LOGIN, 200, 0, 1, 2);
          return;
        }
      }
    }
    flag = 3;
    throw std::logic_error("Nickname not exist.");
  } catch (std::exception &e) {
    sendClient(LOGIN, flag, 2, 2, 2);
  }
}

void Backend::LoginUser(Shop tmp) {
  uint8_t flag = 3;
  try {
    for (int i = 0; i < User_num; i++) {
      Shop shop(i);
      if (strcmp(shop.nickname, tmp.nickname))
        continue;
      else {
        if (!shop.MatchPassword(tmp)) {
          flag = 1;
          throw std::logic_error("Wrong password");
        } else {  // find
          if (tmp.type != shop.type) {
            flag = 2;
            throw std::logic_error("Wrong type");
          }
          svShop = shop;
          sendClient(LOGIN, 200, 1, 2, 2);
          return;
        }
      }
    }
    flag = 3;
    throw std::logic_error("Nickname not exist.");
  } catch (std::exception &e) {
    sendClient(LOGIN, flag, 2, 2, 2);
  }
}

void Backend::ModifyPassword(int userID, int type, std::string password) {
  try {
    if (type == 0) {
      Customer customer(userID);
      customer.ModifyPassword(password);
      customer.write();
      svCustomer = customer;
      sendClient(MODIFY_PASSWORD, 200, 0, 2, 2);
    } else {
      Shop shop(userID);
      shop.ModifyPassword(password);
      shop.write();
      svShop = shop;
      sendClient(MODIFY_PASSWORD, 200, 1, 2, 2);
    }
  } catch (std::exception &e) {
    sendClient(MODIFY_PASSWORD, 1, 2, 2, 2);
  }
}

void Backend::ModifyProduct(Product product) {
  try {
    product.write();
    sendClient(MODIFY_PRODUCT, 200, 2, 2, 2);
  } catch (std::exception &e) {
    sendClient(MODIFY_PRODUCT, 1, 2, 2, 2);
  }
}

void Backend::Recharge(int userID, int type, double amount) {
  try {
    if (type == 0) {  // Customer
      Customer customer(userID);
      customer.Recharge(amount);
      customer.write();
      svCustomer = customer;
      sendClient(RECHARGE, 200, 0, 2, 2);
    } else {  // Shop
      Shop shop(userID);
      shop.Recharge(amount);
      shop.write();
      svShop = shop;
      sendClient(RECHARGE, 200, 1, 2, 2);
    }
  } catch (std::exception &e) {
    sendClient(RECHARGE, 1, 2, 2, 2);
  }
}

void Backend::Consume(int userID, int type, double amount) {
  try {
    if (type == 0) {  // Customer
      Customer customer(userID);
      customer.Consume(amount);
      customer.write();
      svCustomer = customer;
      sendClient(CONSUME, 200, 0, 2, 2);
    } else {  // Shop
      Shop shop(userID);
      shop.Consume(amount);
      shop.write();
      svShop = shop;
      sendClient(CONSUME, 200, 1, 2, 2);
    }
  } catch (std::exception &e) {
    sendClient(CONSUME, 1, 2, 2, 2);
  }
}

void Backend::AddProduct(Product product) {
  try {
    // change general config
    product.id = Product_num++;
    SetGeneralConfig();
    // add product
    if (product.type == ProductType::Book) {  // Book
      BookProduct book(product);
      book.write();
    } else if (product.type == ProductType::Food) {  // Food
      FoodProduct food(product);
      food.write();
    } else if (product.type == ProductType::Cloth) {  // Cloth
      ClothProduct cloth(product);
      cloth.write();
    }
    sendClient(ADD_PRODUCT, 200, 2, 2, 2);
  } catch (std::exception &e) {
    sendClient(ADD_PRODUCT, 1, 2, 2, 2);
  }
}

void Backend::SetDiscount(int index, double discount) {
  try {
    Product p(index);
    p.discount = discount;
    p.write();
    sendClient(SET_DISCOUNT_INDEX, 200, 2, 2, 2);
  } catch (std::exception &e) {
    sendClient(SET_DISCOUNT_INDEX, 1, 2, 2, 2);
  }
}

void Backend::SetDiscount(int shopID, int type, double discount) {
  try {
    for (int i = 0; i < Product_num; i++) {
      Product p(i);
      if ((int)p.type == type && p.belong_id == shopID) {
        p.discount = discount;
        p.write();
      }
    }
    sendClient(SET_DISCOUNT_TYPE, 200, 2, 2, 2);
  } catch (std::exception &e) {
    sendClient(SET_DISCOUNT_TYPE, 1, 2, 2, 2);
  }
}

void Backend::SearhProduct(std::string description) {
  try {
    int cnt = 0;
    QVector<Product> productList;
    for (int i = 0; i < Product_num; i++) {
      Product p = Product(i);
      if (strstr(p.name, description.c_str()) != NULL ||
          strstr(p.description, description.c_str()) != NULL) {
        cnt++;
        productList.push_back(p);
      }
    }
    svProductList = productList;
    sendClient(SEARCH_PRODUCT_DESC, 200, 2, 2, 0);
  } catch (std::exception &e) {
    sendClient(SEARCH_PRODUCT_DESC, 1, 2, 2, 2);
  }
}

void Backend::SearhProduct(int shopID) {
  try {
    int cnt = 0;
    QVector<Product> productList;
    for (int i = 0; i < Product_num; i++) {
      Product p(i);
      if (p.belong_id == shopID) {
        cnt++;
        productList.push_back(p);
      }
    }
    svProductList = productList;
    sendClient(SEARCH_PRODUCT_ID, 200, 2, 2, 1);
  } catch (std::exception &e) {
    sendClient(SEARCH_PRODUCT_ID, 1, 2, 2, 2);
  }
}

void Backend::AddToChart(int customerID, int productID, int num) {
  uint8_t flag = 2;
  try {
    // update user config
    Customer customer(customerID);
    if (!customer.AddToChart(MAX_CHART_SIZE)) {
      flag = 1;
      throw std::logic_error("Chart full");
    }
    customer.write();
    svCustomer = customer;
    // update chart config
    Chart chart = Chart(customerID);
    chart.size++;
    for (int i = 0; i < MAX_CHART_SIZE; i++)
      if (!chart.product[i].stock) {
        chart.product[i] = Product(productID);
        chart.product[i].stock = num;
        chart.Write();
        break;
      }
    chart.Write();
    svChart = chart;
    sendClient(ADD_TO_CHART, 200, 0, 1, 2);
  } catch (std::exception &e) {
    sendClient(ADD_TO_CHART, flag, 2, 2, 2);
  }
}

void Backend::GetChart(int customerID) {
  try {
    Chart chart = Chart(customerID);
    svChart = chart;
    sendClient(GET_CHART, 200, 2, 1, 2);
  } catch (std::exception &e) {
    sendClient(GET_CHART, 1, 2, 2, 2);
  }
}

void Backend::ModifyChart(int customerID, int chartID, int num) {
  try {
    int userFlag = 2;
    Chart chart(customerID);
    if (!num) {  // delete now item
      // update customer config
      Customer customer(customerID);
      customer.RemoveFromChart();
      customer.write();
      chart.size--;
      svCustomer = customer;
      userFlag = 0;
    }
    chart.product[chartID].stock = num;
    chart.Write();
    svChart = chart;
    sendClient(MODIFY_CHART, 200, userFlag, 1, 2);
  } catch (std::exception &e) {
    sendClient(MODIFY_CHART, 1, 2, 2, 2);
  }
}

void Backend::AddOrder(Chart order) {
  try {
    Product p[MAX_CHART_SIZE];
    for (int i = 0; i < order.size; i++) {
      p[i] = Product(order.product[i].id);
      if (p[i].stock < order.product[i].stock)
        throw std::logic_error("Stock insufficient.");
      p[i].stock -= order.product[i].stock;
    }
    // update product config
    for (int i = 0; i < order.size; i++) {
      p[i].write();
    }
    sendClient(ADD_ORDER, 200, 2, 2, 2);
  } catch (std::exception &e) {
    sendClient(ADD_ORDER, 1, 2, 2, 2);
  }
}

void Backend::PayOrder(Chart order) {
  Customer customer(order.customerID);
  try {
    double amount = 0, productAmount;
    for (int i = 0; i < order.size; i++) {
      productAmount = order.product[i].getPrice() * order.product[i].discount *
                      order.product[i].stock;
      amount += productAmount;
      // update shop config
      Shop shop(order.product[i].belong_id);
      shop.Recharge(productAmount);
      shop.write();
    }

    if (customer.GetBalance() < amount)
      throw std::logic_error("Balance insufficient.");
    customer.Consume(amount);
    customer.write();
    svCustomer = customer;
    sendClient(PAY_ORDER, 200, 0, 2, 2);
  } catch (std::exception &e) {
    sendClient(PAY_ORDER, 1, 2, 2, 2);
  }
}

void Backend::FreeOrder(Chart order) {
  try {
    for (int i = 0; i < order.size; i++) {
      Product p(order.product[i].id);
      p.stock += order.product[i].stock;
      p.write();
    }
    order.Print();
    sendClient(FREE_ORDER, 200, 2, 2, 2);
  } catch (std::exception &e) {
    sendClient(FREE_ORDER, 1, 2, 2, 2);
  }
}
