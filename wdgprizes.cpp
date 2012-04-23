#include "wdgprizes.h"
#include "prizelistmodel.h"

wdgPrizes::wdgPrizes(QWidget *parent) : QWidget(parent), changed(false)
{
    buttonAddRow = new QPushButton(QIcon::fromTheme("archive-insert"),trUtf8("Zeile hinzufügen"),this);
    buttonRemoveRow = new QPushButton(QIcon::fromTheme("archive-remove"),trUtf8("Zeile(n) löschen"),this);
    buttonMoveRowUp = new QPushButton(QIcon::fromTheme("arrow-up"),trUtf8("Zeile nach oben verschieben"),this);
    buttonMoveRowDown = new QPushButton(QIcon::fromTheme("arrow-down"),trUtf8("Zeile nach unten verschieben"),this);
    buttonDuplicateRow = new QPushButton(QIcon::fromTheme("tab-duplicate"),trUtf8("Zeile duplizieren"),this);
    buttonSaveChanges = new QPushButton(QIcon::fromTheme("document-save"),trUtf8("Änderungen speichern"),this);
    model = new prizelistmodel;
    tableView = new QTableView(this);
    tableView->setModel(model);
    tableView->horizontalHeader()->setStretchLastSection(true);
    //tableView->horizontalHeader()->hide();

    QHBoxLayout *layoutButtonsUpper = new QHBoxLayout;
    layoutButtonsUpper->addWidget(buttonAddRow);
    layoutButtonsUpper->addWidget(buttonMoveRowUp);
    layoutButtonsUpper->addWidget(buttonDuplicateRow);
    QHBoxLayout *layoutButtonsLower = new QHBoxLayout;
    layoutButtonsLower->addWidget(buttonRemoveRow);
    layoutButtonsLower->addWidget(buttonMoveRowDown);
    layoutButtonsLower->addWidget(buttonSaveChanges);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
    layout->addLayout(layoutButtonsUpper);
    layout->addLayout(layoutButtonsLower);

    connect(buttonSaveChanges,SIGNAL(clicked()),SLOT(saveChanges()));
    connect(buttonAddRow,SIGNAL(clicked()),SLOT(processAddRow()));
    connect(buttonRemoveRow,SIGNAL(clicked()),SLOT(processRemoveRow()));
    connect(buttonMoveRowUp,SIGNAL(clicked()),SLOT(processMoveRowUp()));
    connect(buttonMoveRowDown,SIGNAL(clicked()),SLOT(processMoveRowDown()));
    connect(buttonDuplicateRow,SIGNAL(clicked()),SLOT(processDuplicateRow()));
}

bool wdgPrizes::updatePrizeListFileName(QString newFileName)
{
    if( promptSaveChanges() ) {
        fileName = newFileName; //Use false to create a new file if none was selected
        if( !fileName.isEmpty() )
            readFile();
        return true;
    }
    else
        return false;
}

bool wdgPrizes::readFile() //returns true on success
{
    QFile file(fileName);
    if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht geöffnet werden").arg(fileName));
        model->clear();
        return false;
    }
    QDataStream stream(&file);
    model->getContent()->clear();
    QStringList test;
    while( !stream.atEnd() )
        stream >> *(model->getContent());
    file.close();
    model->newData();
    return true;
}

void wdgPrizes::lock(bool b)
{
    tableView->setEnabled(b);
    buttonSaveChanges->setEnabled(b);
    buttonAddRow->setEnabled(b);
    buttonRemoveRow->setEnabled(b);
    buttonMoveRowUp->setEnabled(b);
    buttonMoveRowDown->setEnabled(b);
    buttonDuplicateRow->setEnabled(b);
}

bool wdgPrizes::promptSaveChanges()
{
    if( changed ) {
        switch(QMessageBox::question(this,tr("Daten geändert"),tr("Die Daten von %1 wurden geändert. Änderungen speichern?").arg(fileName),QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel)) {
            case QMessageBox::Save : return saveChanges();
                                     break;
            case QMessageBox::Discard : return true;
                                        break;
            case QMessageBox::Cancel : return false;
                                       break;
            default : return false;
                      break;
        }
    }
    else
        return true; //If our data didn't change, we don't need to save and are free to i.e. load a new file
}

bool wdgPrizes::saveChanges()
{
    if( changed ) {
        QFile file(fileName);
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
            QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht zum Schreiben geöffnet werden").arg(fileName));
            return false;
        }
        QDataStream stream(&file);
        foreach(QString entry, *(model->getContent()))
            stream << entry;
        file.close();
        return true;
    }
    else
        return true; //If our data didn't change, we don't need to save and are free to i.e. load a new file
}

void wdgPrizes::processAddRow()
{
    model->insertRow(tableView->selectionModel()->selectedRows().value(0).row());
}

void wdgPrizes::processRemoveRow()
{
    QList<QModelIndex> indexList = tableView->selectionModel()->selectedRows();
    qSort(indexList);
    for(int index = indexList.size()-1; index >= 0; index--)
        model->removeRow(indexList.at(index).row());
}

void wdgPrizes::processMoveRowUp()
{
    int row = tableView->selectionModel()->selectedRows().value(0).row();
    if( model->moveRowUp(row) )
        tableView->selectionModel()->select(model->index(row-1,0),QItemSelectionModel::SelectCurrent);
}

void wdgPrizes::processMoveRowDown()
{
    int row = tableView->selectionModel()->selectedRows().value(0).row();
    if( model->moveRowDown(row) )
        tableView->selectionModel()->select(model->index(row+1,0),QItemSelectionModel::SelectCurrent);
}

void wdgPrizes::processDuplicateRow()
{
    model->duplicateRow(tableView->selectionModel()->selectedRows().value(0).row());
}
