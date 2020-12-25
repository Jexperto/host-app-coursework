#include "roommanager.h"
#include "receiveeventmessage.h"
#include <QDebug>

RoomManager::RoomManager(QString host, int port, QString apiVersion, QString applicationId, QObject *parent): QObject(parent){
    this->requester = new Requester(this);
    this->roomCode = "-1";
    this->apiVersion = apiVersion;
    this->applicationId = applicationId;
    this->roomId = "-1";
    this->requester->initRequester(host,port,nullptr);
    this->users = new QMap<QString,QString>;
}

RoomManager::~RoomManager()
{
    delete users;
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
       requester->sendRequest("rooms/",[this](const QJsonValue& valobj)->void{
            QJsonObject obj = valobj.toObject();
           if (obj.contains("room-id") && obj["room-id"].isString())
               this->roomId =(obj["room-id"].toString());

           if (obj.contains("code") && obj["code"].isString())
               roomCode = obj["code"].toString();

           if (obj.contains("chaos") && obj["chaos"].isString())
               requester->setToken(obj["chaos"].toString());
           emit roomCreated();
           qDebug() << "Success! " << obj;
       },

       [this](const QJsonValue& valobj)->void{
           QJsonObject obj = valobj.toObject();
           if ((obj.contains("description") && obj["description"].isString()))
                qDebug() << "Fail! " << obj["description"].toString();
           else
                qDebug() << "Fail! " << "No comments...";
           emit roomCreateFailed();

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
    requester->sendRequest("rooms/",[this](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        this->roomId ="-1";
        this->roomCode = "-1";
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
    },
    [](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fail! " << obj["description"].toString();
        else
             qDebug() << "Fail! " << "No comments...";

    },
    Requester::Type::DELET,QVariantMap(),&extraHeader);
}

void RoomManager::addPage(const QJsonObject &pageInfo)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/pages/",[=](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString())){
             qDebug() << "Success! " << obj["description"].toString();
             qDebug() << "Success! " << obj;}
        else
             qDebug() << "Success! " << "No comments...";
        emit pageCreated(true);
    },
    [=](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString())){
             qDebug() << "Fail! " << obj["description"].toString();
             qDebug() << "Fail! " << obj;
        }
        else
             qDebug() << "Fail! " << "No comments...";
       emit pageCreated(false);
    },
    Requester::Type::POST, pageInfo.toVariantMap(),&extraHeader);
}

void RoomManager::deletePage(const QJsonObject &pageInfo)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/pages/",[](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
    },
    [](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fail! " << obj["description"].toString();
        else
             qDebug() << "Fail! " << "No comments...";

    },
    Requester::Type::DELET, pageInfo.toVariantMap(),&extraHeader);
}

void RoomManager::sendEvent(const QJsonObject &message)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/events/pool/",[this](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
        emit sentEventSuc();
    },
    [this](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fail! " << obj["description"].toString();
        else
             qDebug() << "Fail! " << "No comments...";
        emit sentEventFail();

    },
    Requester::Type::POST, message.toVariantMap(),&extraHeader);
}

void RoomManager::addResource(const QString &query, const QString &data)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    QString url = "rooms/resources/?name=%1";
    requester->sendRawRequest(url.arg(query),[this,query](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
        emit resourceSetSuccessfully(query);
    },
    [this,&query](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fail! " << obj["description"].toString();
        else
             qDebug() << "Fail! " << "No comments...";
        emit resourceSetFailed(query);

    },
    Requester::Type::POST, data.toUtf8(),&extraHeader);
}

void RoomManager::deleteResource(const QString &query)
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    QString url = "rooms/resources/?name=%1";
    QJsonObject obj;
    obj["name"] = query;
    requester->sendRequest(url.arg(query),[this,query](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Success! " << obj["description"].toString();
        else
             qDebug() << "Success! " << "No comments...";
        emit resourceDeletedSuccessfully(query);
    },
    [this,&query](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fail! " << obj["description"].toString();
        else
             qDebug() << "Fail! " << "No comments...";
        emit resourceDeleteFailed(query);

    },
    Requester::Type::DELET, obj.toVariantMap(),&extraHeader);
}

void RoomManager::receiveEvent()
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/events/pool/",[this](const QJsonValue& valobj)->void{
     if (requester->getLastCode() == 204){
          qDebug() << "Success! No more events!";
          emit noMoreEvents();
         return;
     }

     QJsonObject obj = valobj.toObject();
     ReceiveEventMessage* msg = new ReceiveEventMessage(this);
     msg->read(obj);
     emit receivedEvent(msg);

    },
    [this](const QJsonValue& valobj)->void{
        QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fail! " << obj["description"].toString();
        else
             qDebug() << "Fail! " << "No comments...";
        emit sentEventFail();

    },
    Requester::Type::GET, QVariantMap(),&extraHeader);
}

void RoomManager::getUsers() //clean users pointer
{
    QMap<QString,QString> extraHeader;
    extraHeader.insert("room-id",this->roomId);
    extraHeader.insert("api-version",this->apiVersion);
    requester->sendRequest("rooms/users/",[this](const QJsonValue& valobj)->void{
     QJsonArray obj = valobj.toArray();
     delete users;
     users = new QMap<QString,QString>;
     foreach (const auto user, obj){
         QJsonObject userobj = user.toObject();
            if ((userobj.contains("uuid") && userobj["uuid"].isString()) && (userobj.contains("name") && userobj["name"].isString()))
              users->insert(userobj["uuid"].toString(),userobj["name"].toString());
     }
     emit receivedUsers(users);

    },
    [this](const QJsonValue& valobj)->void{
         QJsonObject obj = valobj.toObject();
        if ((obj.contains("description") && obj["description"].isString()))
             qDebug() << "Fail! " << obj["description"].toString();
        else
             qDebug() << "Fail! " << "No comments...";
        emit sentEventFail();

    },
    Requester::Type::GET, QVariantMap(),&extraHeader);
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

