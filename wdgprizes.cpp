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

    connect(buttonAddRow,SIGNAL(clicked()),SLOT(processAddRow()));
    connect(buttonRemoveRow,SIGNAL(clicked()),SLOT(processRemoveRow()));
    connect(buttonMoveRowUp,SIGNAL(clicked()),SLOT(processMoveRowUp()));
    connect(buttonMoveRowDown,SIGNAL(clicked()),SLOT(processMoveRowDown()));
    connect(buttonDuplicateRow,SIGNAL(clicked()),SLOT(processDuplicateRow()));
    connect(buttonSaveChanges,SIGNAL(clicked()),SLOT(saveChanges()));

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(dataChanged(QModelIndex)));
}

void wdgPrizes::closeEvent(QCloseEvent *event)
{
    if( promptSaveChanges() )
        event->accept();
    else
        event->ignore();
}

bool wdgPrizes::updatePrizeListFileName(QString newFileName)
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

bool wdgPrizes::readFile() //returns true on success
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
    while( !stream.atEnd() )
        model->getContent()->append(stream.readLine());
    file.close();
    model->newData();
    return true;
}

bool wdgPrizes::promptSaveChanges()
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

bool wdgPrizes::saveChanges()
{
    if( changed ) {
        if( fileName.isEmpty() ) {
            QSettings settings;
            fileName = QFileDialog::getSaveFileName(this,trUtf8("Preisliste speichern"),settings.value("duckracer/lastdirectory",QDir::homePath()).toString(),trUtf8("Preislisten (*.prz)"));
            if( fileName.isEmpty() )
                return false;
            if( !fileName.endsWith(".prz") )
                fileName.append(".prz");
            emit prizeListFileNameChanged();
        }
        QFile file(fileName);
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
            QMessageBox::critical(this,trUtf8("Fehler"),trUtf8("Datei %1 konnte nicht zum Schreiben geöffnet werden").arg(fileName));
            return false;
        }
        QTextStream stream(&file);
        foreach(QString entry, *(model->getContent()))
            stream << entry << endl;
        file.close();
        changed = false;
        return true;
    }
    else
        return true; //If our data didn't change, we don't need to save and are free to i.e. load a new file
}

void wdgPrizes::processAddRow()
{
    if( model->insertRows(tableView->selectionModel()->selectedRows().value(0).row(),1,QModelIndex()) )
        changed = true;
}

void wdgPrizes::processRemoveRow()
{
    QList<QModelIndex> indexList = tableView->selectionModel()->selectedRows();
    qSort(indexList);
    for(int index = indexList.size()-1; index >= 0; index--)
        if( model->removeRow(indexList.at(index).row()) )
            changed = true;
}

void wdgPrizes::processMoveRowUp()
{
    int row = tableView->selectionModel()->selectedRows().value(0).row();
    if( model->moveRowUp(row) ) {
        tableView->selectionModel()->select(model->index(row-1,0),QItemSelectionModel::SelectCurrent);
        changed = true;
    }
}

void wdgPrizes::processMoveRowDown()
{
    int row = tableView->selectionModel()->selectedRows().value(0).row();
    if( model->moveRowDown(row) ) {
        tableView->selectionModel()->select(model->index(row+1,0),QItemSelectionModel::SelectCurrent);
        changed = true;
    }
}

void wdgPrizes::processDuplicateRow()
{
    if( model->duplicateRow(tableView->selectionModel()->selectedRows().value(0).row()) )
        changed = true;
}

QString wdgPrizes::currentFileName() const
{
    return fileName;
}

void wdgPrizes::dataChanged(const QModelIndex& topLeft)
{
    changed = true;
    if( topLeft.row()+2 == model->rowCount(QModelIndex()) )
        tableView->selectRow(topLeft.row()+1);
}
