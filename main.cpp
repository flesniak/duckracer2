#include <QtGui/QApplication>
#include "duckracer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    duckracer w;
    w.show();
    
    return a.exec();
}
