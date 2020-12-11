#include "messagedata.h"
#include <QJsonObject>

MessageData::MessageData()
{

}

MessageData::MessageData(QString formName, QString user, QJsonObject dataByID, QJsonObject dataByClass)
{
    this->mFormName = formName;
    this->mUser = user;
    this->mDataByID = dataByID;
    this->mDataByClass = dataByClass;

}

MessageData::MessageData(QString formName, QString user, QJsonObject dataByID)
{
    this->mFormName = formName;
    this->mUser = user;
    this->mDataByID = dataByID;
}

MessageData::MessageData(QString formName, QString user)
{
    this->mFormName = formName;
    this->mUser = user;
}



void MessageData::write(QJsonObject& json) const
{
        QJsonObject object;
        object["formName"] = mFormName;
        object["user"] = mUser;
        if (!mDataByID.empty())
            object["dataById"] = mDataByID;
        else object["dataById"] = QJsonValue::Null;
        if (!mDataByClass.empty())
            object["dataByClass"] = mDataByClass;
        else object["dataByClass"] = QJsonValue::Null;
        json = object;
}


void MessageData::read(const QJsonObject& json)
{
        if (json.contains("formName") && json["formName"].isString())
            mFormName = json["formName"].toString();
        if (json.contains("user") && json["user"].isString())
            mUser = json["user"].toString();
        if (json.contains("dataById") && json["dataById"].isObject())
            mDataByID = json["dataById"].toObject();
        if (json.contains("dataByClass") && json["dataByClass"].isObject())
            mDataByClass = json["dataByClass"].toObject();
}
