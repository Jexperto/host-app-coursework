#include "questionformlist.h"
#include "QJsonObject"
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

bool QuestionFormList::setItemAt(int index, const QuestionFormItem &item)
{
    if (index < 0 || index > mItems.size() - 1)
        return false;
    emit beginInsertRow(index);
    mItems[index] = item;
    emit insertedRow();
    return true;

}

void QuestionFormList::appendItem(const QuestionFormItem &item)
{
    beginInsertRow(mItems.size());
    mItems.append(item);
    insertedRow();
}

void QuestionFormList::removeItem(int index)
{
    if (index < 0 || index > mItems.size() - 1)
        return;
    emit beginRemoveRow(index);
    mItems.removeAt(index);
    emit removedRow();}

int QuestionFormList::convertItemToEventDataJson(int index, QJsonObject& data)
{
        QJsonObject formObject;
        formObject["question"] = this->mItems.at(index).question;
        if (this->mItems.at(index).answers.length()>1){
            int count = 1;
            QString buttonTemplate = "button%1";
            foreach(const auto answer, this->mItems.at(index).answers) {
                formObject[buttonTemplate.arg(count)] = answer;
                count++;
            }
         }
        data = formObject;
        return this->mItems.at(index).answers.length();
}

QuestionFormItem QuestionFormList::getItem(int index) const
{
    if (index < 0 || index > mItems.size() - 1)
        return QuestionFormItem();
    return mItems[index];
}

void QuestionFormList::clear()
{
    mItems.clear();
}

int QuestionFormList::size()
{
    return mItems.size();
}
