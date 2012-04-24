#include "wdgprintlabels.h"

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
    buttonPrint = new QPushButton(QIcon::fromTheme("printer"),trUtf8("Etiketten drucken"),grpLabelRange);

    QVBoxLayout *layoutLabelRange = new QVBoxLayout(grpLabelRange);
    layoutLabelRange->addWidget(labelFirstLabel);
    layoutLabelRange->addWidget(spinBoxFirstLabel);
    layoutLabelRange->addWidget(labelLastLabel);
    layoutLabelRange->addWidget(spinBoxLastLabel);
    layoutLabelRange->addWidget(labelPreselection);
    layoutLabelRange->addWidget(comboBoxPreselection);
    layoutLabelRange->addWidget(buttonPrint);

    connect(comboBoxPreselection,SIGNAL(currentIndexChanged(QString)),SLOT(comboBoxIndexChanged(QString)));
    connect(spinBoxFirstLabel,SIGNAL(valueChanged(int)),SLOT(spinBoxValuesChanged()));
    connect(spinBoxLastLabel,SIGNAL(valueChanged(int)),SLOT(spinBoxValuesChanged()));
    connect(buttonPrint,SIGNAL(clicked()),SLOT(processPrint()));

    comboBoxIndexChanged(comboBoxPreselection->currentText());
}

void wdgPrintLabels::spinBoxValuesChanged()
{
    if( block )
        return;
    block = true;
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

void wdgPrintLabels::processPrint()
{

}
