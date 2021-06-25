#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <iostream>
#include <fstream>
#include "product.h"
#include "user.h"

#define CHART_BLOCK_SIZE 508
#define MAX_CHART_SIZE 5
/*
data/chart.data ( CHART DATA BLOCK )

 0 (4 bytes) 4 (4 bytes) 8 (100 bytes) 108           208    408           508
 +--+--+--+--+--+--+--+--+-------------+-------------+------+-------------+
 |customer_id|    size   |  product_0  |  product_1  | ...  |  product_4  |
 +--+--+--+--+--+--+--+--+-------------+-------------+------+-------------+

 product: 100 bytes
                                 1  1  1  1  1  1
   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  (byte)
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |     id    | belong_id |num(stock) |    type   |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |           price       |        discount       |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |                     name                      |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 |  feature  |                 description
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
             |
 +--+--+--+--+

(total:  508 bytes)
*/
// CAUTION: if item_num[i]==0, we consider this item is deleted.

class Chart
{
public:
    int customerID;
    int size;
    Product product[MAX_CHART_SIZE];
    Chart(){
        size = 0;
        for(int i=0;i<MAX_CHART_SIZE;i++) product[i].stock = 0;
    };
    Chart(int id, int size){
        this->customerID = id;
        this->size = size;
        for(int i=0;i<MAX_CHART_SIZE;i++) product[i].stock = 0;
    };
    Chart(int index);

    void Write();
    void Print();

//    Chart(const Chart &chart);
//    Chart& operator=(const Chart& chart);

};

#endif // CHART_H
