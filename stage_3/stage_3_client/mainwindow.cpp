#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  connect(this, SIGNAL(sigLoginWindow()), &this->loginWindow, SLOT(show()));
  connect(this, &MainWindow::sigAddProductWindow, &this->addProductWindow,
          &AddProductWindow::init);
  connect(this, &MainWindow::sigModifyWindow, &this->modifyProductWindow,
          &ModifyProductWindow::init);
  connect(&this->loginWindow,
          (void (LoginWindow::*)(Customer)) & LoginWindow::sigLogIn, this,
          (void (MainWindow::*)(Customer)) & MainWindow::Login);
  connect(&this->loginWindow,
          (void (LoginWindow::*)(Shop)) & LoginWindow::sigLogIn, this,
          (void (MainWindow::*)(Shop)) & MainWindow::Login);
  connect(&this->loginWindow,
          (void (LoginWindow::*)(Customer)) & LoginWindow::sigSignUp, this,
          (void (MainWindow::*)(Customer)) & MainWindow::Signup);
  connect(&this->loginWindow,
          (void (LoginWindow::*)(Shop)) & LoginWindow::sigSignUp, this,
          (void (MainWindow::*)(Shop)) & MainWindow::Signup);
  connect(&this->addProductWindow, &AddProductWindow::sigAddProduct, this,
          &MainWindow::AddProduct);
  connect(&this->modifyProductWindow, &ModifyProductWindow::sigModifyProduct,
          this, &MainWindow::ModifyProduct);

  payState = 1;
  loginState = UserType::Unknown;

  // socket
  tcpSocket = new QTcpSocket();
  connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ReciveData()));
  tcpSocket->connectToHost("127.0.0.1", 8888);
  if (!tcpSocket->waitForConnected(3000)) {
    QMessageBox::information(this, "Netwrok", "连接服务端失败！");
    return;
  } else {
    QMessageBox::information(this, "Network", "Connection successfully.");
  }

  uiInit();
}

MainWindow::~MainWindow() {
  if (!payState) {
    QMessageBox::information(NULL, "Suggestion", "Order abandoned.");
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)FREE_ORDER << order;
    SendData(sendMsg);
  }
  delete ui;
}

//------------------------------------------------
// PRIVATE FUNCTION

