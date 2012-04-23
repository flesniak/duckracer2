#include "dlgconfigure.h"

dlgConfigure::dlgConfigure(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Optionen");

    QGroupBox *grpConfSerial = new QGroupBox(tr("Serielle Schnittstelle"),this);

    QLabel *labelSerialPort = new QLabel(tr("Schnittstelle"),grpConfSerial);
    comboBoxSerialPort = new QComboBox(grpConfSerial);
    comboBoxSerialPort->setEditable(true);

    QLabel *labelBaudRate = new QLabel(tr("Baudrate"),grpConfSerial);
    comboBoxBaudRate = new QComboBox(grpConfSerial);
    QStringList baudrates;
    baudrates << "50" << "75" << "110" << "134" << "150" << "200" << "300" << "600" << "1200" << "1800" << "2400"
              << "4800" << "9600" << "19.2k" << "38.4k" << "57.6k" << "115.2k" << "230.4k" << "460.8k" << "500k"
              << "576k" << "921.6k" << "1000k" << "1152k" << "1500k" << "2000k" << "2500k" << "3000k"
              << "3500k" << "4000k";
    comboBoxBaudRate->addItems(baudrates);

    QPushButton *buttonReloadPorts = new QPushButton(tr("Aktualisieren"),grpConfSerial);

    QGridLayout *layoutGrpConfSerial = new QGridLayout(grpConfSerial);
    layoutGrpConfSerial->addWidget(labelSerialPort,0,0);
    layoutGrpConfSerial->addWidget(comboBoxSerialPort,0,1);
    layoutGrpConfSerial->addWidget(labelBaudRate,1,0);
    layoutGrpConfSerial->addWidget(comboBoxBaudRate,1,1);
    layoutGrpConfSerial->addWidget(buttonReloadPorts,2,1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal,this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(grpConfSerial,0,0);
    layout->addWidget(buttonBox,1,0);

    connect(buttonBox,SIGNAL(accepted()),SLOT(processOK()));
    connect(buttonBox,SIGNAL(rejected()),SLOT(reject()));
    connect(buttonReloadPorts,SIGNAL(clicked()),SLOT(collectSerialPorts()));

    loadConfig();
}

void dlgConfigure::processOK()
{
    saveConfig();
    accept();
}

void dlgConfigure::loadConfig()
{
    collectSerialPorts();
    QSettings settings;
    QString serialPortName = settings.value("serial/port").toString();
    int serialPortIndex = comboBoxSerialPort->findText(serialPortName);
    if( serialPortIndex >= 0 )
        comboBoxSerialPort->setCurrentIndex(serialPortIndex);
    else {
        comboBoxSerialPort->addItem(serialPortName);
        comboBoxSerialPort->setCurrentIndex(comboBoxSerialPort->count()-1);
    }
    int baudRateIndex = comboBoxBaudRate->findText(settings.value("serial/baudrate").toString());
    if( baudRateIndex >= 0 )
        comboBoxBaudRate->setCurrentIndex(baudRateIndex);
}

void dlgConfigure::saveConfig()
{
    QSettings settings;
    settings.setValue("serial/port",comboBoxSerialPort->currentText());
    settings.setValue("serial/baudrate",comboBoxBaudRate->currentText());
}

void dlgConfigure::collectSerialPorts()
{
    comboBoxSerialPort->clear();
    QDir dirDev("/dev");
    QStringList dirFilters;
    dirFilters << "tty?*";
    dirDev.setNameFilters(dirFilters);
    dirDev.setFilter(QDir::Readable | QDir::Files | QDir::Drives | QDir::System);
    comboBoxSerialPort->addItems(dirDev.entryList());
}
