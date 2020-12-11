#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

#include <QObject>
#include <QJsonObject>

class MessageData
{
public:
    explicit MessageData();
    explicit MessageData(QString formName, QString user, QJsonObject dataByID, QJsonObject dataByClass);
    explicit MessageData(QString formName, QString user, QJsonObject dataByID);

    void write(QJsonObject&json) const;
    void read(const QJsonObject& json);

    MessageData(QString formName, QString user);
private:
    QString mFormName;
    QString mUser;
    QJsonObject mDataByID;
    QJsonObject mDataByClass;
};

#endif // MESSAGEDATA_H
