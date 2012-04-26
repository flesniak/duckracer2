#ifndef WDGLABELS_H
#define WDGLABELS_H

#include <QtGui>

class wdgPrintLabels : public QWidget
{
    Q_OBJECT
public:
    explicit wdgPrintLabels(QWidget *parent = 0);

public slots:
    void reloadPrintParameters();

private:
    QSpinBox *spinBoxFirstLabel;
    QSpinBox *spinBoxLastLabel;
    QComboBox *comboBoxPreselection;
    QSpinBox *spinBoxYear;
    QPushButton *buttonPrint;
    QLabel *labelBarcodeFileName;
    QLabel *labelBarcodeContFileName;
    QLabel *labelEncryption;
    QComboBox *comboBoxLp;
    bool block;

    struct {
        QString barcodeFileName;
        QString barcodeContFileName;
        int barcodeEncryption;
        unsigned short barcodeCipher;
    } printparameters;

private slots:
    void spinBoxValuesChanged();
    void comboBoxIndexChanged(QString str);
    void processPrint();
    void updatePrintButton();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // WDGLABELS_H
