#ifndef DUCKRACER_H
#define DUCKRACER_H

#include <QtGui>

class duckracer : public QMainWindow
{
    Q_OBJECT
public:
    duckracer(QWidget *parent = 0);
    ~duckracer();

private:
    QTabWidget *tabWidget;

private slots:
    void processConfigure();
    void processQuit();
};

#endif // DUCKRACER_H
