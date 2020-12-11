#include "questionformlist.h"

QuestionFormList::QuestionFormList(QObject *parent) : QObject(parent)
{
    //mItems.append(QuestionFormItem({"Test question",2,2,20,true,QStringList({"1","2","3","4"})}));
    //mItems.append(QuestionFormItem({"Test question",2,2,20,true,QStringList({"1","2","3","4"})}));
    //mItems.append(QuestionFormItem({"Test question",2,2,20,true,QStringList({"1","2","3","4"})}));
}

QVector<QuestionFormItem> QuestionFormList::items() const
{
    return mItems;
}

bool QuestionFormList::setItemAt(int index, const QuestionFormItem &status)
{
    if (index < 0 || index > mItems.size() - 1)
        return false;
    emit beginInsertRow(index);
    mItems[index] = status;
    emit insertedRow();
    return true;

}

void QuestionFormList::appendItem(const QuestionFormItem &status)
{
    beginInsertRow(mItems.size());
    mItems.append(status);
    insertedRow();
}

void QuestionFormList::removeItem(int index)
{
    if (index < 0 || index > mItems.size() - 1)
        return;
    emit beginRemoveRow(index);
    mItems.removeAt(index);
    emit removedRow();}

QuestionFormItem QuestionFormList::getItem(int index) const
{
    if (index < 0 || index > mItems.size() - 1)
        return QuestionFormItem();
    return mItems[index];
}