void MainWindow::uiInit() {
  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint);
  setWindowFlags(Qt::WindowCloseButtonHint);

  ui->chargeAmount->setValidator(new QDoubleValidator(0, 100000, 2));
  ui->discount->setValidator(new QDoubleValidator(0, 1, 2));
  ui->number->setValidator(new QIntValidator(1, 100));

  ui->pay->setText("");
  ui->number->hide();
  ui->addToChartButton->hide();

  // searchTableView
  searchModel = new QStandardItemModel;
  this->ui->searchTableView->setModel(searchModel);
  ui->searchTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->searchTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->searchTableView->setSelectionMode(QAbstractItemView::SingleSelection);

  this->searchModel->setHorizontalHeaderItem(0, new QStandardItem("商品名"));
  this->searchModel->setHorizontalHeaderItem(1, new QStandardItem("商品类型"));
  this->searchModel->setHorizontalHeaderItem(2, new QStandardItem("现价"));
  this->searchModel->setHorizontalHeaderItem(3, new QStandardItem("库存"));
  this->searchModel->setHorizontalHeaderItem(4, new QStandardItem("商家编号"));
  this->searchModel->setHorizontalHeaderItem(5, new QStandardItem("商品描述"));

  this->ui->searchTableView->setColumnWidth(0, 100);  // width
  this->ui->searchTableView->setColumnWidth(1, 80);
  this->ui->searchTableView->setColumnWidth(2, 80);
  this->ui->searchTableView->setColumnWidth(3, 80);
  this->ui->searchTableView->setColumnWidth(4, 80);
  this->ui->searchTableView->setColumnWidth(5, 280);

  // modifyTableView
  manageModel = new QStandardItemModel;
  this->ui->manageTableView->setModel(manageModel);
  ui->manageTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->manageTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->manageTableView->setSelectionMode(QAbstractItemView::SingleSelection);

  this->manageModel->setHorizontalHeaderItem(0, new QStandardItem("商品名"));
  this->manageModel->setHorizontalHeaderItem(1, new QStandardItem("商品类型"));
  this->manageModel->setHorizontalHeaderItem(2, new QStandardItem("原价"));
  this->manageModel->setHorizontalHeaderItem(3, new QStandardItem("库存"));
  this->manageModel->setHorizontalHeaderItem(4, new QStandardItem("折扣"));
  this->manageModel->setHorizontalHeaderItem(5, new QStandardItem("商品描述"));

  this->ui->manageTableView->setColumnWidth(0, 100);  // width
  this->ui->manageTableView->setColumnWidth(1, 80);
  this->ui->manageTableView->setColumnWidth(2, 80);
  this->ui->manageTableView->setColumnWidth(3, 80);
  this->ui->manageTableView->setColumnWidth(4, 80);
  this->ui->manageTableView->setColumnWidth(5, 280);

  // chartTableView
  chartModel = new QStandardItemModel;
  this->ui->chartTableView->setModel(chartModel);
  ui->chartTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->chartTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->chartTableView->setSelectionMode(QAbstractItemView::SingleSelection);

  this->chartModel->setHorizontalHeaderItem(0, new QStandardItem("商品名"));
  this->chartModel->setHorizontalHeaderItem(1, new QStandardItem("商品类型"));
  this->chartModel->setHorizontalHeaderItem(2, new QStandardItem("现价"));
  this->chartModel->setHorizontalHeaderItem(3, new QStandardItem("商家编号"));
  this->chartModel->setHorizontalHeaderItem(4, new QStandardItem("商品描述"));
  this->chartModel->setHorizontalHeaderItem(5, new QStandardItem("数量"));
  this->chartModel->setHorizontalHeaderItem(6, new QStandardItem("选择状态"));

  this->ui->chartTableView->setColumnWidth(0, 100);  // width
  this->ui->chartTableView->setColumnWidth(1, 80);
  this->ui->chartTableView->setColumnWidth(2, 80);
  this->ui->chartTableView->setColumnWidth(3, 80);
  this->ui->chartTableView->setColumnWidth(4, 230);
  this->ui->chartTableView->setColumnWidth(5, 50);
  this->ui->chartTableView->setColumnWidth(6, 80);

  // orderTableView
  orderModel = new QStandardItemModel;
  this->ui->orderTableView->setModel(orderModel);
  ui->orderTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->orderTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->orderTableView->setSelectionMode(QAbstractItemView::SingleSelection);

  this->orderModel->setHorizontalHeaderItem(0, new QStandardItem("商品名"));
  this->orderModel->setHorizontalHeaderItem(1, new QStandardItem("价格"));
  this->orderModel->setHorizontalHeaderItem(2, new QStandardItem("数量"));

  this->ui->orderTableView->setColumnWidth(0, 100);  // width
  this->ui->orderTableView->setColumnWidth(1, 80);
  this->ui->orderTableView->setColumnWidth(2, 80);

  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_testTCPButton_clicked() {
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
  std::string str = "你好tcp";
  out << clientID << (uint8_t)TEST << QString::fromStdString(str);
  SendData(block);
}

void MainWindow::setCustomer() {
  loginState = UserType::Customer;

  // ui
  ui->remaining->setText(QString::asprintf("%.2f", customer.GetBalance()));
  ui->loginButton->setText("登出");
  ui->stackedWidget->setCurrentIndex(3);
  ui->orderPageButton->setText("购物车");
  ui->number->show();
  ui->addToChartButton->show();
  ui->newPassword->setText("");
  ui->chargeAmount->setText("");
  ui->remaining->setText(QString::asprintf("%.2f", customer.GetBalance()));
  ui->nickname->setText(QString(customer.nickname));
}

void MainWindow::setShop() {
  loginState = UserType::Shop;

  // ui
  ui->remaining->setText(QString::asprintf("%.2f", shop.GetBalance()));
  ui->loginButton->setText("登出");
  ui->stackedWidget->setCurrentIndex(3);
  ui->orderPageButton->setText("商品管理");
  ui->number->hide();
  ui->addToChartButton->hide();
  ui->remaining->setText(QString::asprintf("%.2f", shop.GetBalance()));
  ui->nickname->setText(QString(shop.nickname));
}

