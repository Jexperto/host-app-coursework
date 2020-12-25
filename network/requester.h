#ifndef REQUESTER_H
#define REQUESTER_H

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <functional>

class Requester : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void(const QJsonValue &)> handleFunc;
    typedef std::function<void()> finishFunc;

    static const QString KEY_QNETWORK_REPLY_ERROR;
    static const QString KEY_CONTENT_NOT_FOUND;

    enum class Type {
        POST,
        GET,
        PATCH,
        DELET
    };

    explicit Requester(QObject *parent = 0);

    void initRequester(const QString &host, int port, QSslConfiguration *value);


    void sendRequest(const QString &apiStr,
                     const handleFunc &funcSuccess,
                     const handleFunc &funcError,
                     Type type = Type::GET,
                     const QVariantMap &data = QVariantMap(), const QMap<QString,QString>* extraHeaders = nullptr);

    QString getToken() const;
    void setToken(const QString &value);

    int getLastCode() const;

    void sendRawRequest(const QString &apiStr, const handleFunc &funcSuccess, const handleFunc &funcError, Requester::Type type, const QByteArray &data, const QMap<QString, QString> *extraHeaders);
private:
    static const QString httpTemplate;
    static const QString httpsTemplate;
    static const QString testTemplate;
    int lastCode;
    QString host;
    int port;
    QString token;
    QSslConfiguration *sslConfig;

    QString pathTemplate;

    QByteArray variantMapToJson(QVariantMap data);

    QNetworkRequest createRequest(const QString &apiStr, const QMap<QString,QString>* extraHeaders = nullptr);

    QNetworkReply *sendCustomRequest(QNetworkAccessManager *manager,
                                     QNetworkRequest &request,
                                     const QString &type,
                                     const QVariantMap &data);

    QJsonValue parseReply(QNetworkReply *reply);

    bool onFinishRequest(QNetworkReply *reply);

    void handleQtNetworkErrors(QNetworkReply *reply, QJsonValue &obj);
    QNetworkAccessManager *manager;

signals:
    void networkError();


public slots:
};

#endif // REQUESTER_H
