#include "duckplacemodel.h"

#include <QStringList>

duckplacemodel::duckplacemodel(QObject *parent) : QAbstractListModel(parent)
{
    content = new QList<int>;
}

QVariant duckplacemodel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || index.row() > content->size()-1 || (role != Qt::DisplayRole && role != Qt::EditRole) )
        return QVariant();
    /*if( index.row() == content->size() )
        return QString();*/
    int temp = content->at(index.row());
    if( temp > 0 )
        return temp;
    else
        return QString();
}

bool duckplacemodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( index.row() > content->size() || value.type() != QVariant::Int || role != Qt::EditRole )
        return false;
    bool ok;
    int temp = value.toInt(&ok);
    if( ok && temp > 0 && temp < 32768 && !content->contains(temp) ) {
        if( index.row() >= content->size() )
            ok = insertRows(index.row()-1,1,QModelIndex(),temp);
        else
            (*content)[index.row()] = temp;
        if( ok )
            dataChanged(index,index);
        return ok;
    }
    else
        return false;
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

bool duckplacemodel::insertRows(int row, int count, const QModelIndex &parent, int value)
{
    if( row >= content->size() || count == 0 )
        return false;
    if( row < 0 )
        row = 0;
    beginInsertRows(parent,row+1,row+count);
    for(int num = 0; num < count; num++)
        content->insert(row+1,value);
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
    return true;
}

void duckplacemodel::clear()
{
    beginResetModel();
    content->clear();
    endResetModel();
}

void duckplacemodel::newData()
{
    beginResetModel();
    endResetModel();
}

bool duckplacemodel::appendPlace(int duck) //returns true on success, false if duck is already scanned
{
    if( content->contains(duck) )
        return false;
    return insertRows(content->size()-1,1,QModelIndex(),duck);
}