void MainWindow::setSerchList() {
  searchModel->removeRows(0, searchModel->rowCount());
  for (int i = 0; i < searchList.size(); i++) {
    Product p = searchList[i];
    searchModel->setItem(i, 0, new QStandardItem(p.name));
    searchModel->setItem(
        i, 1,
        new QStandardItem(QString::fromStdString(ProductTypeStr[(int)p.type])));
    searchModel->setItem(
        i, 2,
        new QStandardItem(QString::number(p.getPrice() * p.discount, 'f', 2)));
    searchModel->setItem(i, 3,
                         new QStandardItem(QString::number((int)p.stock)));
    searchModel->setItem(i, 4,
                         new QStandardItem(QString::number((int)p.belong_id)));
    searchModel->setItem(i, 5, new QStandardItem(p.description));
  }
}

void MainWindow::setManageList() {
  manageModel->removeRows(0, manageModel->rowCount());
  for (int i = 0; i < manageList.size(); i++) {
    Product p = manageList[i];
    manageModel->setItem(i, 0, new QStandardItem(p.name));
    manageModel->setItem(
        i, 1,
        new QStandardItem(QString::fromStdString(ProductTypeStr[(int)p.type])));
    manageModel->setItem(i, 2,
                         new QStandardItem(QString::number(p.price, 'f', 2)));
    manageModel->setItem(i, 3,
                         new QStandardItem(QString::number((int)p.stock)));
    manageModel->setItem(
        i, 4, new QStandardItem(QString::number(p.discount, 'f', 2)));
    manageModel->setItem(i, 5, new QStandardItem(p.description));
  }
}

void MainWindow::setChart() {
  chartModel->removeRows(0, chartModel->rowCount());
  chart.Print();
  int cnt = 0;
  for (int i = 0; i < MAX_CHART_SIZE; i++)
    if (chart.product[i].stock) {
      Product p = chart.product[i];
      chartModel->setItem(cnt, 0, new QStandardItem(p.name));
      chartModel->setItem(cnt, 1,
                          new QStandardItem(QString::fromStdString(
                              ProductTypeStr[(int)p.type])));
      chartModel->setItem(cnt, 2,
                          new QStandardItem(QString::number(
                              p.getPrice() * p.discount, 'f', 2)));
      chartModel->setItem(cnt, 3,
                          new QStandardItem(QString::number((int)p.belong_id)));
      chartModel->setItem(cnt, 4, new QStandardItem(p.description));
      chartModel->setItem(cnt, 5,
                          new QStandardItem(QString::number((int)p.stock)));
      chartItem[cnt] = new QStandardItem();
      chartItem[cnt]->setCheckable(true);
      chartItem[cnt]->setCheckState(Qt::Unchecked);
      chartItem[cnt]->setEditable(false);
      chartModel->setItem(cnt, 6, chartItem[cnt]);
      cnt++;
    }
}

void MainWindow::setOrder() {
  payState = 0;

  // ui
  orderModel->removeRows(0, orderModel->rowCount());
  for (int i = 0; i < order.size; i++) {
    Product p = order.product[i];
    orderModel->setItem(i, 0, new QStandardItem(p.name));
    orderModel->setItem(
        i, 1,
        new QStandardItem(QString::number(p.getPrice() * p.discount, 'f', 2)));
    orderModel->setItem(i, 2, new QStandardItem(QString::number((int)p.stock)));
  }
  ui->pay->setText(QString::asprintf("%.2lf", pay));
  QMessageBox::information(NULL, "Opeartion",
                           "Order generated. Please pay in \"我的\"");
}

void MainWindow::setPayState(int flag) {
  if (flag == 1) {  // pay successfully
    payState = 1;
    order = Chart();
    orderModel->removeRows(0, orderModel->rowCount());

    // ui
    ui->remaining->setText(QString::asprintf("%.2f", customer.GetBalance()));
    ui->pay->setText("");
    QMessageBox::information(NULL, "Opeartion", "Pay successfully.");
  } else if (flag == 2) {  // canceled.
    payState = 1;
    order = Chart();
    orderModel->removeRows(0, orderModel->rowCount());

    // ui
    ui->pay->setText("");
    QMessageBox::information(NULL, "Opeartion", "Order canceled successfully.");
  }
}

void MainWindow::searchProduct(QString description) {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::ReadWrite);
  out << clientID << (uint8_t)SEARCH_PRODUCT_DESC << description;
  SendData(sendMsg);
}

void MainWindow::searchProduct(int shopID) {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::ReadWrite);
  out << clientID << (uint8_t)SEARCH_PRODUCT_ID << shopID;
  SendData(sendMsg);
}

//------------------------------------------------------
// SLOT FUNCTION

