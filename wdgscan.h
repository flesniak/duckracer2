#ifndef WDGSCAN_H
#define WDGSCAN_H

#include <QtGui>

class duckplacemodel;
class scanner;

class wdgScan : public QWidget
{
    Q_OBJECT
public:
    explicit wdgScan(QWidget *parent = 0);
    QString currentFileName() const;

public slots:
    bool updateScanFileName(QString newFileName); //returns true if change was successful (i.e. not cancelled)
    bool promptSaveChanges(); //returns true if changes were saved or discarded
    void reloadConfiguration();
    void autosave(const QModelIndex &topLeft, const QModelIndex &);

private:
    QLineEdit *editPlace;
    QLineEdit *editDuck;
    QTableView *viewScanList;
    QPushButton *buttonCommit;
    QPushButton *buttonSaveChanges;
    QLabel *labelHint;
    QCheckBox *checkBoxAutosave;
    duckplacemodel *model;
    scanner *scan;
    QString fileName;
    QString scannerPort;
    QString scannerBaudrateString;
    int barcodeEncryption;
    unsigned short barcodeCypher;

    bool readFile();
    bool changed;
    bool doAutosave;
    int currentPlace;

private slots:
    void dataChanged();
    bool saveChanges(); //returns true if changes were successfully saved
    void processNewData(QByteArray data);
    void processCommitData();
    void updatePlaceEdit();
    void processToggleAutosave(bool enabled);

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void scanFileNameChanged();
};

#endif // WDGSCAN_H
