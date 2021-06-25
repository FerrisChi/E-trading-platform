#include "backend.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Backend BN(&a);
    return a.exec();
}