void MainWindow::ReciveData() {
  QByteArray recvMsg = tcpSocket->readAll();
  QDataStream in(&recvMsg, QIODevice::ReadOnly);
  uint8_t opCode, flag, userFlag, chartFlag, productFlag;
  in >> opCode >> flag >> userFlag >> chartFlag >> productFlag;
  qDebug() << "Recv " << opCode << flag << userFlag << chartFlag << productFlag;
  if (userFlag == 0) {
    in >> customer;
    setCustomer();
    customer.print();
  } else if (userFlag == 1) {
    in >> shop;
    setShop();
    shop.print();
  }
  if (chartFlag == 1) {
    in >> chart;
    setChart();
  }
  if (productFlag == 0) {
    int cnt;
    Product p;
    in >> cnt;
    searchList.clear();
    for (int i = 0; i < cnt; i++) {
      in >> p;
      searchList.push_back(p);
    }
    setSerchList();
  } else if (productFlag == 1) {
    int cnt;
    Product p;
    in >> cnt;
    manageList.clear();
    for (int i = 0; i < cnt; i++) {
      in >> p;
      manageList.push_back(p);
    }
    setManageList();
  }
  switch (opCode) {
    case MODIFY_PASSWORD:
      if (flag == 200)
        QMessageBox::information(NULL, "Operation",
                                 "Password modified successfully.");
      else
        QMessageBox::critical(NULL, "Error", "Password cannot modified.");
      break;
    case RECHARGE:
      if (flag == 200)
        QMessageBox::information(NULL, "Operation", "Recharge successfully.");
      else
        QMessageBox::critical(NULL, "Error", "Recharge Error");
      break;
    case SET_DISCOUNT_INDEX:
    case SET_DISCOUNT_TYPE:
      if (flag == 200)
        QMessageBox::information(NULL, "Operation",
                                 "Set discount successfully.");
      else
        QMessageBox::critical(NULL, "Error", "Set discount Error");
      break;
    case MODIFY_CHART:
    case MODIFY_PRODUCT:
      if (flag == 200)
        QMessageBox::information(NULL, "Operation", "Modified successfully.");
      else
        QMessageBox::critical(NULL, "Error", "Modified Error");
      break;
    case SEARCH_PRODUCT_DESC:
    case SEARCH_PRODUCT_ID:
      if (flag != 200) QMessageBox::critical(NULL, "Error", "Search Error");
      break;
    case GET_CHART:
      if (flag != 200) QMessageBox::critical(NULL, "Error", "Get chart Error");
      break;
    case LOGIN:
      if (flag == 200)
        QMessageBox::information(NULL, "Operation", "Welcom!");
      else if (flag == 1)
        QMessageBox::critical(NULL, "Error", "Wrong password Error");
      else if (flag == 2)
        QMessageBox::critical(NULL, "Error", "Wrong type.");
      else if (flag == 3)
        QMessageBox::critical(NULL, "Error", "Nickname not exist.");
      break;
    case ADD_TO_CHART:
      if (flag == 200)
        QMessageBox::information(NULL, "Operation",
                                 "Add to chart successfully.");
      else
        QMessageBox::critical(NULL, "Error", "Chart is full.");
      break;
    case ADD_PRODUCT:
      if (flag == 200)
        QMessageBox::information(NULL, "Operation",
                                 "Product added successfully.");
      else
        QMessageBox::critical(NULL, "Error", "Add product Error");
      break;
    case ADD_ORDER:
      if (flag == 200)
        setOrder();
      else
        QMessageBox::critical(NULL, "Error", "Insufficient stock.");
      break;
    case ADD_USER:
      if (flag == 200)
        QMessageBox::information(NULL, "Sign up", "Sign up successfully.");
      else if (flag == 1)
        QMessageBox::critical(NULL, "Error", "Nickname exist.");
      else if (flag == 2)
        QMessageBox::critical(NULL, "Error", "Sign up Error");
      break;
    case PAY_ORDER:
      if (flag == 200)
        setPayState(1);
      else
        QMessageBox::critical(NULL, "Error", "Pay order Error.");
      break;
    case FREE_ORDER:
      if (flag == 200)
        setPayState(2);
      else
        QMessageBox::critical(NULL, "Error", "Free order Error.");
      break;
    case CONNECT:
      clientID = flag;
      qDebug() << "Now client id:" << clientID;
      break;
    default:
      qDebug() << "Cannot resolve message. Abandoned.";
  }
}

