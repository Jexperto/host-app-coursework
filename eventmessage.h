#ifndef EVENTMESSAGE_H
#define EVENTMESSAGE_H

#include <QObject>
#include <messagedata.h>
class EventMessage : QObject
{
    Q_OBJECT
public:
    explicit EventMessage(QObject *parent = nullptr);
    explicit EventMessage(QString timeStamp, QString event,MessageData data, QString receiver, QObject *parent = nullptr);
    QString sendTimestamp() const;
    void setSendTimestamp(long sendTimestamp);

    QString getEvent() const;
    void setEvent(const QString &event);

    MessageData data() const;
    void setData(const MessageData &data);

    QString receiver() const;
    void setReceiver(const QString &receiver);

    void write(QJsonObject&json) const;
    void read(const QJsonObject& json);

private:
    QString mSendTimestamp; //Basic ISO date
    QString mEvent;
    QString mReceiver;
    MessageData mData;
};

#endif // EVENTMESSAGE_H
