#include "requester.h"
const QString Requester::httpTemplate = "http://%1:%2/api/%3";
const QString Requester::httpsTemplate = "https://%1:%2/api/%3";
const QString Requester::testTemplate = "http://%1:%2/test/%3";
const QString Requester::KEY_QNETWORK_REPLY_ERROR = "QNetworkReplyError";
const QString Requester::KEY_CONTENT_NOT_FOUND = "ContentNotFoundError";


Requester::Requester(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void Requester::initRequester(const QString &host, int port, QSslConfiguration *value){
    this->host = host;
    this->port = port;
    sslConfig = value;
//    if (sslConfig != nullptr)
//        pathTemplate = httpsTemplate;
//    else
        pathTemplate = httpTemplate;
}

void Requester::sendRequest(const QString &apiStr,
                            const handleFunc &funcSuccess,
                            const handleFunc &funcError,
                            Requester::Type type,
                            const QVariantMap &data, const QMap<QString,QString>* extraHeaders)
{
    QNetworkRequest request = createRequest(apiStr,extraHeaders);

    QNetworkReply *reply;
    switch (type) {
    case Type::POST: {
        QByteArray postDataByteArray = variantMapToJson(data);
        reply = manager->post(request, postDataByteArray);
        break;
    } case Type::GET: {
        reply = manager->get(request);
        break;
    } case Type::DELET: {
        if (data.isEmpty())
            reply = manager->deleteResource(request);
        else
            reply = sendCustomRequest(manager, request, "DELETE", data);
        break;
    } case Type::PATCH: {
        reply = sendCustomRequest(manager, request, "PATCH", data);
        break;
    } default:
        reply = nullptr;
        Q_ASSERT(false);
    }
    qDebug() << "Request sent";
    connect(reply, &QNetworkReply::finished, this,
            [this, funcSuccess, funcError, reply]() {
        QJsonValue obj = parseReply(reply);
          qDebug() << "Request finished";
        if (onFinishRequest(reply)) {
            if (funcSuccess != nullptr)
                funcSuccess(obj);
        } else {
            if (funcError != nullptr) {
                handleQtNetworkErrors(reply, obj);
                funcError(obj);
            }
        }
        reply->close();
        reply->deleteLater();
    } );

}

void Requester::sendRawRequest(const QString &apiStr,
                            const handleFunc &funcSuccess,
                            const handleFunc &funcError,
                            Requester::Type type,
                            const QByteArray &data, const QMap<QString,QString>* extraHeaders)
{
    QNetworkRequest request = createRequest(apiStr,extraHeaders);
    qDebug() << apiStr;
    QNetworkReply *reply;
    switch (type) {
    case Type::POST: {
        QByteArray postDataByteArray = data;
        reply = manager->post(request, postDataByteArray);
        break;
    } case Type::GET: {
        reply = manager->get(request);
        break;
    } case Type::DELET: {
        if (data.isEmpty())
            reply = manager->deleteResource(request);
        else
            reply = manager->sendCustomRequest(request, "DELETE", data);
        break;
    } case Type::PATCH: {
        reply = manager->sendCustomRequest(request, "PATCH", data);
        break;
    } default:
        reply = nullptr;
        Q_ASSERT(false);
    }
    qDebug() << "Request sent";
    connect(reply, &QNetworkReply::finished, this,
            [this, funcSuccess, funcError, reply]() {
        QJsonValue obj = parseReply(reply);
          qDebug() << "Request finished";
        if (onFinishRequest(reply)) {
            if (funcSuccess != nullptr)
                funcSuccess(obj);
        } else {
            if (funcError != nullptr) {
                handleQtNetworkErrors(reply, obj);
                funcError(obj);
            }
        }
        reply->close();
        reply->deleteLater();
    } );

}

QString Requester::getToken() const
{
    return token;
}

void Requester::setToken(const QString &value)
{
    token = value;
}

int Requester::getLastCode() const
{
    return lastCode;
}




QByteArray Requester::variantMapToJson(QVariantMap data)
{
    QJsonDocument postDataDoc = QJsonDocument::fromVariant(data);
    QByteArray postDataByteArray = postDataDoc.toJson();

    return postDataByteArray;
}

QNetworkRequest Requester::createRequest(const QString &apiStr, const QMap<QString,QString>* extraHeaders)
{
    QNetworkRequest request;
    QString url = pathTemplate.arg(host).arg(port).arg(apiStr);
    request.setUrl(QUrl(url));
    request.setRawHeader("Content-Type","application/json");
    if (extraHeaders!=nullptr){
    QMapIterator<QString, QString> i(*extraHeaders);
    while (i.hasNext()) {
        i.next();
         request.setRawHeader(i.key().toUtf8(),i.value().toUtf8());
    }}
    if(!token.isEmpty())
        request.setRawHeader("chaos",token.toUtf8());
    if (sslConfig != nullptr)
        request.setSslConfiguration(*sslConfig);

    return request;
}

QNetworkReply* Requester::sendCustomRequest(QNetworkAccessManager* manager,
                                            QNetworkRequest &request,
                                            const QString &type,
                                            const QVariantMap &data)
{
    request.setRawHeader("HTTP", type.toUtf8());
    QByteArray postDataByteArray = variantMapToJson(data);
    QBuffer *buff = new QBuffer;
    buff->setData(postDataByteArray);
    buff->open(QIODevice::ReadOnly);
    QNetworkReply* reply =  manager->sendCustomRequest(request, type.toUtf8(), buff);
    buff->setParent(reply);
    return reply;
}

QJsonValue Requester::parseReply(QNetworkReply *reply)
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    auto replyText = reply->readAll();
    jsonDoc = QJsonDocument::fromJson(replyText, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qDebug() << replyText;
        qWarning() << "Json parse error: " << parseError.errorString();
    }else{
        if(jsonDoc.isObject())
            return jsonDoc.object();
        else if (jsonDoc.isArray())
            return jsonDoc.array();
    }
    return jsonObj;
}

bool Requester::onFinishRequest(QNetworkReply *reply)
{
    auto replyError = reply->error();
    if (replyError == QNetworkReply::NoError ) {
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastCode = code;
        if ((code >=200) && (code < 300)) {
            return true;
        }
    }
    return false;
}

void Requester::handleQtNetworkErrors(QNetworkReply *reply, QJsonValue &obj)
{
    auto replyError = reply->error();
    if (!(replyError == QNetworkReply::NoError ||
          replyError == QNetworkReply::ContentNotFoundError ||
          replyError == QNetworkReply::ContentAccessDenied ||
          replyError == QNetworkReply::ProtocolInvalidOperationError
          ) ) {
        qDebug() << reply->error();
        obj.toObject()[KEY_QNETWORK_REPLY_ERROR] = reply->errorString();
    } else if (replyError == QNetworkReply::ContentNotFoundError)
        obj.toObject()[KEY_CONTENT_NOT_FOUND] = reply->errorString();
}
