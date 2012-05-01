#include "dlgconfigure.h"
#include "scanner.h"

dlgConfigure::dlgConfigure(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Optionen");

    QGroupBox *grpConfSerial = new QGroupBox(tr("Serielle Schnittstelle"),this);

    QLabel *labelSerialPort = new QLabel(tr("Schnittstelle"),grpConfSerial);
    comboBoxSerialPort = new QComboBox(grpConfSerial);
    comboBoxSerialPort->setEditable(true);

    QLabel *labelBaudRate = new QLabel(tr("Baudrate"),grpConfSerial);
    comboBoxBaudRate = new QComboBox(grpConfSerial);
    /*QStringList baudrates;
    baudrates << "50" << "75" << "110" << "134" << "150" << "200" << "300" << "600" << "1200" << "1800" << "2400"
              << "4800" << "9600" << "19.2k" << "38.4k" << "57.6k" << "115.2k" << "230.4k" << "460.8k" << "500k"
              << "576k" << "921.6k" << "1000k" << "1152k" << "1500k" << "2000k" << "2500k" << "3000k"
              << "3500k" << "4000k";*/
    comboBoxBaudRate->addItems(baudrates);

    QPushButton *buttonReloadPorts = new QPushButton(tr("Ports aktualisieren"),grpConfSerial);

    QGridLayout *layoutGrpConfSerial = new QGridLayout(grpConfSerial);
    layoutGrpConfSerial->addWidget(labelSerialPort,0,0);
    layoutGrpConfSerial->addWidget(comboBoxSerialPort,0,1);
    layoutGrpConfSerial->addWidget(labelBaudRate,1,0);
    layoutGrpConfSerial->addWidget(comboBoxBaudRate,1,1);
    layoutGrpConfSerial->addWidget(buttonReloadPorts,2,0,1,2);

    QGroupBox *grpConfBarcode = new QGroupBox(tr("Drucker-Steuerdateien"),this);
    QLabel *labelBarcode = new QLabel(trUtf8("Pfad zur Barcode-Datei"),grpConfBarcode);
    QLabel *labelBarcodeCont = new QLabel(trUtf8("Pfad zur Barcode-Continue-Datei"),grpConfBarcode);
    QPushButton *buttonBarcodePath = new QPushButton(QIcon::fromTheme("folder"),QString(),grpConfBarcode);
    QPushButton *buttonBarcodeContPath = new QPushButton(QIcon::fromTheme("folder"),QString(),grpConfBarcode);
    lineEditBarcodePath = new QLineEdit(grpConfBarcode);
    lineEditBarcodeContPath = new QLineEdit(grpConfBarcode);
    QLabel *labelLabelSet = new QLabel(trUtf8("Größe eines Satzes Etiketten für Vorauswahl"),grpConfBarcode);
    spinBoxLabelSet = new QSpinBox(grpConfBarcode);
    spinBoxLabelSet->setRange(10,10000);
    spinBoxLabelSet->setValue(500);

    QHBoxLayout *layoutLabelSet = new QHBoxLayout;
    layoutLabelSet->addWidget(labelLabelSet);
    layoutLabelSet->addWidget(spinBoxLabelSet);

    QGridLayout *layoutGrpConfBarcode = new QGridLayout(grpConfBarcode);
    layoutGrpConfBarcode->addWidget(labelBarcode,0,0,1,2);
    layoutGrpConfBarcode->addWidget(buttonBarcodePath,1,0);
    layoutGrpConfBarcode->addWidget(lineEditBarcodePath,1,1);
    layoutGrpConfBarcode->addWidget(labelBarcodeCont,2,0,1,2);
    layoutGrpConfBarcode->addWidget(buttonBarcodeContPath,3,0);
    layoutGrpConfBarcode->addWidget(lineEditBarcodeContPath,3,1);
    layoutGrpConfBarcode->addLayout(layoutLabelSet,4,0,1,2);
    layoutGrpConfBarcode->setColumnStretch(1,1);
    layoutGrpConfBarcode->setRowStretch(5,1);

    QGroupBox *grpEncryption = new QGroupBox(trUtf8("Barcode-Verschlüsselung"),this);
    QLabel *labelEncryption = new QLabel(trUtf8("Verschlüsselungstyp"),grpEncryption);
    comboBoxEncryption = new QComboBox(this);
    comboBoxEncryption->addItem(trUtf8("Neuer Algorithmus"));
    comboBoxEncryption->addItem(trUtf8("Kompatiblitätsmodus"));
    QLabel *labelCypher = new QLabel(trUtf8("Chiffre"),grpEncryption);
    spinBoxCypher = new QSpinBox(this);
    spinBoxCypher->setRange(1,32767);

    QVBoxLayout *layoutEncryption = new QVBoxLayout(grpEncryption);
    layoutEncryption->addWidget(labelEncryption);
    layoutEncryption->addWidget(comboBoxEncryption);
    layoutEncryption->addWidget(labelCypher);
    layoutEncryption->addWidget(spinBoxCypher);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal,this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(grpConfSerial,0,0);
    layout->addWidget(grpEncryption,1,0);
    layout->addWidget(grpConfBarcode,0,1,2,1);
    layout->addWidget(buttonBox,2,0,1,2);

    connect(buttonBox,SIGNAL(accepted()),SLOT(processOK()));
    connect(buttonBox,SIGNAL(rejected()),SLOT(reject()));
    connect(buttonReloadPorts,SIGNAL(clicked()),SLOT(collectSerialPorts()));
    connect(buttonBarcodePath,SIGNAL(clicked()),SLOT(processSelectBarcodePath()));
    connect(buttonBarcodeContPath,SIGNAL(clicked()),SLOT(processSelectBarcodeContPath()));
    connect(lineEditBarcodePath,SIGNAL(textChanged(QString)),SLOT(updateContPath()));
    connect(comboBoxEncryption,SIGNAL(currentIndexChanged(int)),SLOT(updateCypherField(int)));

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
    lineEditBarcodePath->setText(settings.value("barcode/path").toString());
    lineEditBarcodeContPath->setText(settings.value("barcode/contpath").toString());
    spinBoxLabelSet->setValue(settings.value("barcode/setsize",500).toInt());
    comboBoxEncryption->setCurrentIndex(settings.value("barcode/encryptiontype",0).toInt());
    spinBoxCypher->setValue(settings.value("barcode/encryptioncypher",1337).toInt());
}

