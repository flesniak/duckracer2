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

    buttonPrint = new QPushButton(trUtf8("Liste drucken"),this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(grpElements,0,0);
    layout->addWidget(grpSortBy,0,1);
    layout->addWidget(buttonPrint,1,0,1,2);

    connect(checkBoxPlace,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(checkBoxDuck,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(checkBoxPrize,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(radioButtonPlace,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(radioButtonDuck,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));
    connect(radioButtonPrize,SIGNAL(toggled(bool)),SLOT(updateCheckBoxes()));

    updateCheckBoxes();
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
    if( !radioButtonPlace->isEnabled() )
        radioButtonPlace->setChecked(false);
    radioButtonDuck->setEnabled(checkBoxDuck->isChecked() && !scanFileName.isEmpty());
    if( !radioButtonDuck->isEnabled() )
        radioButtonDuck->setChecked(false);
    radioButtonPrize->setEnabled(checkBoxPrize->isChecked() && !prizeListFileName.isEmpty());
    if( !radioButtonPrize->isEnabled() )
        radioButtonPrize->setChecked(false);

    buttonPrint->setEnabled( (checkBoxPlace->isChecked() || checkBoxDuck->isChecked() || checkBoxPlace->isChecked())
                             && (radioButtonPlace->isChecked() || radioButtonDuck->isChecked() || radioButtonPrize->isChecked()) );
}
