#include "duckracer.h"
#include "dlgconfigure.h"
#include "wdglabels.h"
#include "wdgprizes.h"
#include "wdgscan.h"

const unsigned char homeButtonHeight = 50;
const unsigned char homeButtonMargin = 25;

duckracer::duckracer(QWidget *parent) : QMainWindow(parent), widgetLabels(0), widgetPrizes(0), widgetScan(0)
{
    setWindowTitle(tr("DuckRacer2"));

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    tabWidget->setTabsClosable(true);

    QWidget *widgetHome = new QWidget(tabWidget);
    widgetHome->setContentsMargins(homeButtonMargin,homeButtonMargin,homeButtonMargin,homeButtonMargin);
    QLabel *labelHome = new QLabel(tr("Willkommen bei DuckRacer 2"),widgetHome);
    QFont homeFont(labelHome->font());
    homeFont.setPointSize(24);
    homeFont.setBold(true);
    labelHome->setFont(homeFont);
    labelHome->setAlignment(Qt::AlignCenter);
    QPushButton *buttonScan = new QPushButton(tr("Enten scannen"),widgetHome);
    QPushButton *buttonManagePrizes = new QPushButton(tr("Preisliste verwalten"),widgetHome);
    QPushButton *buttonPrintLabels = new QPushButton(tr("Entenetiketten drucken"),widgetHome);
    QPushButton *buttonOptions = new QPushButton(tr("DuckRacer konfigurieren"),widgetHome);
    buttonScan->setMinimumHeight(40);
    buttonManagePrizes->setMinimumHeight(40);
    buttonPrintLabels->setMinimumHeight(40);
    buttonOptions->setMinimumHeight(40);

    QGridLayout *layoutHome = new QGridLayout(widgetHome);
    layoutHome->addWidget(labelHome,0,0,1,2);
    layoutHome->addWidget(buttonScan,1,0);
    layoutHome->addWidget(buttonManagePrizes,1,1);
    layoutHome->addWidget(buttonPrintLabels,2,0);
    layoutHome->addWidget(buttonOptions,2,1);
    layoutHome->setSpacing(homeButtonMargin);

    tabWidget->addTab(widgetHome,tr(QString::fromUtf8("Hauptmenü").toLatin1()));
    QWidget *buttonCloseHomeTab = tabWidget->findChild<QTabBar *>()->tabButton(0,QTabBar::RightSide);
    buttonCloseHomeTab->resize(0,0);
    buttonCloseHomeTab->hide();

    QMenu *menuDuckracer = new QMenu("D&uckRacer",this);
    QAction *actionConfigure = menuDuckracer->addAction(QIcon::fromTheme("configure"),tr("O&ptionen"));
    QAction *actionQuit = menuDuckracer->addAction(QIcon::fromTheme("application-exit"),tr("B&eenden"));
    menuBar()->addMenu(menuDuckracer);

    QMenu *menuActions = new QMenu("A&ktion",this);
    QAction *actionScan = menuActions->addAction(tr("Enten scannen"));
    QAction *actionManagePrizes = menuActions->addAction(tr("Preisliste verwalten"));
    QAction *actionPrintLabels = menuActions->addAction(tr("Entenetiketten drucken"));
    menuBar()->addMenu(menuActions);

    connect(buttonScan,SIGNAL(clicked()),actionScan,SLOT(trigger()));
    connect(buttonManagePrizes,SIGNAL(clicked()),actionManagePrizes,SLOT(trigger()));
    connect(buttonPrintLabels,SIGNAL(clicked()),actionPrintLabels,SLOT(trigger()));
    connect(buttonOptions,SIGNAL(clicked()),actionConfigure,SLOT(trigger()));

    connect(actionConfigure,SIGNAL(triggered()),SLOT(processConfigure()));
    connect(actionManagePrizes,SIGNAL(triggered()),SLOT(openManagePrizes()));
    connect(actionPrintLabels,SIGNAL(triggered()),SLOT(openPrintLabels()));
    connect(actionQuit,SIGNAL(triggered()),SLOT(processQuit()));
    connect(actionScan,SIGNAL(triggered()),SLOT(openScan()));

    connect(tabWidget,SIGNAL(tabCloseRequested(int)),SLOT(processCloseTab(int)));
}

duckracer::~duckracer()
{
    //TODO: Delete non-Qt classes?
}

void duckracer::processConfigure()
{
    dlgConfigure *configure = new dlgConfigure(this);
    if( configure->exec() == QDialog::Accepted ) {
        //Make tabs reload the configuration
        ;
    }
}

void duckracer::processQuit()
{
    //TODO: Cleanup?
    close();
}

void duckracer::processCloseTab(int index)
{
    if( index == 0 )
        return;
    QWidget *closeWidget = tabWidget->widget(index);
    tabWidget->removeTab(index);
    if( closeWidget == widgetLabels )
        widgetLabels = 0;
    if( closeWidget == widgetPrizes )
        widgetPrizes = 0;
    if( closeWidget == widgetScan )
        widgetScan = 0;
    delete closeWidget;
}

void duckracer::openScan()
{
    if( widgetScan == 0 ) {
        widgetScan = new wdgScan(tabWidget);
        tabWidget->addTab(widgetScan,tr("Enten scannen"));
        tabWidget->setCurrentWidget(widgetScan);
    }
    else
        tabWidget->setCurrentWidget(widgetScan);
}

void duckracer::openManagePrizes()
{
    if( widgetPrizes == 0 ) {
        widgetPrizes = new wdgPrizes(widgetPrizes);
        tabWidget->addTab(widgetPrizes,tr("Preisliste verwalten"));
        tabWidget->setCurrentWidget(widgetPrizes);
    }
    else
        tabWidget->setCurrentWidget(widgetPrizes);
}

void duckracer::openPrintLabels()
{
    if( widgetLabels == 0 ) {
        widgetLabels = new wdgLabels(tabWidget);
        tabWidget->addTab(widgetLabels,tr("Etiketten drucken"));
        tabWidget->setCurrentWidget(widgetLabels);
    }
    else
        tabWidget->setCurrentWidget(widgetLabels);
}
