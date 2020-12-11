#include "questionformmodel.h"
#include "questionformlist.h"

QuestionFormModel::QuestionFormModel(QObject *parent)
    : QAbstractListModel(parent), mFormList(nullptr)
{
}

int QuestionFormModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mFormList)
        return 0;
    return mFormList->items().size();
}

QVariant QuestionFormModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mFormList)
        return QVariant();
    QuestionFormItem item = mFormList->items().at(index.row());
     switch (role) {
         case questionRole:
             return QVariant(item.question);
         case numOfColsRole:
             return QVariant(item.numOfCols);
         case numOfRowsRole:
             return QVariant(item.numOfRows);
         case timerRole:
             return QVariant(item.timer);
         case timerRunningRole:
             return QVariant(item.timerRunning);
        case answersRole:
             return QVariant(item.answers);
     }
    return QVariant();
}

bool QuestionFormModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mFormList)
        return false;
      QuestionFormItem item = mFormList->items().at(index.row());
      switch (role) {
          case questionRole:
              item.question = value.toString();
          break;
          case numOfColsRole:
              item.numOfCols = value.toInt();
          break;
          case numOfRowsRole:
              item.numOfRows = value.toInt();
              break;
          case timerRole:
              item.timer = value.toInt();
              break;
          case timerRunningRole:
              item.timerRunning = value.toBool();
              break;
         case answersRole:
              item.answers = value.toStringList();
              break;
      }
    if (mFormList->setItemAt(index.row(),item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags QuestionFormModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> QuestionFormModel::roleNames() const
{
     QHash<int,QByteArray> names;
     names[numOfRowsRole] = "numOfRows";
     names[numOfColsRole] = "numOfCols";
     names[questionRole] = "question";
     names[timerRole] = "timer";
     names[timerRunningRole] = "timerRunning";
     names[answersRole] = "answers";
     return names;
}

QuestionFormList *QuestionFormModel::list() const
{
    return mFormList;
}

void QuestionFormModel::setList(QuestionFormList *formList)
{
    beginResetModel();
    if (mFormList){
        mFormList->disconnect(this);
    }
    mFormList = formList;
    if (mFormList) {
        connect(mFormList, &QuestionFormList::beginInsertRow, this, [=](int row) {
            beginInsertRows(QModelIndex(), row, row);
            });
        connect(mFormList, &QuestionFormList::beginRemoveRow, this, [=](int row) {
            beginRemoveRows(QModelIndex(), row, row);
            });
        connect(mFormList, &QuestionFormList::insertedRow, this, [=]() {
            endInsertRows();
            });
        connect(mFormList, &QuestionFormList::removedRow, this, [=]() {
            endRemoveRows();
            });
    }
    endResetModel();
}
