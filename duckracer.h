#ifndef DUCKRACER_H
#define DUCKRACER_H

#include <QtGui/QMainWindow>

class duckracer : public QMainWindow
{
    Q_OBJECT
    
public:
    duckracer(QWidget *parent = 0);
    ~duckracer();
};

#endif // DUCKRACER_H
