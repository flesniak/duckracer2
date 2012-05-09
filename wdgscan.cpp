#include "wdgscan.h"
#include "duckplacemodel.h"
#include "scanner.h"

wdgScan::wdgScan(QWidget *parent) : QWidget(parent), changed(false), doAutosave(false), currentPlace(1)
{
    scan = new scanner(this);

    model = new duckplacemodel(this);
    viewScanList = new QTableView(this);
    viewScanList->setModel(model);
    viewScanList->horizontalHeader()->setStretchLastSection(true);

    QFont font;
    font.setPointSize(24);

    editPlace = new QLineEdit(this);
    editPlace->setReadOnly(true);
    editPlace->setFont(font);
    editPlace->setAlignment(Qt::AlignCenter);
    QValidator *validator = new QIntValidator(1, 32767, this);
    editDuck = new QLineEdit(this);
    editDuck->setValidator(validator);
    editDuck->setFont(font);
    editDuck->setAlignment(Qt::AlignCenter);

    font.setBold(true);
    QLabel *labelPlace = new QLabel(trUtf8("Platzierung"),this);
    labelPlace->setFont(font);
    QLabel *labelDuck = new QLabel(trUtf8("Enten-Nr."),this);
    labelDuck->setFont(font);
    labelHint = new QLabel(this);
    labelHint->setFont(font);
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::red);
    labelHint->setPalette(pal);

    buttonCommit = new QPushButton(trUtf8("Ente übernehmen"),this);
    buttonCommit->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    font.setPixelSize(16);
    buttonCommit->setFont(font);
    buttonSaveChanges = new QPushButton(QIcon::fromTheme("document-save"),trUtf8("Änderungen speichern"),this);

    checkBoxAutosave = new QCheckBox(trUtf8("Automatisch speichern"),this);
    checkBoxAutosave->setToolTip(trUtf8("Neu gescannte Enten werden sofort auf die Festplatte geschrieben, um die Datensicherheit bei eventuellen Abstürzen zu gewährleisten. Dazu muss eine Datei gewählt werden."));
    checkBoxScanner = new QCheckBox(trUtf8("Scanner verwenden"),this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(viewScanList,0,0,6,1);
    layout->addWidget(labelPlace,0,1);
    layout->addWidget(labelDuck,0,2);
    layout->addWidget(editPlace,1,1);
    layout->addWidget(editDuck,1,2);
    layout->addWidget(labelHint,2,1,1,2);
    layout->addWidget(checkBoxAutosave,3,1);
    layout->addWidget(checkBoxScanner,4,1);
    layout->addWidget(buttonCommit,3,2,2,1);
    layout->addWidget(buttonSaveChanges,5,1,1,2);
    layout->setRowStretch(2,1);

    connect(scan,SIGNAL(newData(QByteArray)),SLOT(processNewData(QByteArray)));
    connect(editDuck,SIGNAL(returnPressed()),SLOT(processCommitData()));
    connect(buttonCommit,SIGNAL(clicked()),SLOT(processCommitData()));
    connect(buttonSaveChanges,SIGNAL(clicked()),SLOT(saveChanges()));
    connect(checkBoxAutosave,SIGNAL(toggled(bool)),SLOT(processToggleAutosave(bool)));
    connect(checkBoxScanner,SIGNAL(toggled(bool)),SLOT(processActivateScanner(bool)));

    updatePlaceEdit();
}

void wdgScan::processActivateScanner(bool active)
{
    if( active ) {
        if( !scan->isRunning() )
            scan->start();
    }
    else
        scan->stop();
}

void wdgScan::processCommitData()
{
    if( editDuck->text().isEmpty() )
        return;
    int temp = editDuck->text().toInt();
    if( temp < 0 || temp > 32767 )
        return;
    if( model->appendPlace(temp) ) {
        editDuck->clear();
        autosave();
    }
    else
        QMessageBox::warning(this,trUtf8("Warnung"),trUtf8("Ente wurde bereits erfasst!"));
    editDuck->setFocus();
}

void wdgScan::reloadConfiguration()
{
    QSettings settings;
    scannerPort = settings.value("serial/port").toString();
    scannerBaudrateString = settings.value("serial/baudrate").toString();
    barcodeEncryption = settings.value("barcode/encryptiontype",0).toInt();
    barcodeCypher = settings.value("barcode/encryptioncypher",1337).toInt();
}

