#include "formlist.h"
#include "QFile"
#include "QJsonObject"
#include <QJsonParseError>
#include <QJsonArray>

FormList::FormList(QObject *parent) : QObject(parent)
{

}

FormList::FormList(const QVector<QuestionFormItem>& list, QObject* parent) : QObject(parent)
{
    this->mObjectsList = list;
}




void FormList::write(QJsonArray& json) const
{
    QJsonArray array;
    foreach (const auto form, mObjectsList)
    {
        QJsonObject formObject;
        formObject["question"] = form.question;
        formObject["numOfRows"] = form.numOfRows;
        formObject["numOfCols"] = form.numOfCols;
        formObject["timer"] = form.timer;
        formObject["timerRunning"] = form.timerRunning;
        QJsonArray ansArray;
        foreach(const auto answer, form.wrongAnswers) {
            ansArray.append(answer);
        }
        formObject["wrongAnswers"] = ansArray;
        formObject["rightAnswer"] = form.rightAnswer;
        array.append(formObject);
    }
    json = array;

}


void FormList::read(const QJsonArray& json)
{
    mObjectsList.clear();
    for (int index = 0; index < json.size(); ++index) {
        QJsonObject formObject = json[index].toObject();
        QuestionFormItem item;

        if (formObject.contains("question") && formObject["question"].isString())
            item.question = formObject["question"].toString();
        if (formObject.contains("numOfRows") && formObject["numOfRows"].isDouble())
            item.numOfRows = formObject["numOfRows"].toInt();
        if (formObject.contains("numOfCols") && formObject["numOfCols"].isDouble())
            item.numOfCols = formObject["numOfCols"].toInt();
        if (formObject.contains("timer") && formObject["timer"].isDouble())
            item.timer = formObject["timer"].toInt();
        if (formObject.contains("timerRunning") && formObject["timerRunning"].isBool())
            item.timerRunning = formObject["timerRunning"].toBool();
        if (formObject.contains("wrongAnswers") && formObject["wrongAnswers"].isArray()) {
            QJsonArray answerArray = formObject["wrongAnswers"].toArray();
            item.wrongAnswers.clear();
            item.wrongAnswers.reserve(answerArray.size());
            for (int i = 0; i < answerArray.size(); ++i) {
                item.wrongAnswers.append(answerArray[i].toString());
            }
        if (formObject.contains("rightAnswer") && formObject["rightAnswer"].isString())
                item.rightAnswer = formObject["rightAnswer"].toString();
            mObjectsList.append(item);
        }

    }

}

QVector<QuestionFormItem> FormList::objectsList() const
{
	return mObjectsList;
}

void FormList::setObjectsList(const QVector<QuestionFormItem>& objects)
{
    mObjectsList = objects;
}



