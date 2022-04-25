#include "audio.h"
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Audio::Audio(QObject *parent) : QObject(parent)
{}

Audio *Audio::fromJsonObject(QJsonObject object) {
    Audio *audio = new Audio();
    if (object.contains("id")) audio->setId(object.value("id").toInt());
    if (object.contains("durationMs")) audio->setDuration(object.value("durationMs").toInt()/1000);
    if (object.contains("artists")) {
        QJsonArray artists = object.value("artists").toArray();
        QJsonObject artistt = artists.at(0).toObject();
        audio->setArtist(artistt.value("name").toString());
    }
    if (object.contains("title")) audio->setTitle(object.value("title").toString());
        audio->setUrl("https://api.music.yandex.net/tracks/"+object.value("id").toString()+"/download-info");
    /*QUrl urll("https://api.music.yandex.net/tracks/44317484/download-info"+QString::number(object.value("id").toInt()));
    qDebug() << "QUrl: " << urll.toString() + "\n";
    QNetworkRequest request(urll);
    QNetworkAccessManager* _manager = new QNetworkAccessManager();
   // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    QNetworkReply *reply = _manager->get(request);
    QEventLoop looppp;
    QObject::connect(reply, SIGNAL(finished()) , &looppp, SLOT(quit()));
    looppp.exec();
     QByteArray dataaa = reply->readAll();
     QString DataAsString     = QString::fromUtf8(dataaa);
   //  qDebug() << DataAsString;
     QJsonDocument jDoc = QJsonDocument::fromJson(dataaa);
     QJsonObject jObj = jDoc.object();
      QJsonArray res = jObj.value("result").toArray();
      QJsonObject downloadInfos = res.at(0).toObject();
      QString downloadInfoUrl = downloadInfos.value("downloadInfoUrl").toString();*/
    return audio;
}

int Audio::id() const
{
    return _id;
}

void Audio::setId(int id)
{
    _id = id;
}

int Audio::ownerId() const
{
    return _ownerId;
}

void Audio::setOwnerId(int ownerId)
{
    _ownerId = ownerId;
}

int Audio::duration() const
{
    return _duration;
}

void Audio::setDuration(int duration)
{
    _duration = duration;
}

QString Audio::artist() const
{
    return _artist;
}

void Audio::setArtist(const QString &artist)
{
    _artist = artist;
}

QString Audio::title() const
{
    return _title;
}

void Audio::setTitle(const QString &title)
{
    _title = title;
}

QString Audio::url() const
{
    return _url;
}

void Audio::setUrl(const QString &url)
{
    _url = url;
}

