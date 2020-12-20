#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <QObject>
#include "requester.h"

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
    void createRoom(const QString& appChaos, const EventMode& eventMode, const int& maxUsers,const QString& formVersion,const QJsonObject* optionals = nullptr);
    void deleteRoom();
    void addPage(const QJsonObject& pageInfo);
    void deletePage(const QJsonObject& pageInfo);
    void sendEvent(const QJsonObject& message);

    QString getRoomId() const;
    void setRoomId(const QString &value);

    QString getApplicationId() const;
    void setApplicationId(const QString &value);

    QString getApiVersion() const;
    void setApiVersion(const QString &value);

    QString getRoomCode() const;

private:
    Requester* requester;
    QString roomCode;
    QString apiVersion;
    QString applicationId;
    QString roomId;
signals:
    void roomCreated();
    void pageCreated(bool success);
    void sentEventSuc();
    void sentEventFail();
};

#endif // ROOMMANAGER_H
