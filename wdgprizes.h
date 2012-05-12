#ifndef WDGPRIZES_H
#define WDGPRIZES_H

#include <QtGui>

class prizelistmodel;

class wdgPrizes : public QWidget
{
    Q_OBJECT
public:
    explicit wdgPrizes(QWidget *parent = 0);
    QString currentFileName() const;

public slots:
    bool updatePrizeListFileName(QString newFileName); //returns true if change was successful (i.e. not cancelled)
    bool promptSaveChanges(); //returns true if changes were saved or discarded
    
private:
    prizelistmodel *model;
    QTableView *tableView;
    QPushButton *buttonSaveChanges;
    QPushButton *buttonAddRow;
    QPushButton *buttonRemoveRow;
    QPushButton *buttonMoveRowUp;
    QPushButton *buttonMoveRowDown;
    QPushButton *buttonDuplicateRow;
    QString fileName;

    bool readFile();
    bool changed;

private slots:
    void processAddRow();
    void processRemoveRow();
    void processMoveRowUp();
    void processMoveRowDown();
    void processDuplicateRow();
    void dataChanged(const QModelIndex& topLeft);
    bool saveChanges(); //returns true if changes were successfully saved

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void prizeListFileNameChanged();
};

#endif // WDGPRIZES_H
