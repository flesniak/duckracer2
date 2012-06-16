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

    QPrinter *printer = new QPrinter();
    QPrintDialog *printDialog = new QPrintDialog(printer,this);
    printDialog->setWindowTitle(trUtf8("Liste drucken"));
    if( printDialog->exec() != QDialog::Accepted )
        return;

    //Open needed files
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

    //Create the document to print
    QTextDocument document;
    QTextCursor cursor(&document);

    //Headline
    QTextCharFormat charFormat = cursor.charFormat();
    charFormat.setFontPointSize(16);
    charFormat.setFontWeight(QFont::Bold);
    cursor.setCharFormat(charFormat);
    cursor.insertText(lineEditHeadline->text());

    //Table
    QTextTable *table = cursor.insertTable(2,checkBoxDuck->isChecked() + checkBoxPlace->isChecked() + checkBoxPrize->isChecked());
    /*QTextFrameFormat tableFrameFormat = table->frameFormat();
    //tableFrameFormat.setBorderBrush(blackBrush);
    //tableFrameFormat.setRightMargin(2);
    tableFrameFormat.setBorder(1);
    tableFrameFormat.setBorderBrush(QBrush(Qt::black));
    tableFrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Inset);
    //tableFrameFormat.setWidth(document.size().width());
    table->setFrameFormat(tableFrameFormat);*/
    QTextTableFormat tableFormat = table->format();
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorderBrush(QBrush(Qt::black));
    tableFormat.setBorder(0.5);
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(2);
    table->setFormat(tableFormat);

    //Table Header
    charFormat.setFontPointSize(12);
    if( checkBoxPlace->isChecked() ) {
        cursor.setCharFormat(charFormat);
        cursor.insertText(trUtf8("Platzierung"));
        cursor.movePosition(QTextCursor::NextCell);
    }
    if( checkBoxDuck->isChecked() ) {
        cursor.setCharFormat(charFormat);
        cursor.insertText(trUtf8("Enten-Nummer"));
        cursor.movePosition(QTextCursor::NextCell);
    }
    if( checkBoxPrize->isChecked() ) {
        cursor.setCharFormat(charFormat);
        cursor.insertText(trUtf8("Preis"));
        cursor.movePosition(QTextCursor::NextCell);
    }

    unsigned int place = 1;
    while( !(checkBoxDuck->isChecked() && scanFileStream.atEnd()) && !(checkBoxPrize->isChecked() && prizeFileStream.atEnd()) ) { //True as long as all files to use are not at end
        table->appendRows(1);
        if( checkBoxPlace->isChecked() ) {
            cursor.insertText(QString::number(place));
            cursor.movePosition(QTextCursor::NextCell);
        }
        if( checkBoxDuck->isChecked() ) {
            cursor.insertText(scanFileStream.readLine());
            cursor.movePosition(QTextCursor::NextCell);
        }
        if( checkBoxPrize->isChecked() ) {
            cursor.insertText(prizeFileStream.readLine());
            cursor.movePosition(QTextCursor::NextCell);
        }
        place++;
    }
    table->removeRows(table->rows()-1,1); //I don't know why, but we always need an empty row, otherwise we can't fill the table, so we remove the last empty line here
    document.print(printer);
    prizeFile.close();
    scanFile.close();
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
