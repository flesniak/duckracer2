#include "duckracer.h"
#include "dlgconfigure.h"
#include "wdgprizes.h"
#include "wdgscan.h"
#include "wdgprintlabels.h"
#include "wdgprintlists.h"

static const char* version = "2.0beta";
const unsigned char homeButtonHeight = 50;
const unsigned char homeButtonMargin = 25;

duckracer::duckracer(QWidget *parent) : QMainWindow(parent), widgetPrizes(0), widgetScan(0), widgetPrintLabels(0), widgetPrintLists(0)
{
    setWindowTitle(trUtf8("DuckRacer %1").arg(version));

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    tabWidget->setTabsClosable(true);

    QWidget *widgetHome = new QWidget(tabWidget);
    widgetHome->setContentsMargins(homeButtonMargin,homeButtonMargin,homeButtonMargin,homeButtonMargin);
    QLabel *labelHome = new QLabel(trUtf8("Willkommen bei DuckRacer %1").arg(version),widgetHome);
    QFont homeFont(labelHome->font());
    homeFont.setPointSize(24);
    homeFont.setBold(true);
    labelHome->setFont(homeFont);
    labelHome->setAlignment(Qt::AlignCenter);
    QPushButton *buttonScan = new QPushButton(QIcon::fromTheme("scanner"),trUtf8("Enten scannen"),widgetHome);
    QPushButton *buttonManagePrize = new QPushButton(QIcon::fromTheme("scribus"),trUtf8("Preisliste verwalten"),widgetHome);
    QPushButton *buttonPrintLabels = new QPushButton(QIcon::fromTheme("preferences-web-browser-identification"),trUtf8("Entenetiketten drucken"),widgetHome);
    QPushButton *buttonPrintLists = new QPushButton(QIcon::fromTheme("printer"),trUtf8("Listen drucken"),widgetHome);
    buttonScan->setMinimumHeight(40);
    buttonManagePrize->setMinimumHeight(40);
    buttonPrintLabels->setMinimumHeight(40);
    buttonPrintLists->setMinimumHeight(40);
    labelPrizeListFileName = new QLabel(widgetHome);
    labelScanFileName = new QLabel(widgetHome);

    QVBoxLayout *layoutLabels = new QVBoxLayout;
    layoutLabels->addWidget(labelPrizeListFileName);
    layoutLabels->addWidget(labelScanFileName);
    layoutLabels->setSpacing(5);

    QGridLayout *layoutHome = new QGridLayout(widgetHome);
    layoutHome->addWidget(labelHome,0,0,1,2);
    layoutHome->addWidget(buttonScan,1,0);
    layoutHome->addWidget(buttonManagePrize,1,1);
    layoutHome->addWidget(buttonPrintLabels,2,0);
    layoutHome->addWidget(buttonPrintLists,2,1);
    layoutHome->addLayout(layoutLabels,3,0,1,2);
    layoutHome->setSpacing(homeButtonMargin);

    tabWidget->addTab(widgetHome,trUtf8("Hauptmenü"));
    QWidget *buttonCloseHomeTab = tabWidget->findChild<QTabBar *>()->tabButton(0,QTabBar::RightSide);
    buttonCloseHomeTab->resize(0,0);
    buttonCloseHomeTab->hide();

    QMenu *menuDuckracer = new QMenu("D&uckRacer",this);
    QAction *actionOpenScanFile = menuDuckracer->addAction(QIcon::fromTheme("document-open"),trUtf8("Scan-Datei öffnen"));
    QAction *actionOpenPrizeFile = menuDuckracer->addAction(QIcon::fromTheme("document-open"),trUtf8("Preisliste öffnen"));
    actionCloseScanFile = menuDuckracer->addAction(QIcon::fromTheme("document-close"),trUtf8("Scan-Datei schließen"));
    actionClosePrizeFile = menuDuckracer->addAction(QIcon::fromTheme("document-close"),trUtf8("Preisliste schließen"));
    QAction *actionConfigure = menuDuckracer->addAction(QIcon::fromTheme("configure"),trUtf8("O&ptionen"));
    QAction *actionQuit = menuDuckracer->addAction(QIcon::fromTheme("application-exit"),trUtf8("B&eenden"));
    menuBar()->addMenu(menuDuckracer);

    QMenu *menuActions = new QMenu("A&ktion",this);
    QAction *actionScan = menuActions->addAction(trUtf8("Enten scannen"));
    QAction *actionManagePrize = menuActions->addAction(trUtf8("Preisliste verwalten"));
    QAction *actionPrintLabels = menuActions->addAction(trUtf8("Entenetiketten drucken"));
    QAction *actionPrintLists = menuActions->addAction(trUtf8("Listen drucken"));
    menuBar()->addMenu(menuActions);

    connect(buttonScan,SIGNAL(clicked()),actionScan,SLOT(trigger()));
    connect(buttonManagePrize,SIGNAL(clicked()),actionManagePrize,SLOT(trigger()));
    connect(buttonPrintLabels,SIGNAL(clicked()),actionPrintLabels,SLOT(trigger()));
    connect(buttonPrintLists,SIGNAL(clicked()),actionPrintLists,SLOT(trigger()));

    connect(actionScan,SIGNAL(triggered()),SLOT(openScan()));
    connect(actionManagePrize,SIGNAL(triggered()),SLOT(openManagePrizes()));
    connect(actionPrintLabels,SIGNAL(triggered()),SLOT(openPrintLabels()));
    connect(actionPrintLists,SIGNAL(triggered()),SLOT(openPrintLists()));

    connect(actionOpenScanFile,SIGNAL(triggered()),SLOT(processOpenScanFile()));
    connect(actionOpenPrizeFile,SIGNAL(triggered()),SLOT(processOpenPrizeFile()));
    connect(actionCloseScanFile,SIGNAL(triggered()),SLOT(processCloseScanFile()));
    connect(actionClosePrizeFile,SIGNAL(triggered()),SLOT(processClosePrizeFile()));
    connect(actionConfigure,SIGNAL(triggered()),SLOT(processConfigure()));
    connect(actionQuit,SIGNAL(triggered()),SLOT(processQuit()));

    connect(tabWidget,SIGNAL(tabCloseRequested(int)),SLOT(processCloseTab(int)));

    QSettings settings;
    prizeListFileName = settings.value("duckracer/prizeFileName").toString();
    if( !QFile(prizeListFileName).exists() )
        prizeListFileName.clear();
    scanFileName = settings.value("duckracer/scanFileName").toString();
    if( !QFile(scanFileName).exists() )
        scanFileName.clear();
    updateFileNameLabels();
}

