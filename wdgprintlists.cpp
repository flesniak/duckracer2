#include "wdgprintlists.h"

wdgPrintLists::wdgPrintLists(QWidget *parent) : QWidget(parent)
{
    QGroupBox *grpElements = new QGroupBox(trUtf8("Zu druckende Spalten"),this);
    checkBoxPlace = new QCheckBox(trUtf8("Platzierung"),grpElements);
    checkBoxDuck = new QCheckBox(trUtf8("Enten-Nummer"),grpElements);
    checkBoxPrize = new QCheckBox(trUtf8("Preis"),grpElements);

    QVBoxLayout *layoutElements = new QVBoxLayout(grpElements);
    layoutElements->addWidget(checkBoxPlace);
    layoutElements->addWidget(checkBoxDuck);
    layoutElements->addWidget(checkBoxPrize);

    QGroupBox *grpSortBy = new QGroupBox(trUtf8("Liste sortieren"),this);
    radioButtonPlace = new QRadioButton(trUtf8("nach Platzierung"),grpSortBy);
    radioButtonDuck = new QRadioButton(trUtf8("nach Enten-Nummer"),grpSortBy);
    radioButtonPrize = new QRadioButton(trUtf8("nach Preis"),grpSortBy);

    QVBoxLayout *layoutSortBy = new QVBoxLayout(grpSortBy);
    layoutSortBy->addWidget(radioButtonPlace);
    layoutSortBy->addWidget(radioButtonDuck);
    layoutSortBy->addWidget(radioButtonPrize);

    QLabel *labelHeadline = new QLabel(trUtf8("Kopfzeile"),this);
    lineEditHeadline = new QLineEdit(this);

    QHBoxLayout *layoutHeadline = new QHBoxLayout;
    layoutHeadline->addWidget(labelHeadline);
    layoutHeadline->addWidget(lineEditHeadline);

    buttonPrint = new QPushButton(trUtf8("Liste drucken"),this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(grpElements,0,0);
    layout->addWidget(grpSortBy,0,1);
    layout->addLayout(layoutHeadline,1,0,1,2);
    layout->addWidget(buttonPrint,2,0,1,2);

    connect(checkBoxPlace,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(checkBoxDuck,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(checkBoxPrize,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(radioButtonPlace,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(radioButtonDuck,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(radioButtonPrize,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(buttonPrint,SIGNAL(clicked()),SLOT(print()));

    loadSettings();
    updateCheckBoxes();
}

void wdgPrintLists::print()
{
    if( prizeListFileName.isEmpty() && scanFileName.isEmpty() )
        return;
    saveSettings();

    QPrinter printer;
    QPrintDialog *printDialog = new QPrintDialog(&printer,this);
    printDialog->setWindowTitle(trUtf8("Liste drucken"));
    if( printDialog->exec() != QDialog::Accepted )
        return;
    QPainter painter(&printer);
    //print by place for now only
    QFile scanFile(scanFileName);
    if( !scanFile.open(QFile::ReadOnly) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht geöffnet werden!").arg(scanFileName));
        return;
    }
    QTextStream scanFileStream(&scanFile);
    QFile prizeFile(prizeListFileName);
    if( !prizeFile.open(QFile::ReadOnly) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht geöffnet werden!").arg(prizeListFileName));
        return;
    }
    QTextStream prizeFileStream(&prizeFile);

    //HEADLINE
    QRectF bounds = painter.boundingRect(printer.pageRect(),lineEditHeadline->text());
    bounds.moveCenter(QPointF(printer.pageRect().width()/2,bounds.height()/2));
    painter.drawText(bounds,lineEditHeadline->text());

    //TABLE
    QTextDocument document;
    QTextTable table(&document);
    while( !prizeFileStream.atEnd() ) {
        table.insertColumns(0,3);
    }
    painter.end();
    qDebug() << "print end";
}

bool wdgPrintLists::updateFileNames(const QString &newPrizeListFileName, const QString &newScanFileName)
{
    prizeListFileName = newPrizeListFileName;
    scanFileName = newScanFileName;
    updateCheckBoxes();
    return true;
}

void wdgPrintLists::updateCheckBoxes()
{
    checkBoxPlace->setEnabled(!scanFileName.isEmpty() || !prizeListFileName.isEmpty());
    checkBoxDuck->setEnabled(!scanFileName.isEmpty());
    checkBoxPrize->setEnabled(!prizeListFileName.isEmpty());
    radioButtonPlace->setEnabled(checkBoxPlace->isChecked() && (!scanFileName.isEmpty() || !prizeListFileName.isEmpty()));
    radioButtonDuck->setEnabled(checkBoxDuck->isChecked() && !scanFileName.isEmpty());
    radioButtonPrize->setEnabled(checkBoxPrize->isChecked() && !prizeListFileName.isEmpty());

    buttonPrint->setEnabled( (checkBoxPlace->isChecked() || checkBoxDuck->isChecked() || checkBoxPlace->isChecked())
                             && ((radioButtonPlace->isEnabled() && radioButtonPlace->isChecked())
                              || (radioButtonDuck->isEnabled() && radioButtonDuck->isChecked())
                              || (radioButtonPrize->isEnabled() && radioButtonPrize->isChecked())) );
}

void wdgPrintLists::loadSettings()
{
    QSettings settings;
    lineEditHeadline->setText(settings.value("lists/headline",trUtf8("Entenrennen RoundTable")).toString());
}

void wdgPrintLists::saveSettings()
{
    QSettings settings;
    settings.setValue("lists/headline",lineEditHeadline->text());
}
