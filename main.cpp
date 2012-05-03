#include <QtGui/QApplication>
#include "duckracer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("DuckRacer2");
    app.setOrganizationName("Round Table 76");

    //This should translate our openFileDialog, but doesn't?
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    duckracer w;
    w.show();
    
    return app.exec();
}