duckracer::~duckracer()
{
    //TODO: Delete non-Qt classes?
}

void duckracer::closeEvent(QCloseEvent *event)
{
    if( widgetScan != 0 && !widgetScan->promptSaveChanges() )
        return event->ignore();
    if( widgetPrizes != 0 && !widgetPrizes->promptSaveChanges() )
        return event->ignore();
    event->accept();
}

void duckracer::processConfigure()
{
    dlgConfigure *configure = new dlgConfigure(this);
    if( configure->exec() == QDialog::Accepted ) {
        //Make tabs reload the configuration
        if( widgetPrintLabels != 0 )
            widgetPrintLabels->reloadPrintParameters();
        if( widgetScan != 0 )
            widgetScan->reloadConfiguration();
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
    if( closeWidget == widgetPrintLabels )
        widgetPrintLabels = 0;
    if( closeWidget == widgetPrizes ) {
        if(!widgetPrizes->close())
            return;
        widgetPrizes = 0;
    }
    if( closeWidget == widgetScan ) {
        if(!widgetScan->close())
            return;
        widgetScan = 0;
    }
    if( closeWidget == widgetPrintLists )
        widgetPrintLists = 0;
    tabWidget->removeTab(index);
    delete closeWidget;
}

void duckracer::openScan()
{
    if( widgetScan == 0 ) {
        widgetScan = new wdgScan(tabWidget);
        tabWidget->addTab(widgetScan,trUtf8("Enten scannen"));
        tabWidget->setCurrentWidget(widgetScan);
        connect(widgetScan,SIGNAL(scanFileNameChanged()),SLOT(checkScanFileName()));
        widgetScan->updateScanFileName(scanFileName);
    }
    else
        tabWidget->setCurrentWidget(widgetScan);
}

void duckracer::openManagePrizes()
{
    if( widgetPrizes == 0 ) {
        widgetPrizes = new wdgPrizes(tabWidget);
        tabWidget->addTab(widgetPrizes,trUtf8("Preisliste verwalten"));
        tabWidget->setCurrentWidget(widgetPrizes);
        connect(widgetPrizes,SIGNAL(prizeListFileNameChanged()),SLOT(checkPrizeListFileName()));
        widgetPrizes->updatePrizeListFileName(prizeListFileName);
    }
    else
        tabWidget->setCurrentWidget(widgetPrizes);
}

void duckracer::openPrintLabels()
{
    if( widgetPrintLabels == 0 ) {
        widgetPrintLabels = new wdgPrintLabels(tabWidget);
        tabWidget->addTab(widgetPrintLabels,trUtf8("Etiketten drucken"));
        tabWidget->setCurrentWidget(widgetPrintLabels);
    }
    else
        tabWidget->setCurrentWidget(widgetPrintLabels);
}

void duckracer::openPrintLists()
{
    if( widgetPrintLists == 0 ) {
        widgetPrintLists = new wdgPrintLists(tabWidget);
        tabWidget->addTab(widgetPrintLists,trUtf8("Listen drucken"));
        tabWidget->setCurrentWidget(widgetPrintLists);
        widgetPrintLists->updateFileNames(prizeListFileName,scanFileName);
    }
    else
        tabWidget->setCurrentWidget(widgetPrintLists);
}

void duckracer::processOpenPrizeFile()
{
    QSettings settings;
    QString newFileName;
    newFileName = QFileDialog::getOpenFileName(this,trUtf8("Preisliste öffnen"),settings.value("duckracer/lastdirectory",QDir::homePath()).toString(),tr("Preislisten (*.prz);;Textdateien (*.txt)"));
    if( !newFileName.isEmpty() && newFileName != prizeListFileName && (widgetPrizes == 0 || widgetPrizes->updatePrizeListFileName(newFileName)) )
        checkPrizeListFileName(newFileName);
}

void duckracer::processOpenScanFile()
{
    QSettings settings;
    QString newFileName;
    newFileName = QFileDialog::getOpenFileName(this,trUtf8("Scan-Datei öffnen"),settings.value("duckracer/lastdirectory",QDir::homePath()).toString(),tr("Scan-Dateien (*.dsc);;Textdateien (*.txt)"));
    if( !newFileName.isEmpty() && newFileName != scanFileName && (widgetScan == 0 || widgetScan->updateScanFileName(newFileName)) )
        checkScanFileName(newFileName);
}

void duckracer::processClosePrizeFile()
{
    if( widgetPrizes == 0 || widgetPrizes->updatePrizeListFileName(QString()) ) { //Only update filename if it was changed successfully
        prizeListFileName.clear();
        checkPrizeListFileName();
    }
}

void duckracer::processCloseScanFile()
{
    if( widgetScan == 0 || widgetScan->updateScanFileName(QString()) ) { //Only update filename if it was changed successfully
        scanFileName.clear();
        checkScanFileName();
    }
}

void duckracer::updateFileNameLabels()
{
    if( prizeListFileName.isEmpty() ) {
        labelPrizeListFileName->setText(trUtf8("Preisliste: Keine Preisliste geöffnet."));
        actionClosePrizeFile->setEnabled(false);
    }
    else {
        labelPrizeListFileName->setText(trUtf8("Preisliste: %1").arg(prizeListFileName));
        actionClosePrizeFile->setEnabled(true);
    }
    if( scanFileName.isEmpty() ) {
        labelScanFileName->setText(trUtf8("Scan-Datei: Keine Scan-Datei geöffnet."));
        actionCloseScanFile->setEnabled(false);
    }
    else {
        labelScanFileName->setText(trUtf8("Scan-Datei: %1").arg(scanFileName));
        actionCloseScanFile->setEnabled(true);
    }
    if( widgetPrintLists != 0 )
        widgetPrintLists->updateFileNames(prizeListFileName,scanFileName);
}

QString duckracer::getPrizeListFileName()
{
    if( prizeListFileName.isEmpty() )
        processOpenPrizeFile();
    return prizeListFileName;
}

QString duckracer::getScanFileName()
{
    if( scanFileName.isEmpty() )
        processOpenScanFile();
    return scanFileName;
}

void duckracer::checkPrizeListFileName(const QString &newFileName)
{
    QSettings settings;
    if( widgetPrizes != 0 ) {
        prizeListFileName = widgetPrizes->currentFileName(); //This sets prizeListFileName to the one widgetPrizes is using, thus setting it to an possibly changed filename
        settings.setValue("duckracer/lastdirectory",QFileInfo(prizeListFileName).path());
    }
    else
        prizeListFileName = newFileName;
    settings.setValue("duckracer/prizefilename",prizeListFileName);
    updateFileNameLabels();
}

void duckracer::checkScanFileName(const QString &newFileName)
{
    QSettings settings;
    if( widgetScan != 0 ) {
        scanFileName = widgetScan->currentFileName(); //This sets scanFileName to the one widgetPrizes is using, thus setting it to an possibly changed filename
        settings.setValue("duckracer/lastdirectory",QFileInfo(scanFileName).path());
    }
    else
        scanFileName = newFileName;
    settings.setValue("duckracer/scanfilename",scanFileName);
    updateFileNameLabels();
}
