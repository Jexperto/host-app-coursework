#include "eventmessage.h"
#include <QJsonObject>
#include <datamanager.h>

EventMessage::EventMessage(QObject *parent): QObject(parent)
{

}

EventMessage::EventMessage(QString timeStamp, QString event, QJsonObject data, QString receiver,QObject *parent): QObject(parent)
{
    this->mSendTimestamp = timeStamp;
    this->mEvent = event;
    this->mData = data;
    this->mReceiver = receiver;
}

void EventMessage::write(QJsonObject &json) const
{
    QJsonObject obj;
    obj["receiver"] = this->mReceiver;
    obj["send-timestamp"] = this->mSendTimestamp;
    QJsonObject messageObject;
    messageObject["event"] = this->mEvent; 
    messageObject["data"] = this->mData;
    obj["message"] = messageObject;
    json = obj;
}

void EventMessage::read(const QJsonObject &json)
{
    if (json.contains("send-timestamp") && json["send-timestamp"].isString())
        mSendTimestamp = json["send-timestamp"].toString();
    if (json.contains("receiver") && json["receiver"].isString())
        mReceiver = (json["receiver"].toString());
    if (json.contains("message") && json["message"].isObject()){
        QJsonObject msg = (json["message"].toObject());
        if (msg.contains("event") && msg["event"].isString())
            mEvent = msg["event"].toString();
        if (msg.contains("data") && msg["data"].isObject())
            mData = msg["data"].toObject();
    }

}

QString EventMessage::getSendTimestamp() const
{
    return mSendTimestamp;
}

void EventMessage::setSendTimestamp(QString sendTimestamp)
{
    mSendTimestamp = sendTimestamp;
}

QString EventMessage::getEvent() const
{
    return mEvent;
}

void EventMessage::setEvent(const QString &event)
{
    mEvent = event;
}

QJsonObject EventMessage::data() const
{
    return mData;
}

void EventMessage::setData(const QJsonObject &data)
{
    mData = data;
}

QString EventMessage::receiver() const
{
    return mReceiver;
}

void EventMessage::setReceiver(const QString &receiver)
{
    mReceiver = receiver;
}

