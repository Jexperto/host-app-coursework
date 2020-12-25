#ifndef RECEIVEEVENTMESSAGE_H
#define RECEIVEEVENTMESSAGE_H

#include <QObject>
#include <QJsonObject>

class ReceiveEventMessage : public QObject
{
    Q_OBJECT

    QString mSendTimestamp;
    QString mProcessingTimestamp;
    QString mCompleteTimeStamp;
    QString mEvent;
    QString mSender;
    QJsonObject mData;

public:

    explicit ReceiveEventMessage(QObject *parent = nullptr);
     ReceiveEventMessage(QString event,QString sender, QJsonObject data, QString sendTimestamp, QString processingTimestamp, QString completeTimeStamp,QObject *parent = nullptr);

    void write(QJsonObject &json) const;
    void read(const QJsonObject &json);

    QJsonObject data() const;
    void setData(const QJsonObject &data);

    QString getEvent() const;
    void setEvent(const QString &event);

    QString completeTimeStamp() const;
    void setCompleteTimeStamp(const QString &completeTimeStamp);

    QString processingTimestamp() const;
    void setProcessingTimestamp(const QString &processingTimestamp);

    QString sendTimestamp() const;
    void setSendTimestamp(const QString &sendTimestamp);

    QString sender() const;
    void setSender(const QString &sender);

signals:

};

#endif // RECEIVEEVENTMESSAGE_H
