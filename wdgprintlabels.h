#ifndef WDGLABELS_H
#define WDGLABELS_H

#include <QtGui>

class wdgPrintLabels : public QWidget
{
    Q_OBJECT
public:
    explicit wdgPrintLabels(QWidget *parent = 0);

private:
    QSpinBox *spinBoxFirstLabel;
    QSpinBox *spinBoxLastLabel;
    QComboBox *comboBoxPreselection;
    QPushButton *buttonPrint;
    bool block;

private slots:
    void spinBoxValuesChanged();
    void comboBoxIndexChanged(QString str);
    void processPrint();
};

#endif // WDGLABELS_H
