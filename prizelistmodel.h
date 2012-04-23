#ifndef PRIZELISTMODEL_H
#define PRIZELISTMODEL_H

#include <QAbstractListModel>

class QStringList;

class prizelistmodel : public QAbstractListModel
{
    Q_OBJECT
//friend class wdgPrizes;

public:
    explicit prizelistmodel(QObject *parent = 0);

    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void clear();
    void newData();
    bool moveRowUp(int row);
    bool moveRowDown(int row);
    bool duplicateRow(int row);
    QStringList* getContent() { return content; };

private:
    QStringList* content;
};

#endif // PRIZELISTMODEL_H
