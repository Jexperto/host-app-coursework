#include "sendeventmessage.h"
#include <QJsonObject>
#include <datamanager.h>

SendEventMessage::SendEventMessage(QObject *parent): QObject(parent)
{

}

SendEventMessage::SendEventMessage(QString timeStamp, QString event, QJsonObject data, QString receiver,QObject *parent): QObject(parent)
{
    this->mSendTimestamp = timeStamp;
    this->mEvent = event;
    this->mData = data;
    this->mReceiver = receiver;
}

void SendEventMessage::write(QJsonObject &json) const
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

void SendEventMessage::read(const QJsonObject &json)
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

QString SendEventMessage::getSendTimestamp() const
{
    return mSendTimestamp;
}

void SendEventMessage::setSendTimestamp(QString sendTimestamp)
{
    mSendTimestamp = sendTimestamp;
}

QString SendEventMessage::getEvent() const
{
    return mEvent;
}

void SendEventMessage::setEvent(const QString &event)
{
    mEvent = event;
}

QJsonObject SendEventMessage::data() const
{
    return mData;
}

void SendEventMessage::setData(const QJsonObject &data)
{
    mData = data;
}

QString SendEventMessage::receiver() const
{
    return mReceiver;
}

void SendEventMessage::setReceiver(const QString &receiver)
{
    mReceiver = receiver;
}

