#include "duckracer.h"

duckracer::duckracer(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("DuckRacer2"));

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    tabWidget->setTabsClosable(true);

    QMenu *menuDuckracer = new QMenu("D&uckRacer",this);
    QAction *actionConfigure = menuDuckracer->addAction(QIcon::fromTheme("configure"),tr("O&ptionen"));
    QAction *actionQuit = menuDuckracer->addAction(QIcon::fromTheme("application-exit"),tr("B&eenden"));
    menuBar()->addMenu(menuDuckracer);

    connect(actionConfigure,SIGNAL(triggered()),SLOT(processConfigure()));
    connect(actionQuit,SIGNAL(triggered()),SLOT(processQuit()));
}

duckracer::~duckracer()
{

}

void duckracer::processConfigure()
{

}

void duckracer::processQuit()
{
    //TODO: Cleanup?
    close();
}
