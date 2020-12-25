#ifndef EVENTMESSAGE_H
#define EVENTMESSAGE_H

#include <QObject>
#include <messagedata.h>

#define FORM_LOAD "formLoad"
class SendEventMessage : QObject
{
    Q_OBJECT
public:
    explicit SendEventMessage(QObject *parent = nullptr);
    explicit SendEventMessage(QString timeStamp, QString event,QJsonObject data, QString receiver, QObject *parent = nullptr);
    QString getSendTimestamp() const;
    void setSendTimestamp(QString sendTimestamp);

    QString getEvent() const;
    void setEvent(const QString &event);

    QJsonObject data() const;
    void setData(const QJsonObject &data);

    QString receiver() const;
    void setReceiver(const QString &receiver);

    void write(QJsonObject&json) const;
    void read(const QJsonObject& json);

private:
    QString mSendTimestamp; //Basic ISO date
    QString mEvent;
    QString mReceiver;
    QJsonObject mData;
};

#endif // EVENTMESSAGE_H
