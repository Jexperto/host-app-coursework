#include "datamanager.h"




DataManager::DataManager()
{

}

template<typename T>
void DataManager::write(DataManager::EventTypes eventType, T structure, QJsonObject* json)
{
    writeFormLoad(structure, json);
}

void DataManager::writeFormLoad(FormLoadStruct structure,QJsonObject* json) const
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