void MainWindow::SendData(QByteArray sendMsg) {
  qDebug() << "Sending message";
  int sendRe = tcpSocket->write(sendMsg);
  tcpSocket->waitForBytesWritten();
  if (sendRe == -1) {
    QMessageBox::critical(this, "Network", "Send information failed.");
    return;
  }
}

void MainWindow::on_resetBackendButton_clicked() {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::ReadWrite);
  out << clientID << (uint8_t)RESET;
  SendData(sendMsg);
  this->close();
}

void MainWindow::on_loginButton_clicked() {
  try {
    if (loginState == UserType::Unknown) {
      emit sigLoginWindow();
    } else {  // logout
      if (!payState) throw std::logic_error("Order unpay.");
      loginState = UserType::Unknown;
      searchProduct("");

      // ui
      ui->loginButton->setText("登录");
      ui->orderPageButton->setText("登陆后查看");
      ui->stackedWidget->setCurrentIndex(0);
      ui->number->hide();
      ui->addToChartButton->hide();
    }
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_modifypasswordButton_clicked() {
  try {
    if (ui->newPassword->text().isEmpty())
      throw std::logic_error("Password cannot be null");
    std::string newpassword = ui->newPassword->text().toStdString();
    if (loginState == UserType::Unknown) throw std::logic_error("Not login.");
    if (newpassword.length() > MAX_STRING_LENGTH)
      throw std::logic_error("Password too long.");

    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::WriteOnly);
    out << clientID << (uint8_t)MODIFY_PASSWORD;

    if (loginState == UserType::Customer) {
      out << customer.id << (int)loginState << ui->newPassword->text();
    } else {
      out << shop.id << (int)loginState << ui->newPassword->text();
    }
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_chargeButton_clicked() {
  try {
    if (ui->chargeAmount->text().isEmpty())
      throw std::logic_error("Amount cannot be null.");
    if (loginState == UserType::Unknown) throw std::logic_error("Not login.");
    double amount = ui->chargeAmount->text().toDouble();
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::WriteOnly);
    out << clientID << (uint8_t)RECHARGE;

    if (loginState == UserType::Customer) {
      out << customer.id << (int)loginState << amount;
    } else {
      out << shop.id << (int)loginState << amount;
    }
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_myButton_clicked() {
  if (loginState == UserType::Unknown) {
    QMessageBox::critical(NULL, "Error", "Not login.");
  } else {
    ui->stackedWidget->setCurrentIndex(3);
  }
}

void MainWindow::on_mainPageButton_clicked() {
  ui->stackedWidget->setCurrentIndex(0);
  ui->searchDescription->clear();
  searchProduct("");
}

void MainWindow::on_orderPageButton_clicked() {
  if (loginState == UserType::Unknown) {
    QMessageBox::critical(NULL, "Error", "Not login.");
  } else if (loginState == UserType::Customer) {  // Customer
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)GET_CHART << customer.id;
    SendData(sendMsg);
    ui->stackedWidget->setCurrentIndex(1);
  } else {  // Shop
    searchProduct(shop.id);
    ui->stackedWidget->setCurrentIndex(2);
    ui->discount->setText("1");
  }
}

void MainWindow::on_addProductButton_clicked() {
  emit sigAddProductWindow(shop.id);
}

void MainWindow::on_searchButton_clicked() {
  searchProduct(ui->searchDescription->text());
}

void MainWindow::on_manageTableView_doubleClicked(const QModelIndex &index) {
  int row = index.row();
  emit sigModifyWindow(manageList[row]);
}

void MainWindow::on_discountButton_clicked() {
  try {
    if (ui->discount->text().isEmpty())
      throw std::logic_error("Discount cannot be null.");
    double discount = ui->discount->text().toDouble();
    if (ui->selectedProductButton->isChecked()) {
      int curRow = ui->manageTableView->currentIndex().row();
      if (curRow < 0) throw std::logic_error("Please select a line.");
      QByteArray sendMsg;
      QDataStream out(&sendMsg, QIODevice::ReadWrite);
      out << clientID << (uint8_t)SET_DISCOUNT_INDEX << manageList[curRow].id
          << discount;
      SendData(sendMsg);
    } else {
      ProductType type;
      if (ui->allBookButton->isChecked()) {
        type = ProductType::Book;
      } else if (ui->allFoodButton->isChecked()) {
        type = ProductType::Food;
      } else {
        type = ProductType::Cloth;
      }
      QByteArray sendMsg;
      QDataStream out(&sendMsg, QIODevice::ReadWrite);
      out << clientID << (uint8_t)SET_DISCOUNT_TYPE << shop.id << (int)type
          << discount;
      SendData(sendMsg);
    }
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_addToChartButton_clicked() {
  try {
    if (ui->number->text().isEmpty())
      throw std::logic_error("Product number cannot be null.");
    int row = ui->searchTableView->currentIndex().row();
    if (row < 0) throw std::logic_error("Please select a line.");
    int number = ui->number->text().toInt();
    if (number <= 0 || number >= 1000)
      throw std::logic_error("Please input number between 1 and 999.");
    searchList[row].print();
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)ADD_TO_CHART << customer.id
        << searchList[row].id << number;
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_modifyChartButton_clicked() {
  try {
    if (ui->modifyChartNum->text().isEmpty())
      throw std::logic_error("Product number cannot be null.");
    int chartID = ui->chartTableView->currentIndex().row();
    if (chartID < 0) throw std::logic_error("Please select a line.");
    int num = ui->modifyChartNum->text().toInt();
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)MODIFY_CHART << chart.customerID << chartID
        << num;
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_orderButton_clicked() {
  try {
    if (!payState) throw std::logic_error("Please handle old order first.");
    order = Chart();
    pay = 0;
    order.customerID = customer.id;
    int cnt = 0;
    for (int i = 0; i < chartModel->rowCount(); i++)
      if (chartItem[i]->checkState() == Qt::Checked) {
        pay += chart.product[i].getPrice() * chart.product[i].discount *
               chart.product[i].stock;
        order.product[order.size++] = chart.product[i];
        cnt++;
      }
    if (cnt == 0) throw std::logic_error("Please choose at list one item.");
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)ADD_ORDER << order;
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_cancelButton_clicked() {
  try {
    if (payState) throw std::logic_error("No order to cancel");
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)FREE_ORDER << order;
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_payButton_clicked() {
  try {
    if (payState) throw std::logic_error("No order to pay.");
    if (pay > customer.GetBalance())
      throw std::logic_error("Balance insufficient.");
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)PAY_ORDER << order;
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_MainWindow_destroyed() {
  if (!payState) {
    QMessageBox::information(NULL, "Suggestion", "Order abandoned.");
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)FREE_ORDER << order;
    SendData(sendMsg);
  }
}

void MainWindow::AddProduct(Product p) {
  try {
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)ADD_PRODUCT << p;
    p.print();
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::ModifyProduct(Product p) {
  try {
    QByteArray sendMsg;
    QDataStream out(&sendMsg, QIODevice::ReadWrite);
    out << clientID << (uint8_t)MODIFY_PRODUCT << p;
    p.print();
    SendData(sendMsg);
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_allProductButton_clicked() { searchProduct(""); }

void MainWindow::Login(Customer tmpCustomer) {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::ReadWrite);
  out << clientID << (uint8_t)LOGIN << (int)UserType::Customer << tmpCustomer;
  SendData(sendMsg);
}

void MainWindow::Login(Shop tmpShop) {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::ReadWrite);
  out << clientID << (uint8_t)LOGIN << (int)UserType::Shop << tmpShop;
  SendData(sendMsg);
}

void MainWindow::Signup(Customer tmpCustomer) {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::ReadWrite);
  out << clientID << (uint8_t)ADD_USER << (int)UserType::Customer
      << tmpCustomer;
  SendData(sendMsg);
}

void MainWindow::Signup(Shop tmpShop) {
  QByteArray sendMsg;
  QDataStream out(&sendMsg, QIODevice::ReadWrite);
  out << clientID << (uint8_t)ADD_USER << (int)UserType::Shop << tmpShop;
  SendData(sendMsg);
}

// -----------------------------------------------------
// DEBUG function

void MainWindow::on_pushButton_clicked() {
  try {
    if (loginState == UserType::Unknown) throw std::logic_error("Not login.");
    if (loginState == UserType::Customer) customer.print();
    if (loginState == UserType::Shop) shop.print();
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_pushButton_2_clicked() {
  try {
    chart.Print();
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}

void MainWindow::on_pushButton_3_clicked() {
  try {
    order.Print();
  } catch (std::exception &e) {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
  }
}
