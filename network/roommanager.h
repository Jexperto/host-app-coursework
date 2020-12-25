#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <QObject>
#include "requester.h"
class ReceiveEventMessage;

class RoomManager : public QObject
{
    Q_OBJECT
public:
    enum class EventMode{
        WEBSOCKET,
        POOL
    };
    static const QString ROOM_ID_HEADER;
    static const QString APPLICATION_ID_HEADER;
    static const QString API_VERSION_HEADER;
    RoomManager(QString host, int port, QString apiVersion, QString applicationId, QObject *parent = nullptr);
    ~RoomManager();
    void createRoom(const QString& appChaos, const EventMode& eventMode, const int& maxUsers,const QString& formVersion,const QJsonObject* optionals = nullptr);
    void deleteRoom();
    void addPage(const QJsonObject& pageInfo);
    void deletePage(const QJsonObject& pageInfo);
    void sendEvent(const QJsonObject& message);
    void addResource(const QString& query,const QString& data);
    void deleteResource(const QString& query);

    QString getRoomId() const;
    void setRoomId(const QString &value);

    QString getApplicationId() const;
    void setApplicationId(const QString &value);

    QString getApiVersion() const;
    void setApiVersion(const QString &value);

    QString getRoomCode() const;

    void receiveEvent();
    void getUsers();
private:
    Requester* requester;
    QString roomCode;
    QString apiVersion;
    QString applicationId;
    QString roomId;
    QMap<QString,QString>* users;
signals:
    void resourceSetSuccessfully(QString name);
    void resourceSetFailed(QString name);
    void resourceDeletedSuccessfully(QString name);
    void resourceDeleteFailed(QString name);
    void noMoreEvents();
    void receivedEvent(ReceiveEventMessage* eventMessage);
    void receivedUsers(QMap<QString,QString>* users);
    void roomCreated();
    void roomCreateFailed();
    void pageCreated(bool success);
    void sentEventSuc();
    void sentEventFail();
};

#endif // ROOMMANAGER_H
