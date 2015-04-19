#include "wdgprintlabels.h"
#include "scanner.h"

wdgPrintLabels::wdgPrintLabels(QWidget *parent) : QWidget(parent), block(false)
{
    QGroupBox *grpLabelRange = new QGroupBox(trUtf8("Druckparameter"),this);
    QLabel *labelFirstLabel = new QLabel(trUtf8("Nummer des ersten Etiketts"),grpLabelRange);
    spinBoxFirstLabel = new QSpinBox(grpLabelRange);
    spinBoxFirstLabel->setRange(1,1000000);
    QLabel *labelLastLabel = new QLabel(trUtf8("Nummer des letzten Etiketts"),grpLabelRange);
    spinBoxLastLabel = new QSpinBox(grpLabelRange);
    spinBoxLastLabel->setRange(1,1000000);
    QLabel *labelPreselection = new QLabel(trUtf8("Schnellwahl"),grpLabelRange);
    comboBoxPreselection = new QComboBox(grpLabelRange);
    QStringList preselectionValues;
    QSettings settings;
    int setSize = settings.value("barcode/setsize",500).toInt();
    if( setSize <= 0 )
        setSize = 500;
    for(int num = 1; num < 100000 && preselectionValues.size() < 5000; num+=setSize)
        preselectionValues << QString::number(num)+"-"+QString::number(num+setSize-1);
    comboBoxPreselection->addItems(preselectionValues);
    comboBoxPreselection->setCurrentIndex(0);
    QLabel *labelYear = new QLabel(trUtf8("Jahr"),grpLabelRange);
    spinBoxYear = new QSpinBox(grpLabelRange);
    spinBoxYear->setRange(2010,2200);
    spinBoxYear->setValue(settings.value("barcode/lastyear",QDate::currentDate().year()).toInt());

    QVBoxLayout *layoutLabelRange = new QVBoxLayout(grpLabelRange);
    layoutLabelRange->addWidget(labelFirstLabel);
    layoutLabelRange->addWidget(spinBoxFirstLabel);
    layoutLabelRange->addWidget(labelLastLabel);
    layoutLabelRange->addWidget(spinBoxLastLabel);
    layoutLabelRange->addWidget(labelPreselection);
    layoutLabelRange->addWidget(comboBoxPreselection);
    layoutLabelRange->addWidget(labelYear);
    layoutLabelRange->addWidget(spinBoxYear);
    layoutLabelRange->addStretch();

    QGroupBox *grpControlFiles = new QGroupBox(trUtf8("Drucker-Steuerungsdateien"),this);
    QLabel *labelbarcodeFileNameHint = new QLabel(trUtf8("Folgende Barcode-Steuerdateien werden verwendet:"),grpControlFiles);
    labelEncryption = new QLabel(grpControlFiles);

    QWidget *scrollAreaContainer = new QWidget;
    labelBarcodeFileName = new QLabel(scrollAreaContainer);
    labelBarcodeContFileName = new QLabel(scrollAreaContainer);

    QVBoxLayout *layoutScrollArea = new QVBoxLayout(scrollAreaContainer);
    layoutScrollArea->addWidget(labelBarcodeFileName);
    layoutScrollArea->addWidget(labelBarcodeContFileName);
    layoutScrollArea->addStretch();

    QScrollArea *scrollAreabarcodeFileNames = new QScrollArea(labelbarcodeFileNameHint);
    scrollAreabarcodeFileNames->setWidgetResizable(true);
    scrollAreabarcodeFileNames->setWidget(scrollAreaContainer);

    QVBoxLayout *layoutControlFiles = new QVBoxLayout(grpControlFiles);
    layoutControlFiles->addWidget(labelbarcodeFileNameHint);
    layoutControlFiles->addWidget(scrollAreabarcodeFileNames);
    layoutControlFiles->addWidget(labelEncryption);

    QGroupBox *grpLp = new QGroupBox(trUtf8("Drucker"),this);
    QLabel *labelLp = new QLabel(trUtf8("Drucker wählen"),grpLp);
    comboBoxLp = new QComboBox(grpLp);
    QDir dirDev("/dev");
    QDir dirDevUsb("/dev/usb");
    QStringList dirFilters;
    dirFilters << "lp?";
    dirFilters << "usblp?";
    const QDir::Filters filt = QDir::Readable | QDir::Files | QDir::Drives | QDir::System;
    QStringList devItems = dirDev.entryList(dirFilters,filt);
    devItems.replaceInStrings(QRegExp("^"),dirDev.path()+"/");
    QStringList devUsbItems = dirDevUsb.entryList(dirFilters,filt);
    devUsbItems.replaceInStrings(QRegExp("^"),dirDevUsb.path()+"/");
    comboBoxLp->addItems(devItems);
    comboBoxLp->addItems(devUsbItems);

    QHBoxLayout *layoutLp = new QHBoxLayout(grpLp);
    layoutLp->addWidget(labelLp);
    layoutLp->addWidget(comboBoxLp);

    buttonPrint = new QPushButton(QIcon::fromTheme("printer"),trUtf8("Etiketten drucken"),grpLabelRange);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(grpLabelRange,0,0,2,1);
    layout->addWidget(grpControlFiles,0,1);
    layout->addWidget(grpLp,1,1);
    layout->addWidget(buttonPrint,2,0,1,2);

    connect(comboBoxPreselection,SIGNAL(currentIndexChanged(QString)),SLOT(comboBoxIndexChanged(QString)));
    connect(spinBoxFirstLabel,SIGNAL(valueChanged(int)),SLOT(spinBoxValuesChanged()));
    connect(spinBoxLastLabel,SIGNAL(valueChanged(int)),SLOT(spinBoxValuesChanged()));
    connect(comboBoxLp,SIGNAL(currentIndexChanged(int)),SLOT(updatePrintButton()));
    connect(buttonPrint,SIGNAL(clicked()),SLOT(processPrint()));

    comboBoxIndexChanged(comboBoxPreselection->currentText());
    reloadPrintParameters();
}

