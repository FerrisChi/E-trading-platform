#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowCloseButtonHint);

    connect(this, SIGNAL(sigLoginWindow()), &this->loginWindow, SLOT(show()));
    connect(this, SIGNAL(sigAddProductWindow()), &this->addProductWindow, SLOT(show()));
    connect(this, &MainWindow::sigModifyWindow, &this->modifyProductWindow, &ModifyProductWindow::init);


    ui->chargeAmount->setValidator(new QDoubleValidator(0,100000,2));
    ui->discount->setValidator(new QDoubleValidator(0, 1, 2));


    ui->mainPage->show();
    login_state = 0;



    // mainPage
    searchModel = new QStandardItemModel;
    this->ui->searchTableView->setModel(searchModel);
    ui->searchTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->searchTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->searchTableView->setSelectionMode( QAbstractItemView::SingleSelection);

    this->searchModel->setHorizontalHeaderItem(0, new QStandardItem("商品名") );
    this->searchModel->setHorizontalHeaderItem(1, new QStandardItem("商品类型"));
    this->searchModel->setHorizontalHeaderItem(2, new QStandardItem("现价"));
    this->searchModel->setHorizontalHeaderItem(3, new QStandardItem("库存"));
    this->searchModel->setHorizontalHeaderItem(4, new QStandardItem("商家"));
    this->searchModel->setHorizontalHeaderItem(5, new QStandardItem("商品描述"));

    this->ui->searchTableView->setColumnWidth(0, 100);    //width
    this->ui->searchTableView->setColumnWidth(1, 50);
    this->ui->searchTableView->setColumnWidth(2, 100);
    this->ui->searchTableView->setColumnWidth(3, 100);
    this->ui->searchTableView->setColumnWidth(4, 50);
    this->ui->searchTableView->setColumnWidth(5, 300);

    // orderPage
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

    this->ui->manageTableView->setColumnWidth(0, 150);    //width
    this->ui->manageTableView->setColumnWidth(1, 50);
    this->ui->manageTableView->setColumnWidth(2, 100);
    this->ui->manageTableView->setColumnWidth(3, 100);
    this->ui->manageTableView->setColumnWidth(4, 100);
    this->ui->manageTableView->setColumnWidth(5, 350);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {
    this->show();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::setLogin(User* usr) {
    ui->loginButton->setText("登出");

    ui->stackedWidget->setCurrentIndex(3);
    setRemaining(usr->GetBalance());
    setNickname(QString(usr->nickname));
    if(usr->type == UserType::Customer) {
        login_state = 1;
        ui->orderPageButton->setText("购物车");
    } else {
        login_state = 2;
        ui->orderPageButton->setText("商品管理");
    }
}

void MainWindow::setLogout() {
    ui->loginButton->setText("登录");
    login_state = 0;
    ui->orderPageButton->setText("登陆后查看");
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::setRemaining(double amount) {
    QString s = QString::asprintf("%.2f",amount);
    ui->remaining->setText(s);
}

void MainWindow::setNickname(QString Qstr) {
    ui->nickname->setText(Qstr);
}

void MainWindow::setSerchList(QVector<Product> productList) {
    searchModel->removeRows(0, searchModel->rowCount());
    searchList = productList;
    Product p;
    for(int i=0;i<productList.size();i++) {
        p = productList[i];
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
    manageList = productList;
    Product p;
    for(int i=0;i<productList.size();i++) {
        p = productList[i];
        manageModel->setItem(i, 0, new QStandardItem(p.name));
        manageModel->setItem(i, 1, new QStandardItem(QString::fromStdString(ProductTypeStr[(int)p.type])));
        manageModel->setItem(i, 2, new QStandardItem(QString::number(p.price, 'f', 2)));
        manageModel->setItem(i, 3, new QStandardItem(QString::number((int)p.stock)));
        manageModel->setItem(i, 4, new QStandardItem(QString::number(p.discount, 'f', 2)));
        manageModel->setItem(i, 5, new QStandardItem(p.description));
    }
}

//------------------------------------------------------
// SLOT FUNCTION

void MainWindow::on_resetBackendButton_clicked()
{
    emit sigResetBackend();
}

void MainWindow::on_loginButton_clicked()
{
    if(!login_state){
        emit sigLoginWindow();
    } else {
        emit sigLogout();
    }
}

void MainWindow::on_modifypasswordButton_clicked()
{
    std::string newpassword = ui->newPassword->text().toStdString();
    if(newpassword.length() > MAX_STRING_LENGTH){
        QMessageBox::critical(NULL, "Error", "Password too long.");
    } else {
        emit sigModifyPassword(newpassword);
    }

}

void MainWindow::on_chargeButton_clicked()
{
    double amount = ui->chargeAmount->text().toDouble();
    emit sigRecharge(amount);
}

void MainWindow::on_myButton_clicked()
{
    if(!login_state) {
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
    if(!login_state) {
        QMessageBox::critical(NULL, "Error", "Not login.");
    } else if(login_state == 1){ // Customer
        ui->stackedWidget->setCurrentIndex(1);
    } else { // Shop
        ui->stackedWidget->setCurrentIndex(2);
        ui->discount->setText("1");
        emit sigSearchProduct();
    }
}


void MainWindow::on_addProductButton_clicked()
{
    emit sigAddProductWindow();
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
    double discount = ui->discount->text().toDouble();
    if(ui->selectedProductButton->isChecked()) {
        int curRow=ui->manageTableView->currentIndex().row();
        emit sigSetDiscount(manageList[curRow].getID(), discount);
    } else {
        ProductType type;
        if(ui->allBookButton->isChecked()) type = ProductType::Book;
        else if(ui->allFoodButton->isChecked()) type = ProductType::Food;
        else {
            type = ProductType::Cloth;
        }
        emit sigSetDiscount(type, discount);
    }
}
