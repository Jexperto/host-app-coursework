#include "receiveeventmessage.h"


ReceiveEventMessage::ReceiveEventMessage(QObject *parent) : QObject(parent)
{

}

ReceiveEventMessage::ReceiveEventMessage(QString event, QString sender, QJsonObject data, QString sendTimestamp, QString processingTimestamp, QString completeTimeStamp, QObject *parent):QObject(parent)
{
this->mEvent = event;
    this->mSender = sender;
    this->mData = data;
    this->mSendTimestamp = sendTimestamp;
    this->mProcessingTimestamp = processingTimestamp;
    this->mCompleteTimeStamp = completeTimeStamp;
}


void ReceiveEventMessage::write(QJsonObject &json) const
{
    QJsonObject obj;
    obj["sender"] = this->mSender;
    obj["send-timestamp"] = this->mSendTimestamp;
    obj["processing-timestamp"] = this->mProcessingTimestamp;
    obj["complete-timestamp"] = this->mCompleteTimeStamp;
    QJsonObject messageObject;
    messageObject["event"] = this->mEvent;
    messageObject["data"] = this->mData;
    obj["message"] = messageObject;
    json = obj;
}

void ReceiveEventMessage::read(const QJsonObject &json)
{
    if (json.contains("send-timestamp") && json["send-timestamp"].isString())
        mSendTimestamp = json["send-timestamp"].toString();
    if (json.contains("processing-timestamp") && json["processing-timestamp"].isString())
        mSendTimestamp = json["processing-timestamp"].toString();
    if (json.contains("complete-timestamp") && json["complete-timestamp"].isString())
        mSendTimestamp = json["complete-timestamp"].toString();
    if (json.contains("sender") && json["sender"].isString())
        mSender = (json["sender"].toString());
    if (json.contains("message") && json["message"].isObject()){
        QJsonObject msg = (json["message"].toObject());
        if (msg.contains("event") && msg["event"].isString())
            mEvent = msg["event"].toString();
        if (msg.contains("data") && msg["data"].isObject())
            mData = msg["data"].toObject();
    }

}

//.............................GET/SET................................


QString ReceiveEventMessage::sender() const
{
    return mSender;
}

void ReceiveEventMessage::setSender(const QString &sender)
{
    mSender = sender;
}

QJsonObject ReceiveEventMessage::data() const
{
    return mData;
}

void ReceiveEventMessage::setData(const QJsonObject &data)
{
    mData = data;
}

QString ReceiveEventMessage::getEvent() const
{
    return mEvent;

}

void ReceiveEventMessage::setEvent(const QString &event)
{
    mEvent = event;
}

QString ReceiveEventMessage::completeTimeStamp() const
{
    return mCompleteTimeStamp;
}

void ReceiveEventMessage::setCompleteTimeStamp(const QString &completeTimeStamp)
{
    mCompleteTimeStamp = completeTimeStamp;
}

QString ReceiveEventMessage::processingTimestamp() const
{
    return mProcessingTimestamp;
}

void ReceiveEventMessage::setProcessingTimestamp(const QString &processingTimestamp)
{
    mProcessingTimestamp = processingTimestamp;
}

QString ReceiveEventMessage::sendTimestamp() const
{
    return mSendTimestamp;
}

void ReceiveEventMessage::setSendTimestamp(const QString &sendTimestamp)
{
    mSendTimestamp = sendTimestamp;
}
