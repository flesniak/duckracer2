#ifndef DLGCONFIGURE_H
#define DLGCONFIGURE_H

#include <QtGui>

class dlgConfigure : public QDialog
{
    Q_OBJECT
public:
    explicit dlgConfigure(QWidget *parent = 0);

private:
    QComboBox *comboBoxSerialPort;
    QComboBox *comboBoxBaudRate;
    QLineEdit *lineEditBarcodePath;
    QLineEdit *lineEditBarcodeContPath;
    QSpinBox *spinBoxLabelSet;
    QComboBox *comboBoxEncryption;
    QSpinBox *spinBoxCypher;

    void loadConfig();
    void saveConfig();

private slots:
    void processOK();
    void collectSerialPorts();
    void updateContPath();
    void processSelectBarcodePath();
    void processSelectBarcodeContPath();
    void updateCypherField(int index);
};

#endif // DLGCONFIGURE_H
