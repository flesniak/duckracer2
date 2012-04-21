#ifndef DUCKRACER_H
#define DUCKRACER_H

#include <QtGui>

class wdgLabels;
class wdgPrizes;
class wdgScan;

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

private:
    QTabWidget *tabWidget;
    wdgLabels *widgetLabels;
    wdgPrizes *widgetPrizes;
    wdgScan *widgetScan;

private slots:
    void processConfigure();
    void processQuit();
    void processCloseTab(int index);
};

#endif // DUCKRACER_H
