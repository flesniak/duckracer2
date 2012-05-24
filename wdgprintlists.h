#ifndef WDGPRINTLISTS_H
#define WDGPRINTLISTS_H

#include <QtGui>

class wdgPrintLists : public QWidget
{
    Q_OBJECT
public:
    explicit wdgPrintLists(QWidget *parent = 0);
    bool updateFileNames(const QString &newPrizeListFileName, const QString &newScanFileName);
    void reloadConfiguration();

private:
    QCheckBox *checkBoxPlace;
    QCheckBox *checkBoxDuck;
    QCheckBox *checkBoxPrize;
    QRadioButton *radioButtonPlace;
    QRadioButton *radioButtonDuck;
    QRadioButton *radioButtonPrize;
    QPushButton *buttonPrint;
    QLineEdit *lineEditHeadline;
    QString prizeListFileName;
    QString scanFileName;

    void loadSettings();
    void saveSettings();

private slots:
    void updateCheckBoxes();
    void print();
};

#endif // WDGPRINTLISTS_H
