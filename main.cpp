#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "list.h"

/*void visit(int &e)
{
    std::cout << e << std::endl;
}*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
