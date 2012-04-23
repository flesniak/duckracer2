#include <QtGui/QApplication>
#include "duckracer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Should translate our openFileDialog, but doesn't?
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    duckracer w;
    w.show();
    
    return app.exec();
}
