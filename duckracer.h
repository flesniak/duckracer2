#ifndef DUCKRACER_H
#define DUCKRACER_H

#include <QtGui>

class wdgPrizes;
class wdgScan;
class wdgPrintLabels;
class wdgPrintLists;

class duckracer : public QMainWindow
{
    Q_OBJECT
public:
    duckracer(QWidget *parent = 0);
    ~duckracer();

public slots:
    void openScan();
    void openManagePrizes();
    void openPrintLabels();
    void openPrintLists();
    QString getPrizeListFileName(bool save = false); //if save is true and no file is selected, we will prompt for one
    QString getScanFileName();

private:
    QTabWidget *tabWidget;
    QLabel *labelPrizeListFileName;
    QLabel *labelScanFileName;
    QAction *actionCloseScanFile;
    QAction *actionClosePrizeFile;
    wdgPrizes *widgetPrizes;
    wdgScan *widgetScan;
    wdgPrintLabels *widgetPrintLabels;
    wdgPrintLists *widgetPrintLists;

    QString prizeListFileName;
    QString scanFileName;

private slots:
    void processConfigure();
    void processQuit();
    void processCloseTab(int index);
    void processOpenPrizeFile(); //prompt for open, not save, by default
    void processOpenScanFile(bool save = false); //prompt for open, not save, by default
    void processClosePrizeFile();
    void processCloseScanFile();
    void checkPrizeListFileName();
    void checkScanFileName();
    void updateFileNameLabels();
};

#endif // DUCKRACER_H
