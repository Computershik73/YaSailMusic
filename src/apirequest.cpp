#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QSettings>
#include "apirequest.h"

ApiRequest::ApiRequest(QObject *parent) : QObject(parent) {
    _manager = new QNetworkAccessManager(this);
}

ApiRequest::~ApiRequest() {
    delete _manager;
}

void ApiRequest::makeApiGetRequest(const QString &method, const QUrlQuery &q) {
    QSettings settings;
    QString accessToken = settings.value("accessToken").toString();

    QUrlQuery query = q;
    QUrl url(API_URL + method);
    url.setQuery(query.query());
    _manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "OAuth "+accessToken.toUtf8());
    connect(_manager, &QNetworkAccessManager::finished, this, &ApiRequest::dataReady, Qt::QueuedConnection);
    _manager->get(request);
}

void ApiRequest::dataReady(QNetworkReply *reply) {
    QByteArray data = reply->readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(data);
    QJsonObject jObj = jDoc.object();
    if (jObj.contains("result")) {
        QJsonValue jVal = jObj.value("result");
        QString strFromObj = QJsonDocument(jObj).toJson(QJsonDocument::Compact).toStdString().c_str();
        qDebug() << "Reply: " << strFromObj;
        emit gotResponse(jVal);
    } else {
        qDebug() << "Error in API request!";
        qDebug() << "+++++++++++++++++++";
        qDebug() << data;

    }
}
