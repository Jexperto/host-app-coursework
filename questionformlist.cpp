#include "questionformlist.h"
#include <QJsonObject>
#include <QRandomGenerator>
QuestionFormList::QuestionFormList(QObject *parent) : QObject(parent)
{
    this->shuffleSeed = 0;
}

QuestionFormList::QuestionFormList(int shuffleSeed, QObject *parent) : QObject(parent)
{
    this->shuffleSeed = shuffleSeed;
}

int QuestionFormList::getShuffleSeed() const
{
    return shuffleSeed;
}

void QuestionFormList::setShuffleSeed(int value)
{
    shuffleSeed = value;
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
{       //TODO: shuffle
        QJsonObject formObject;
        formObject["question"] = this->mItems.at(index).question;
        QStringList temp(this->mItems.at(index).wrongAnswers);
        if (this->mItems.at(index).wrongAnswers.length()>0){
            QStringList temp(this->mItems.at(index).wrongAnswers);
            QRandomGenerator rng(shuffleSeed);
            temp.insert(rng.bounded(this->mItems.at(index).wrongAnswers.length()),this->mItems.at(index).rightAnswer);

            int count = 1;
            QString buttonTemplate = "button%1";
            foreach(const auto answer, temp) {
                formObject[buttonTemplate.arg(count)] = answer;
                count++;
            }
         }

        data = formObject;
        return (this->mItems.at(index).wrongAnswers.length()+1);
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