void dlgConfigure::saveConfig()
{
    QSettings settings;
    settings.setValue("serial/port",comboBoxSerialPort->currentText());
    settings.setValue("serial/baudrate",comboBoxBaudRate->currentText());
    settings.setValue("barcode/path",lineEditBarcodePath->text());
    settings.setValue("barcode/contpath",lineEditBarcodeContPath->text());
    settings.setValue("barcode/setsize",spinBoxLabelSet->value());
    settings.setValue("barcode/encryptiontype",comboBoxEncryption->currentIndex());
    settings.setValue("barcode/encryptioncypher",spinBoxCypher->value());
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

void dlgConfigure::processSelectBarcodePath()
{
    QSettings settings;
    QString barcodePath = QFileDialog::getOpenFileName(this,trUtf8("Barcode-Datei öffnen"),settings.value("barcode/lastdirectory",QDir::homePath()).toString(),trUtf8("Barcode-Dateien (*.bar)"));
    if( !barcodePath.isEmpty() ) {
        settings.setValue("barcode/lastdirectory",QFileInfo(barcodePath).path());
        lineEditBarcodePath->setText(barcodePath);
    }
}

void dlgConfigure::processSelectBarcodeContPath()
{
    QSettings settings;
    QString barcodePath = QFileDialog::getOpenFileName(this,trUtf8("Barcode-Cont-Datei öffnen"),settings.value("barcode/lastdirectory",QDir::homePath()).toString(),trUtf8("Barcode-Dateien (*.bar)"));
    if( !barcodePath.isEmpty() ) {
        settings.setValue("barcode/lastdirectory",QFileInfo(barcodePath).path());
        lineEditBarcodeContPath->setText(barcodePath);
    }
}

void dlgConfigure::updateContPath()
{
    QString barcodePath = lineEditBarcodePath->text();
    if( !QFile(barcodePath).exists() )
        return;
    if( barcodePath.right(4) != ".bar" )
        return;
    barcodePath.chop(4);
    QStringList barcodeSuffix;
    barcodeSuffix << "_cont.bar" << "cont.bar" << ".cont.bar" << ".contbar";
    foreach(QString barcodeContSuffix, barcodeSuffix)
        if( QFile(barcodePath+barcodeContSuffix).exists() ) {
            lineEditBarcodeContPath->setText(barcodePath+barcodeContSuffix);
            return;
        }
}

void dlgConfigure::updateCypherField(int index)
{
    spinBoxCypher->setEnabled(index == 0);
}
