#include "roommanager.h"
#include "eventmessage.h"
#include <QDebug>

RoomManager::RoomManager(QString host, int port, QString apiVersion, QString applicationId, QObject *parent): QObject(parent){
    this->requester = new Requester(this);
    this->roomCode = "-1";
    this->apiVersion = apiVersion;
    this->applicationId = applicationId;
    this->roomId = "-1";
    this->requester->initRequester(host,port,nullptr);
}


void RoomManager::createRoom(const QString& appChaos ,const EventMode &eventMode, const int &maxUsers, const QString &formVersion, const QJsonObject* optionals)
{

    QJsonObject object;
    object["event-mode"] = eventMode == EventMode::WEBSOCKET ? "websocket" : "pool";
    object["max-size"] = maxUsers;
    object["form-set-version"] = formVersion;
    if (optionals!=nullptr){
       auto i = optionals->begin();
       while (i != optionals->end()){
           object[i.key()] = i.value();
           i++;
       }
}
    qDebug() << object;
       QMap<QString,QString> extraHeader;
       extraHeader.insert("application-id",this->applicationId);
       extraHeader.insert("api-version",this->apiVersion);
       extraHeader.insert("chaos",appChaos);
       requester->sendRequest("rooms/",[this](const QJsonObject& obj)->void{

           if (obj.contains("room-id") && obj["room-id"].isString())
               this->roomId =(obj["room-id"].toString());

           if (obj.contains("code") && obj["code"].isString())
               roomCode = obj["code"].toString();

           if (obj.contains("chaos") && obj["chaos"].isString())
               requester->setToken(obj["chaos"].toString());
           emit roomCreated();
           qDebug() << "Success! " << obj;
       },

       [](const QJsonObject& obj)->void{
           if ((obj.contains("description") && obj["description"].isString()))
                qDebug() << "Fuck me! " << obj["description"].toString();
           else
                qDebug() << "Fuck me! " << "No comments...";

       },
       Requester::Type::POST,object.toVariantMap(),&extraHeader);
}

void RoomManager::deleteRoom()
{
    if (this->getRoomId()=="-1"){
        qDebug() << "Room ID is -1";
        return;
    }
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/",[this](const QJsonObject& obj)->void{
        this->roomId ="-1";
        this->roomCode = "-1";
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
    },
    [](const QJsonObject& obj)->void{
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fuck me! " << obj["description"].toString();
        else
             qDebug() << "Fuck me! " << "No comments...";

    },
    Requester::Type::DELET,QVariantMap(),&extraHeader);
}

void RoomManager::addPage(const QJsonObject &pageInfo)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/pages/",[=](const QJsonObject& obj)->void{
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
        emit pageCreated(true);
    },
    [=](const QJsonObject& obj)->void{
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fuck me! " << obj["description"].toString();
        else
             qDebug() << "Fuck me! " << "No comments...";
       emit pageCreated(false);
    },
    Requester::Type::POST, pageInfo.toVariantMap(),&extraHeader);
}

void RoomManager::deletePage(const QJsonObject &pageInfo)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/pages/",[](const QJsonObject& obj)->void{
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
    },
    [](const QJsonObject& obj)->void{
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fuck me! " << obj["description"].toString();
        else
             qDebug() << "Fuck me! " << "No comments...";

    },
    Requester::Type::DELET, pageInfo.toVariantMap(),&extraHeader);
}

void RoomManager::sendEvent(const QJsonObject &message)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/events/pool/",[this](const QJsonObject& obj)->void{
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
        emit sentEventSuc();
    },
    [this](const QJsonObject& obj)->void{
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fuck me! " << obj["description"].toString();
        else
             qDebug() << "Fuck me! " << "No comments...";
        emit sentEventFail();

    },
    Requester::Type::POST, message.toVariantMap(),&extraHeader);
}

QString RoomManager::getRoomId() const
{
    return roomId;
}

void RoomManager::setRoomId(const QString &value)
{
    roomId = value;
}

QString RoomManager::getApplicationId() const
{
    return applicationId;
}

void RoomManager::setApplicationId(const QString &value)
{
    applicationId = value;
}

QString RoomManager::getApiVersion() const
{
    return apiVersion;
}

void RoomManager::setApiVersion(const QString &value)
{
    apiVersion = value;
}

QString RoomManager::getRoomCode() const
{
    return roomCode;
}

