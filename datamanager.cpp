#include "datamanager.h"

DataManager::DataManager(){}

template<>
void DataManager::read(FormLoadStruct *structure, const QJsonObject *json)
{
    if (json->contains("formName") && (*json)["formName"].isString())
        structure->mFormName = (*json)["formName"].toString();
//    if (json->contains("user") && (*json)["user"].isString())
//        structure->mUser = (*json)["user"].toString();
    if (json->contains("dataById") && (*json)["dataById"].isObject())
        structure->mDataByID = (*json)["dataById"].toObject();
    if (json->contains("dataByClass") && (*json)["dataByClass"].isObject())
        structure->mDataByClass = (*json)["dataByClass"].toObject();
}
template<>
void DataManager::write(const FormLoadStruct *structure, QJsonObject *json)
{
    QJsonObject object;
    object["formName"] = structure->mFormName;
  //  object["user"] = structure->mUser;
    if (!structure->mDataByID.empty())
        object["dataById"] = structure->mDataByID;
    else object["dataById"] = QJsonValue::Null;
    if (!structure->mDataByClass.empty())
        object["dataByClass"] = structure->mDataByClass;
    else object["dataByClass"] = QJsonValue::Null;
    *json = object;
}

