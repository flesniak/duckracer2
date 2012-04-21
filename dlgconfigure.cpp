#include "dlgconfigure.h"

dlgConfigure::dlgConfigure(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Optionen");

    QGroupBox *grpConfSerial = new QGroupBox(tr("Serielle Schnittstelle"),this);
    QLabel *labelSerialPort = new QLabel(tr("Schnittstelle"),grpConfSerial);
    comboBoxSerialPort = new QComboBox(grpConfSerial);
    comboBoxSerialPort->setEditable(true);
    QPushButton *buttonReloadPorts = new QPushButton(tr("Aktualisieren"),grpConfSerial);

    QGridLayout *layoutGrpConfSerial = new QGridLayout(grpConfSerial);
    layoutGrpConfSerial->addWidget(labelSerialPort,0,0);
    layoutGrpConfSerial->addWidget(comboBoxSerialPort,0,1);
    layoutGrpConfSerial->addWidget(buttonReloadPorts,1,1);

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
}

void dlgConfigure::saveConfig()
{
    QSettings settings;
    settings.setValue("serial/port",comboBoxSerialPort->currentText());
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
