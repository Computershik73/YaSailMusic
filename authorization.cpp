
#include "authorization.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>

Authorization::Authorization(QObject *parent) : QObject(parent)
{
}

Authorization::~Authorization()
{
}

void Authorization::setupRequest(QNetworkRequest *r)
{
    r->setRawHeader("Accept", "*/*");
    r->setRawHeader("X-Requested-With", "XMLHttpRequest");
    r->setHeader(QNetworkRequest::UserAgentHeader, "Yandex-Music-API");
    r->setRawHeader("X-Yandex-Music-Client", "YandexMusicAndroid/23020251");

    if(m_token.size() > 0) {
        r->setRawHeader("Authorization", QString("OAuth %1").arg(m_token).toLatin1());
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
            m_userId = jo.value("uid").toString();
            m_token = jo.value("access_token").toString();
            emit authorized(m_token, m_userId);
        } else {
            emit tr("Strange response");
        }
    } else {
        emit tr("Login failed");
    }
}
