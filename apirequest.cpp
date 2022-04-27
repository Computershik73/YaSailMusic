
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QSettings>
#include "apirequest.h"

const char TASK_TYPE_KEY[] = "taskType";

ApiRequest::ApiRequest(QObject *parent) : QObject(parent) {
    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

ApiRequest::~ApiRequest() {
    delete _manager;
}

void ApiRequest::makeApiGetRequest(const QString &method, const QUrlQuery &q, TaskType type) {
    QSettings settings;
    QString accessToken = settings.value("accessToken").toString();

    QUrlQuery query = q;
    QUrl url(API_URL + method);
    url.setQuery(query.query());
    qDebug() << "URL: " << url.toString();
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "OAuth "+accessToken.toUtf8());
    request.setRawHeader("X-Yandex-Music-Content-Type", "adult");
    request.setRawHeader("X-Yandex-Music-Device", "os=iOS; os_version=12.2; manufacturer=Apple; model=iPhone8,4; clid=0; device_id=3713C13E-7B0B-4B33-8031-0BD00EC5DDEA; uuid=70db875aae45466dbe932244c10a62c1");
    request.setRawHeader("User-Agent", "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
    request.setRawHeader("X-Yandex-Music-Client", "YandexMusic/519");
    request.setRawHeader("Ya-Client-User-Agent", "Maple/519 (iPhone; iOS 12.2; Scale/2.00)");
    request.setRawHeader("Cookie", "_yasc=jIPYMjq4BwfgL8w1zfZDfFTNIxqinePslHoDbyatIhhu7A==");
    QNetworkReply *reply = _manager->get(request);
    reply->setProperty(TASK_TYPE_KEY, type);
}

void ApiRequest::makePostRequest(const QUrl &u, const QUrlQuery &query, QHttpMultiPart *multipart, TaskType type) {
    QUrl url = u;
    if (!query.isEmpty()) url.setQuery(query.query());
    QNetworkReply *reply = _manager->post(QNetworkRequest(url), multipart);
    reply->setProperty(TASK_TYPE_KEY, type);
    multipart->setParent(reply);
}

void ApiRequest::finished(QNetworkReply *reply) {
    const QVariant type = reply->property(TASK_TYPE_KEY);
    if (type.isValid()) {
        const TaskType taskType = type.value<TaskType>();
        QJsonDocument jDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jObj = jDoc.object();
        if (taskType == PHOTOS_UPLOAD_TO_SERVER) {
            emit gotResponse(jObj, taskType);
        } else if (jObj.contains("result")) {
            QJsonValue jVal = jObj.value("result");
            QString strFromObj = QJsonDocument(jObj).toJson(QJsonDocument::Compact).toStdString().c_str();
            qDebug() << "Reply: " << strFromObj << "\n";
            emit gotResponse(jVal, taskType);
        } else if (jObj.contains("error")) {
            qDebug() << "Error in API request!";
        }
    }
    reply->deleteLater();
}
