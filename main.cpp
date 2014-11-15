#include "serialexample.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialExample w;
    w.show();

    return a.exec();
}
