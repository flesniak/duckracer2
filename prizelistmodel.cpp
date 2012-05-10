#include "prizelistmodel.h"

#include <QStringList>

prizelistmodel::prizelistmodel(QObject *parent) : QAbstractListModel(parent)
{
    content = new QStringList;
    content->append(QString());
}

QVariant prizelistmodel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || index.row() > content->size() || (role != Qt::DisplayRole && role != Qt::EditRole) )
        return QVariant();
    if( index.row() == content->size() )
        return QString();
    else
        return content->at(index.row());
}

bool prizelistmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( index.row() > content->size() || value.type() != QVariant::String || role != Qt::EditRole )
        return false;
    if( index.row() == content->size() )
        return insertRows(index.row(),1,QModelIndex(),value.toString());
    (*content)[index.row()] = value.toString();
    emit dataChanged(index,index);
    return true;
}

int prizelistmodel::rowCount(const QModelIndex &) const
{
    return content->size()+1;
}

Qt::ItemFlags prizelistmodel::flags(const QModelIndex &) const
{
    return  Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant prizelistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole )
        return QVariant();
    if( orientation == Qt::Horizontal )
        return tr("Preis");
    else //Qt::Vertical
        return section+1;
}
#include <QDebug>
bool prizelistmodel::insertRows(int row, int count, const QModelIndex &parent, const QString &value)
{
    qDebug() << "insertRows" << row << count << value;
    if( row > content->size() || count == 0 )
        return false;
    if( row < 0 )
        row = 0;
    beginInsertRows(parent,row+1,row+count);
    for(int num = 0; num < count; num++)
        content->insert(row+1,value);
    endInsertRows();
    return true;
}

bool prizelistmodel::removeRows(int row, int count, const QModelIndex &parent)
{
    if( row < 0 || row >= content->size() )
        return false;
    beginRemoveRows(parent,row,row+count-1);
    for(int num = 0; num < count; num++)
        content->removeAt(row);
    endRemoveRows();
    if( content->isEmpty() )
        insertRow(-1);
    return true;
}

void prizelistmodel::clear()
{
    beginResetModel();
    content->clear();
    content->append(QString());
    endResetModel();
}

void prizelistmodel::newData()
{
    beginResetModel();
    endResetModel();
    //emit dataChanged(createIndex(0,0),createIndex(content->size()-1,0));
}

bool prizelistmodel::moveRowDown(int row)
{
    if( row < 0 || row >= content->size()-1 ) //don't move lowest row
        return false;
    content->swap(row,row+1);
    emit dataChanged(createIndex(row,0),createIndex(row+1,0));
    return true;
}

bool prizelistmodel::moveRowUp(int row)
{
    if( row < 1 || row >= content->size() ) //don't move hightest row
        return false;
    content->swap(row-1,row);
    emit dataChanged(createIndex(row-1,0),createIndex(row,0));
    return true;
}

bool prizelistmodel::duplicateRow(int row)
{
    if( row < 0 || row >= content->size() )
        return false;
    beginInsertRows(QModelIndex(),row+1,row+1);
    content->insert(row+1,content->at(row));
    endInsertRows();
    return true;
}