void wdgScan::processNewData(QByteArray data)
{
    if( data.size() < 3 )
        return;
    unsigned short ducknum = -1;
    switch( barcodeEncryption ) {
    case 0 : ducknum = scanner::decode(data,barcodeCypher);
        break;
    case 1 : ducknum = scanner::decodeLegacy(data);
        break;
    }
    editDuck->setText(QString::number(ducknum));
    //if( comboBoxAutoCommit.isChecked) ...
}

void wdgScan::autosave(const QModelIndex &topLeft, const QModelIndex &)
{
    changed = true; //functions call autosave even if it is disabled, in this case we have to remember the changed state for promptSaveChanges
    qDebug() << "autosave: changed = true";
    if( !doAutosave ) {
        qDebug() << "autosave: disabled, return";
        return;
    }
    qDebug() << "autosave: continue";
    if( !QFile(fileName).exists() || topLeft.row() < model->rowCount(QModelIndex())-1 ) { //if data was changed somewhere in between, we have to save everything (for simplicity)
        saveChanges();
        return;
    }
    //Write incremental
    QFile file(fileName);
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht zum Schreiben geöffnet werden. Automatisches Speichern deaktiviert.").arg(fileName));
        doAutosave = false;
        return;
    }
    QTextStream stream(&file);
    stream << QString::number(model->getContent()->at(model->getContent()->size()-1)) << endl;
    file.close();
    changed = false;
}

void wdgScan::processToggleAutosave(bool enabled)
{
    if( doAutosave == enabled )
        return;
    doAutosave = true;
    if( saveChanges() == false ) {
        checkBoxAutosave->setChecked(false);
        doAutosave = false;
    }
}

void wdgScan::updatePlaceEdit()
{
    editPlace->setText(QString::number(model->rowCount(QModelIndex())));
}

void wdgScan::closeEvent(QCloseEvent *event)
{
    qDebug() << "wdgScan closeEvent!";
    if( promptSaveChanges() )
        event->accept();
    else
        event->ignore();
}

bool wdgScan::updateScanFileName(QString newFileName)
{
    if( promptSaveChanges() ) {
        fileName = newFileName;
        if( fileName.isEmpty() ) { //"Closing file" -> clearing data
            model->clear();
            return true;
        }
        else
            return readFile();
    }
    else
        return false; //promtSaveChanges failed, will stay with old file!
}

bool wdgScan::readFile() //returns true on success
{
    if( fileName.isEmpty() )
        return false;
    QFile file(fileName);
    if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht geöffnet werden").arg(fileName));
        model->clear();
        return false;
    }
    QTextStream stream(&file);
    model->getContent()->clear();
    while( !stream.atEnd() ) {
        bool ok;
        int temp = stream.readLine().toInt(&ok);
        if( !ok )
            continue;
            //temp = -1;
        model->getContent()->append(temp);
    }
    file.close();
    model->newData();
    return true;
}

bool wdgScan::promptSaveChanges()
{
    if( changed )
        switch(QMessageBox::question(this,trUtf8("Daten geändert"),trUtf8("Vorgenommen Änderungen speichern?"),QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel)) {
            case QMessageBox::Save : return saveChanges();
                                     break;
            case QMessageBox::Discard : return true;
                                        break;
            case QMessageBox::Cancel : return false;
                                       break;
            default : return false;
                      break;
        }
    else
        return true; //If our data didn't change, we don't need to save and are free to i.e. load a new file
}

bool wdgScan::saveChanges()
{
    if( changed ) {
        if( fileName.isEmpty() ) {
            QSettings settings;
            fileName = QFileDialog::getSaveFileName(this,trUtf8("Scan-Datei speichern"),settings.value("duckracer/lastdirectory",QDir::homePath()).toString(),trUtf8("Scan-Dateien (*.dsc);;Textdateien (*.txt)"));
            if( fileName.isEmpty() )
                return false;
            emit scanFileNameChanged();
        }
        QFile file(fileName);
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
            QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht zum Schreiben geöffnet werden").arg(fileName));
            return false;
        }
        QTextStream stream(&file);
        foreach(int entry, *(model->getContent()))
            stream << entry << endl;
        file.close();
        changed = false;
        return true;
    }
    else
        return true; //If our data didn't change, we don't need to save and are free to i.e. load a new file
}

const QString& wdgScan::currentFileName() const
{
    return fileName;
}
