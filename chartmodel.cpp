#include "chartmodel.h"

ChartModel::ChartModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ChartModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return items.size();
}

QVariant ChartModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    ResultUnit item = items.at(index.row());
     switch (role) {
         case nameRole:
             return QVariant(item.name);
         case resultRole:
            return QVariant(item.result);
        case countRole:
            return QVariant(rowCount());
     }
     return QVariant();
}

void ChartModel::append(QString name, double result)
{
    beginInsertRows(QModelIndex(), items.size(), items.size());
        items.append(ResultUnit{name,result});
        endInsertRows();
}

void ChartModel::clear()
{
    items.clear();
}

QHash<int, QByteArray> ChartModel::roleNames() const
{

         QHash<int,QByteArray> names;
         names[nameRole] = "name";
         names[resultRole] = "result";
         names[countRole] = "count";
         return names;

}

QList<ChartModel::ResultUnit> ChartModel::getItems()
{
    return items;
}
