#include "chart.h"

// read from disk
Chart::Chart(int index) {
    std::ifstream ifs("data/chart.data",std::ifstream::in|std::ifstream::binary);
    int startPos = CHART_BLOCK_SIZE * index, productID, num;
    ifs.seekg(startPos, ifs.beg);
    ifs.read((char *)&this->customerID, sizeof(int));
    ifs.read((char*)&this->size, sizeof(int));
    for(int i = 0; i< MAX_CHART_SIZE;i++) {
        ifs.seekg(startPos + PRODUCT_BLOCK_SIZE * i + 16, ifs.beg);
        ifs.read((char *)&num, sizeof(int));
        if(!num) {
            product[i].stock = 0;
            continue;
        }
        ifs.seekg(startPos + PRODUCT_BLOCK_SIZE * i + 8, ifs.beg);
        ifs.read((char *)&productID, sizeof(int));
        product[i] = Product(productID);
        product[i].stock = num;
    }
    ifs.close();
}

// write into "data/chart.data" [this->id]
void Chart::Write() {
    std::fstream fs("data/chart.data", std::fstream::in|std::fstream::out|std::fstream::binary);
    int startPos = CHART_BLOCK_SIZE * customerID;
    fs.seekp(startPos, fs.beg);
    fs.write((char *)&customerID, sizeof(int));
    fs.write((char *)&size, sizeof(int));
    for(int i = 0;i < MAX_CHART_SIZE; i++) {
        if(product[i].stock == 0) { // only write stock
            fs.seekp(startPos + 16 + PRODUCT_BLOCK_SIZE * i, fs.beg);
            fs.write((char *)&product[i].stock, sizeof(int));
            continue;
        }
        // else write all
        fs.seekp(startPos + 8 + PRODUCT_BLOCK_SIZE * i, fs.beg);
        fs.write((char *)&product[i].id, sizeof(int));
        fs.write((char *)&product[i].belong_id, sizeof(int));
        fs.write((char *)&product[i].stock, sizeof(int));
        int type = (int)product[i].type;
        fs.write((char *)&type, sizeof(int));
        fs.write((char *)&product[i].price, sizeof(double));
        fs.write((char *)&product[i].discount, sizeof(double));
        fs.write((char *)&product[i].name, MAX_NAME_SIZE + 1);
        fs.write((char *)&product[i].feature, sizeof(int));
        fs.write((char *)&product[i].description, MAX_DESC_SIZE + 1);
    }
    fs.close();
}

void Chart::Print() {
    qDebug()<<"Chart: "<<customerID<<size;
    for(int i=0;i<MAX_CHART_SIZE;i++)
        if(product[i].stock){
            product[i].print();
        }
}
