QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addproductwindow.cpp \
    ../public/chart.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    modifyproductwindow.cpp \
    ../public/product.cpp \
    ../public/user.cpp

HEADERS += \
    ../public/utils.h \
    addproductwindow.h \
    ../public/chart.h \
    loginwindow.h \
    mainwindow.h \
    modifyproductwindow.h \
    ../public/product.h \
    ../public/user.h

FORMS += \
    addproductwindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    modifyproductwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