void wdgPrintLabels::processPrint()
{
    //Open barcode files
    QFile barcodeFile(printparameters.barcodeFileName);
    if( !barcodeFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Barcode-Datei %1 konnte nicht geöffnet werden").arg(printparameters.barcodeFileName));
        return;
    }
    QFile barcodeContFile(printparameters.barcodeContFileName);
    if( !barcodeContFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Barcode-Cont-Datei %1 konnte nicht geöffnet werden").arg(printparameters.barcodeContFileName));
        return;
    }

    //Read barcode files to QByteArrays
    QByteArray barcodeFirstData;
    QTextStream barcodeFileStream(&barcodeFile);
    while( !barcodeFileStream.atEnd() ) {
        QString barcodeFileStr = barcodeFileStream.readLine();
        if( !barcodeFileStr.isEmpty() && !barcodeFileStr.startsWith(';') )
            barcodeFirstData.append(barcodeFileStr.toAscii() + "\n");
    }
    QByteArray barcodeContinueData;
    barcodeFileStream.setDevice(&barcodeContFile);
    while( !barcodeFileStream.atEnd() ) {
        QString barcodeFileStr = barcodeFileStream.readLine();
        if( !barcodeFileStr.isEmpty() && !barcodeFileStr.startsWith(';') )
            barcodeContinueData.append(barcodeFileStr.toUtf8() + "\n");
    }
    barcodeFile.close();
    barcodeContFile.close();

    QByteArray data;
    unsigned int currentLabel = spinBoxFirstLabel->value();

    //Initial printer commands
    barcodeFirstData.replace("YYYY",QString::number(spinBoxYear->value()).toAscii());
    barcodeFirstData.replace("SSSS",QString::number(currentLabel).toAscii().rightJustified(4,'0'));
    barcodeFirstData.replace("CCCC",printparameters.barcodeEncryption ? scanner::encodeLegacy(currentLabel) : scanner::encode(currentLabel,printparameters.barcodeCipher));
    currentLabel++;
    data.append(barcodeFirstData);

    //Incremental printer commands
    for(;currentLabel <= (unsigned int)spinBoxLastLabel->value(); currentLabel++) {
        QByteArray temp(barcodeContinueData);
        temp.replace("YYYY",QString::number(spinBoxYear->value()).toAscii());
        temp.replace("SSSS",QString::number(currentLabel).toAscii().rightJustified(4,'0'));
        temp.replace("CCCC",printparameters.barcodeEncryption ? scanner::encodeLegacy(currentLabel) : scanner::encode(currentLabel,printparameters.barcodeCipher));
        data.append(temp);
    }

    QFile printer(comboBoxLp->currentText());
    if( !printer.open( QIODevice::WriteOnly ) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Drucker-Port %1 konnte nicht geöffnet werden").arg(comboBoxLp->currentText()));
        return;
    }
    QTextStream printerStream(&printer);
    printerStream.setCodec(QTextCodec::codecForName("UTF-8"));
    printerStream << data;
    printer.close();
}

void wdgPrintLabels::spinBoxValuesChanged()
{
    if( block )
        return;
    block = true;
    if( spinBoxFirstLabel->value() > spinBoxLastLabel->value() )
        spinBoxLastLabel->setValue(spinBoxFirstLabel->value());
    comboBoxPreselection->setCurrentIndex(comboBoxPreselection->findText(spinBoxFirstLabel->text()+"-"+spinBoxLastLabel->text()));
    block = false;
}

void wdgPrintLabels::comboBoxIndexChanged(QString str)
{
    if( str.isEmpty() || block )
        return;
    block = true;
    spinBoxFirstLabel->setValue(str.split('-').at(0).toInt());
    spinBoxLastLabel->setValue(str.split('-').at(1).toInt());
    block = false;
}

void wdgPrintLabels::reloadPrintParameters()
{
    QSettings settings;
    printparameters.barcodeFileName = settings.value("barcode/path").toString();
    if( printparameters.barcodeFileName.isEmpty() )
        labelBarcodeFileName->setText(trUtf8("Keine Barcode-Datei ausgewählt!"));
    else
        labelBarcodeFileName->setText(printparameters.barcodeFileName);
    printparameters.barcodeContFileName = settings.value("barcode/contpath").toString();
    if( printparameters.barcodeContFileName.isEmpty() )
        labelBarcodeContFileName->setText(trUtf8("Keine Barcode-Cont-Datei ausgewählt!"));
    else
        labelBarcodeContFileName->setText(printparameters.barcodeContFileName);
    printparameters.barcodeEncryption = settings.value("barcode/encryptiontype",0).toInt();
    printparameters.barcodeCipher = settings.value("barcode/encryptioncypher",0).toInt();
    QString encryptionHint;
    if( printparameters.barcodeEncryption == 1 )
        encryptionHint = trUtf8("Alte Verschlüsselungsmethode (kompatibel mit Duckracer 1");
    else
        encryptionHint = trUtf8("Verschlüsselung mit Chiffre %1").arg(printparameters.barcodeCipher);
    labelEncryption->setText(encryptionHint);
    updatePrintButton();
}

void wdgPrintLabels::updatePrintButton()
{
    buttonPrint->setEnabled( !printparameters.barcodeFileName.isEmpty() && !printparameters.barcodeContFileName.isEmpty() && !comboBoxLp->currentText().isEmpty() );
}

void wdgPrintLabels::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("barcode/lastyear",spinBoxYear->value());
    event->accept();
}
