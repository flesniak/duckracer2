#include "duckplacemodel.h"

#include <QStringList>

duckplacemodel::duckplacemodel(QObject *parent) : QAbstractListModel(parent)
{
    content = new QList<int>;
    content->append(-1);
}

QVariant duckplacemodel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || index.row() > content->size()-1 || (role != Qt::DisplayRole && role != Qt::EditRole) )
        return QVariant();
    int temp = content->at(index.row());
    if( temp < 0 )
        return QString();
    else
        return temp;
}

bool duckplacemodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( index.row() > content->size()-1 || value.type() != QVariant::String || role != Qt::EditRole )
        return false;
    bool ok;
    int temp = value.toInt(&ok);
    if( ok && temp > 0 && temp < 32767 )
        (*content)[index.row()] = temp;
    emit dataChanged(index,index);
    return true;
}

int duckplacemodel::rowCount(const QModelIndex &) const
{
    return content->size();
}

Qt::ItemFlags duckplacemodel::flags(const QModelIndex &) const
{
    return  Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant duckplacemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole )
        return QVariant();
    if( orientation == Qt::Horizontal )
        return tr("Enten-Nummer");
    else //Qt::Vertical
        return section+1;
}

bool duckplacemodel::insertRows(int row, int count, const QModelIndex &parent)
{
    if( row >= content->size() || count == 0 )
        return false;
    if( row < 0 )
        row = 0;
    beginInsertRows(parent,row+1,row+count);
    for(int num = 0; num < count; num++)
        content->insert(row+1,-1);
    endInsertRows();
    return true;
}

bool duckplacemodel::removeRows(int row, int count, const QModelIndex &parent)
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

void duckplacemodel::clear()
{
    beginResetModel();
    content->clear();
    content->append(-1);
    endResetModel();
}

void duckplacemodel::newData()
{
    beginResetModel();
    endResetModel();
    //emit dataChanged(createIndex(0,0),createIndex(content->size()-1,0));
}

void duckplacemodel::appendPlace(int duck)
{
    QModelIndex index = createIndex(content->size()-1,0);
    (*content)[content->size()-1] = duck;
    emit dataChanged(index,index);
    insertRows(content->size(),1,QModelIndex());
}
