
#include "authorization.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

Authorization::Authorization(QObject *parent) : QObject(parent)
{
}

Authorization::~Authorization()
{
}

void Authorization::setupRequest(QNetworkRequest *r)
{
    QSettings settings;
    QString accessToken = settings.value("accessToken").toString();

    r->setRawHeader("Accept", "*/*");
    r->setRawHeader("X-Requested-With", "XMLHttpRequest");
    r->setHeader(QNetworkRequest::UserAgentHeader, "Yandex-Music-API");
    r->setRawHeader("X-Yandex-Music-Client", "YandexMusicAndroid/23020251");

    if(accessToken.size() > 0) {
        r->setRawHeader("Authorization", QString("OAuth %1").arg(accessToken).toLatin1());
    }
}

void Authorization::doAuth(QString username, QString password)
{
    QUrl url(m_oauthURL);
    QUrlQuery q;
    q.addQueryItem("grant_type", "password");
    q.addQueryItem("username", username);
    q.addQueryItem("password", password);
    q.addQueryItem("client_id", m_clientID);
    q.addQueryItem("client_secret", m_clientSecret);

    QNetworkRequest r(url);
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    setupRequest(&r);

    QNetworkAccessManager* m = new QNetworkAccessManager;
    QNetworkReply *reply = m->post(r, q.toString().toLatin1());

    connect(reply, &QNetworkReply::finished, this, &Authorization::doAuthFinished);
}

bool Authorization::checkToken()
{
    QSettings settings;
    QString accessToken = settings.value("accessToken").toString();
    QString userId = settings.value("userId").toString();
    QDateTime ttl = settings.value("ttl").toDateTime();

    if(!accessToken.isEmpty() && !userId.isEmpty()) {
        return true;
    }

    return false;
}


void Authorization::removeAccessToken() {
    QSettings settings;
    settings.remove("accessToken");
}

void Authorization::removeUserId() {
    QSettings settings;
    settings.remove("userId");
}

void Authorization::doAuthFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError) {
        const QByteArray info = reply->readAll();
        qDebug() << Q_FUNC_INFO << info;
        QJsonDocument doc = QJsonDocument::fromJson(info);
        QJsonObject jo = doc.object();
        if(jo.contains("access_token")) {
            m_userId = QString::number(jo.value("uid").toInt());
            m_token = jo.value("access_token").toString();
            m_ttl = QDateTime::currentDateTime().addSecs(jo.value("expires_in").toInt());

            QSettings settings;
            settings.setValue("accessToken", m_token);
            settings.setValue("userId", m_userId);
            settings.setValue("ttl", m_ttl);

            emit authorized(m_token, m_userId);
        } else {
            emit error("Strange response");
        }
    } else {
        emit error("Login failed");
    }
}
