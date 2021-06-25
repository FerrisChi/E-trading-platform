#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    connect(this, SIGNAL(sigLoginWindow()), &this->loginWindow, SLOT(show()));
    connect(this, &MainWindow::sigAddProductWindow, &this->addProductWindow, &AddProductWindow::init);
    connect(this, &MainWindow::sigModifyWindow, &this->modifyProductWindow, &ModifyProductWindow::init);

    payState = 1;
    loginState = UserType::Unknown;

    // ui
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowCloseButtonHint);

    ui->chargeAmount->setValidator(new QDoubleValidator(0,100000,2));
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
    ui->searchTableView->setSelectionMode( QAbstractItemView::SingleSelection);

    this->searchModel->setHorizontalHeaderItem(0, new QStandardItem("商品名") );
    this->searchModel->setHorizontalHeaderItem(1, new QStandardItem("商品类型"));
    this->searchModel->setHorizontalHeaderItem(2, new QStandardItem("现价"));
    this->searchModel->setHorizontalHeaderItem(3, new QStandardItem("库存"));
    this->searchModel->setHorizontalHeaderItem(4, new QStandardItem("商家编号"));
    this->searchModel->setHorizontalHeaderItem(5, new QStandardItem("商品描述"));

    this->ui->searchTableView->setColumnWidth(0, 100);    //width
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
    ui->manageTableView->setSelectionMode( QAbstractItemView::SingleSelection);

    this->manageModel->setHorizontalHeaderItem(0, new QStandardItem("商品名") );
    this->manageModel->setHorizontalHeaderItem(1, new QStandardItem("商品类型"));
    this->manageModel->setHorizontalHeaderItem(2, new QStandardItem("原价"));
    this->manageModel->setHorizontalHeaderItem(3, new QStandardItem("库存"));
    this->manageModel->setHorizontalHeaderItem(4, new QStandardItem("折扣"));
    this->manageModel->setHorizontalHeaderItem(5, new QStandardItem("商品描述"));

    this->ui->manageTableView->setColumnWidth(0, 100);    //width
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
    ui->chartTableView->setSelectionMode( QAbstractItemView::SingleSelection);

    this->chartModel->setHorizontalHeaderItem(0, new QStandardItem("商品名") );
    this->chartModel->setHorizontalHeaderItem(1, new QStandardItem("商品类型"));
    this->chartModel->setHorizontalHeaderItem(2, new QStandardItem("现价"));
    this->chartModel->setHorizontalHeaderItem(3, new QStandardItem("商家编号"));
    this->chartModel->setHorizontalHeaderItem(4, new QStandardItem("商品描述"));
    this->chartModel->setHorizontalHeaderItem(5, new QStandardItem("数量"));
    this->chartModel->setHorizontalHeaderItem(6, new QStandardItem("选择状态") );

    this->ui->chartTableView->setColumnWidth(0, 100);    //width
    this->ui->chartTableView->setColumnWidth(1, 80);
    this->ui->chartTableView->setColumnWidth(2, 80);
    this->ui->chartTableView->setColumnWidth(3, 80);
    this->ui->chartTableView->setColumnWidth(4, 230);
    this->ui->chartTableView->setColumnWidth(5, 80);
    this->ui->chartTableView->setColumnWidth(6, 50);

    // orderTableView
    orderModel = new QStandardItemModel;
    this->ui->orderTableView->setModel(orderModel);
    ui->orderTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->orderTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->orderTableView->setSelectionMode( QAbstractItemView::SingleSelection);

    this->orderModel->setHorizontalHeaderItem(0, new QStandardItem("商品名") );
    this->orderModel->setHorizontalHeaderItem(1, new QStandardItem("价格"));
    this->orderModel->setHorizontalHeaderItem(2, new QStandardItem("数量"));

    this->ui->orderTableView->setColumnWidth(0, 80);    //width
    this->ui->orderTableView->setColumnWidth(1, 80);
    this->ui->orderTableView->setColumnWidth(2, 50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {
    ui->stackedWidget->setCurrentIndex(0);
    this->show();
}

void MainWindow::setLogin(Customer customer) {
    loginState = UserType::Customer;
    this->customer = customer;

    // ui
    ui->loginButton->setText("登出");
    ui->stackedWidget->setCurrentIndex(3);
    ui->orderPageButton->setText("购物车");
    ui->number->show();
    ui->addToChartButton->show();
    ui->newPassword->setText("");
    ui->chargeAmount->setText("");
    ui->remaining->setText(QString::asprintf("%.2f", customer.GetBalance()));
    ui->nickname->setText(QString(customer.nickname));
    QMessageBox::information(NULL, "Login Successfully", QString(customer.nickname));
}

void MainWindow::setLogin(Shop shop) {
    loginState = UserType::Shop;
    this->shop = shop;

    // ui
    ui->loginButton->setText("登出");
    ui->stackedWidget->setCurrentIndex(3);
    ui->orderPageButton->setText("商品管理");
    ui->remaining->setText(QString::asprintf("%.2f", shop.GetBalance()));
    ui->nickname->setText(QString(shop.nickname));
    QMessageBox::information(NULL, "Login Successfully", QString(shop.nickname));
}

void MainWindow::setRemaining(double amount) {
    if(loginState == UserType::Customer) {
        customer.balance = amount;
    } else if(loginState == UserType::Shop) {
        shop.balance = amount;
    }

    // ui
    QString s = QString::asprintf("%.2f",amount);
    ui->remaining->setText(s);
    QMessageBox::information(NULL, "Operation", "Recharge successfully.");
}

void MainWindow::setSerchList(QVector<Product> productList) {
    searchModel->removeRows(0, searchModel->rowCount());
    searchList = productList;
    for(int i=0;i<productList.size();i++) {
        Product p = productList[i];
        searchModel->setItem(i, 0, new QStandardItem(p.name));
        searchModel->setItem(i, 1, new QStandardItem(QString::fromStdString(ProductTypeStr[(int)p.type])));
        searchModel->setItem(i, 2, new QStandardItem(QString::number(p.getPrice() * p.discount, 'f', 2)));
        searchModel->setItem(i, 3, new QStandardItem(QString::number((int)p.stock)));
        searchModel->setItem(i, 4, new QStandardItem(QString::number((int)p.belong_id)));
        searchModel->setItem(i, 5, new QStandardItem(p.description));
    }
}

void MainWindow::setManageList(QVector<Product> productList) {
    manageModel->removeRows(0, manageModel->rowCount());
    this->manageList = productList;
    for(int i=0;i<productList.size();i++) {
        Product p = productList[i];
        manageModel->setItem(i, 0, new QStandardItem(p.name));
        manageModel->setItem(i, 1, new QStandardItem(QString::fromStdString(ProductTypeStr[(int)p.type])));
        manageModel->setItem(i, 2, new QStandardItem(QString::number(p.price, 'f', 2)));
        manageModel->setItem(i, 3, new QStandardItem(QString::number((int)p.stock)));
        manageModel->setItem(i, 4, new QStandardItem(QString::number(p.discount, 'f', 2)));
        manageModel->setItem(i, 5, new QStandardItem(p.description));
    }
}

void MainWindow::setChart(Chart chart) {
    chartModel->removeRows(0, chartModel->rowCount());
    this->chart = chart;
    int cnt = 0;
    for(int i=0;i<MAX_CHART_SIZE;i++) if(chart.product[i].stock){
        Product p = chart.product[i];
        chartModel->setItem(cnt, 0, new QStandardItem(p.name));
        chartModel->setItem(cnt, 1, new QStandardItem(QString::fromStdString(ProductTypeStr[(int)p.type])));
        chartModel->setItem(cnt, 2, new QStandardItem(QString::number(p.getPrice() * p.discount, 'f', 2)));
        chartModel->setItem(cnt, 3, new QStandardItem(QString::number((int)p.belong_id)));
        chartModel->setItem(cnt, 4, new QStandardItem(p.description));
        chartModel->setItem(cnt, 5, new QStandardItem(QString::number((int)p.stock)));
        chartItem[cnt] = new QStandardItem();
        chartItem[cnt]->setCheckable(true);
        chartItem[cnt]->setCheckState(Qt::Unchecked);
        chartItem[cnt]->setEditable(false);
        chartModel->setItem(cnt, 6, chartItem[cnt]);
        cnt++;
    }
}

void MainWindow::setOrder() {
    orderModel->removeRows(0, orderModel->rowCount());
    for(int i=0;i<order.size;i++) {
        Product p = order.product[i];
        orderModel->setItem(i, 0, new QStandardItem(p.name));
        orderModel->setItem(i, 1, new QStandardItem(QString::number(p.getPrice() * p.discount, 'f', 2)));
        orderModel->setItem(i, 2, new QStandardItem(QString::number((int)p.stock)));
    }
    ui->pay->setText(QString::asprintf("%.2lf", pay));
}

// 0: send order fail; 1: send successful
void MainWindow::setOrderState(int flag) {
    if(flag) {
        setOrder();
        payState = 0;
        QMessageBox::information(NULL, "Opeartion", "Order generated. Please pay in \"我的\"");
    } else {
        QMessageBox::critical(NULL, "Error", "Order generated fialed.");
    }
}

// 0: error; 1: payed; 2: canceled
void MainWindow::setPayState(int flag, Customer customer) {
    if(flag == 1) { // pay successfully
        payState = 1;
        order = Chart();
        orderModel->removeRows(0, orderModel->rowCount());
        this->customer = customer;

        // ui
        ui->remaining->setText(QString::asprintf("%.2f", customer.GetBalance()));
        ui->pay->setText("");
        QMessageBox::information(NULL, "Opeartion", "Pay successfully.");
    } else if(flag == 2) {
        payState = 1;
        order = Chart();
        orderModel->removeRows(0, orderModel->rowCount());

        // ui
        ui->pay->setText("");
        QMessageBox::information(NULL, "Opeartion", "Order canceled successfully.");
    } else {
        QMessageBox::critical(NULL, "Error", "Pay fialed.");
    }
}


//------------------------------------------------------
// SLOT FUNCTION

void MainWindow::on_resetBackendButton_clicked()
{
    this->close();
    emit sigResetBackend();
}

void MainWindow::on_loginButton_clicked()
{
    try {
        if(loginState == UserType::Unknown){
            emit sigLoginWindow();
        } else { // logout
            if(!payState) throw std::logic_error("Order unpay.");
            loginState = UserType::Unknown;
            emit sigSearchProduct("");

            // ui
            ui->loginButton->setText("登录");
            ui->orderPageButton->setText("登陆后查看");
            ui->stackedWidget->setCurrentIndex(0);
            ui->number->hide();
            ui->addToChartButton->hide();
        }
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void MainWindow::on_modifypasswordButton_clicked()
{
    try {
        if(ui->newPassword->text().isEmpty()) throw std::logic_error("Password cannot be null");
        std::string newpassword = ui->newPassword->text().toStdString();
        if(loginState == UserType::Unknown) throw std::logic_error("Not login.");
        if(newpassword.length() > MAX_STRING_LENGTH) throw std::logic_error("Password too long.");
        int id, type;
        if(loginState == UserType::Customer) {
            id = customer.id;
            type = 0;
        } else {
            id = shop.id;
            type = 1;
        }
        emit sigModifyPassword(id, type, newpassword);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void MainWindow::on_chargeButton_clicked()
{
    try {
        if(ui->chargeAmount->text().isEmpty()) throw std::logic_error("Amount cannot be null.");
        if(loginState == UserType::Unknown) throw std::logic_error("Not login.");
        double amount = ui->chargeAmount->text().toDouble();
        int id, type;
        if(loginState == UserType::Customer) {
            id = customer.id;
            type = 0;
        } else {
            id = shop.id;
            type = 1;
        }
        emit sigRecharge(id, type, amount);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }

}

void MainWindow::on_myButton_clicked()
{
    if(loginState == UserType::Unknown) {
        QMessageBox::critical(NULL, "Error", "Not login.");
    } else {
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void MainWindow::on_mainPageButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->searchDescription->clear();
    emit sigSearchProduct("");
}

void MainWindow::on_orderPageButton_clicked()
{
    if(loginState == UserType::Unknown) {
        QMessageBox::critical(NULL, "Error", "Not login.");
    } else if(loginState == UserType::Customer){ // Customer
        ui->stackedWidget->setCurrentIndex(1);
        emit sigGetChart(customer.id);
    } else { // Shop
        ui->stackedWidget->setCurrentIndex(2);
        ui->discount->setText("1");
        emit sigSearchProduct(shop.id);
    }
}

void MainWindow::on_addProductButton_clicked()
{
    emit sigAddProductWindow(shop.id);
}

void MainWindow::on_searchButton_clicked()
{
    std::string description = ui->searchDescription->text().toStdString();
    emit sigSearchProduct(description);
}

void MainWindow::on_manageTableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    emit sigModifyWindow(manageList[row]);
}

void MainWindow::on_discountButton_clicked()
{
    try {
        if(ui->discount->text().isEmpty()) throw std::logic_error("Discount cannot be null.");
        double discount = ui->discount->text().toDouble();
        if(ui->selectedProductButton->isChecked()) {
            int curRow=ui->manageTableView->currentIndex().row();
            if(curRow < 0) throw std::logic_error("Please select a line.");
            emit sigSetDiscount(manageList[curRow].id, discount);
        } else {
            ProductType type;
            if(ui->allBookButton->isChecked()) {
                type = ProductType::Book;
            }
            else if(ui->allFoodButton->isChecked()) {
                type = ProductType::Food;
            }
            else {
                type = ProductType::Cloth;
            }
            emit sigSetDiscount(shop.id, type, discount);
        }
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }


}

void MainWindow::on_addToChartButton_clicked()
{
    try {
        if(ui->number->text().isEmpty()) throw std::logic_error("Product number cannot be null.");
        int row = ui->searchTableView->currentIndex().row();
        if(row < 0) throw std::logic_error("Please select a line.");
        int number = ui->number->text().toInt();
        if(number <= 0 || number >= 1000) throw std::logic_error("Please input number between 1 and 999.");
        if(customer.chart_num == MAX_CHART_SIZE) throw std::logic_error("Chart full.");
        searchList[row].print();
        emit sigAddToChart(customer.id, searchList[row].id, number);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void MainWindow::on_modifyChartButton_clicked()
{
    try {
        if(ui->modifyChartNum->text().isEmpty()) throw std::logic_error("Product number cannot be null.");
        int chartID = ui->chartTableView->currentIndex().row();
        if(chartID < 0) throw std::logic_error("Please select a line.");
        int num = ui->modifyChartNum->text().toInt();
        emit sigModifyChart(chart.customerID, chartID, num);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void MainWindow::on_orderButton_clicked()
{
    try {
        if(!payState) throw std::logic_error("Please handle old order first.");
        order = Chart();
        pay = 0;
        order.size = 0;
        order.customerID = customer.id;
        for(int i=0;i<chartModel->rowCount();i++)
            if(chartItem[i]->checkState() == Qt::Checked){
                pay += chart.product[i].getPrice() * chart.product[i].discount * chart.product[i].stock;
                order.product[order.size++] = chart.product[i];
                qDebug()<<i;
            }
        emit sigAddOrder(order);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void MainWindow::on_cancelButton_clicked()
{
    try {
        if(payState) throw std::logic_error("No order to cancel");
        emit sigFreeOrder(order);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void MainWindow::on_payButton_clicked()
{
    try {
        if(payState == 1) throw std::logic_error("No order to pay.");
        if(pay > customer.GetBalance()) throw std::logic_error("Balance insufficient.");
        emit sigPayOrder(order);
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}

void MainWindow::on_MainWindow_destroyed()
{
    if(!payState) {
        QMessageBox::information(NULL, "Suggestion", "Order abandoned.");
        emit sigFreeOrder(order);
    }
}



// ------------------------------------------
// DEBUG function
void MainWindow::on_pushButton_clicked()
{
    try {
        if(loginState == UserType::Unknown) throw std::logic_error("Not login.");
        if(loginState == UserType::Customer) customer.print();
        if(loginState == UserType::Shop) shop.print();
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    try {
        chart.Print();
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    try {
        order.Print();
    }  catch (std::exception &e) {
        QMessageBox::critical(NULL, "Error", QString(e.what()));
    }
}


void MainWindow::on_allProductButton_clicked()
{
    emit sigSearchProduct("");
}

