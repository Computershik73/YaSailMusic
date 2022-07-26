#include <QAbstractListModel>
#include <QDebug>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QNetworkRequest>
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QGuiApplication>
#include <QtQml>
#include "../YaSailMusic.h"

#include "playlistmodel.h"
#include "../authorization.h"
#include "../cacher.h"
#include "../settings.h"

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_loading(false)
    , m_currentIndex(-1)
{
    m_hash.insert(Qt::UserRole ,QByteArray("trackId"));
    m_hash.insert(Qt::UserRole+1 ,QByteArray("artistId"));
    m_hash.insert(Qt::UserRole+2 ,QByteArray("artistName"));
    m_hash.insert(Qt::UserRole+3 ,QByteArray("artistCover"));
    m_hash.insert(Qt::UserRole+4 ,QByteArray("albumCoverId"));
    m_hash.insert(Qt::UserRole+5 ,QByteArray("albumName"));
    m_hash.insert(Qt::UserRole+6 ,QByteArray("albumCover"));
    m_hash.insert(Qt::UserRole+7 ,QByteArray("trackName"));
    m_hash.insert(Qt::UserRole+8 ,QByteArray("type"));
    m_hash.insert(Qt::UserRole+9 ,QByteArray("duration"));
    m_hash.insert(Qt::UserRole+10 ,QByteArray("storageDir"));
    m_hash.insert(Qt::UserRole+11 ,QByteArray("liked"));
    m_hash.insert(Qt::UserRole+12 ,QByteArray("fileUrl"));
   // PlaylistModel::model = this;
  // qmlRegisterType<QList<Track*>>("org.ilyavysotsky.yasailmusic",1,0,"QList<Track*>");
    m_api = new ApiRequest();
    baseValues_->currentPlaylist=m_playList;
}


int PlaylistModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_playList.size();
}

void PlaylistModel::setNewData() {
 /*   beginRemoveRows(QModelIndex(), 0, m_playList.size()-1);

    m_playList.clear();

    endRemoveRows();
        beginInsertRows(QModelIndex(), 0, new_playList.size()-1);
           for (int i = 0; i<new_playList.size(); i++) {
               qDebug() << "data:" << new_playList.at(i)->trackName << "\n";
               m_playList.append(new_playList.at(i));

}

        endInsertRows();*/
    m_playList =  baseValues_->currentPlaylist;

}

inline void delayy(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}



QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role);
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_playList.size())
        return QVariant();

    Track* item = m_playList.at(index.row());
    if(role == Qt::UserRole) {
        return item->trackId;
    } else if(role == Qt::UserRole+1) {
        return item->artistId;
    } else if(role == Qt::UserRole+2) {
        return item->artistName;
    } else if(role == Qt::UserRole+3) {
        return item->artistCover;
    } else if(role == Qt::UserRole+4) {
        return item->albumCoverId;
    } else if(role == Qt::UserRole+5) {
        return item->albumName;
    } else if (role == Qt::UserRole+6) {
        return item->albumCover;
    } else if (role == Qt::UserRole+7) {
        return item->trackName;
    } else if (role == Qt::UserRole+8) {
        return item->type;
    } else if (role == Qt::UserRole+9) {
        return item->duration;
    } else if (role == Qt::UserRole+10) {
        return item->storageDir;
    } else if (role == Qt::UserRole+11) {
        return item->liked;
    } else if (role == Qt::UserRole+12) {
        return item->fileUrl;
    }
    return QVariant();
}

bool PlaylistModel::insertRows(int position, int rows, Track *item, const QModelIndex &index)
{
    Q_UNUSED(index);
    if (!(m_playList.contains(item)))
    {
        beginInsertRows(QModelIndex(), position, position+rows-1);
        for (int row = 0; row < rows; ++row) {
            if (!(m_playList.contains(item)))
            {
                m_playList.insert(position, item);
            }
        }
        endInsertRows();
    }
    return true;
}

bool PlaylistModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    if((position+rows) > m_playList.size())
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
        m_playList.removeAt(position);
    }
    endRemoveRows();
    return true;
}



void PlaylistModel::setCurrentIndex(int currentIndex)
{

    if(currentIndex >= 0 && currentIndex < m_playList.size() && currentIndex != m_currentIndex)
    {
        m_currentIndex = currentIndex;
        m_currentSong = m_playList.at(currentIndex)->trackName;
        m_currentArtist = m_playList.at(currentIndex)->artistName;
        emit currentIndexChanged(currentIndex);

        if(m_currentIndex == m_playList.size()-1) {
            qDebug() << "Load new tracks!";
            loadMyWave();
        }
    }
}

QVariant PlaylistModel::get(int idx)
{
    if(idx >= m_playList.size())
    {
        return QVariant();
    }

    QMap<QString, QVariant> itemData;

    Track* item = m_playList.at(idx);


    itemData.insert("trackId",item->trackId);
    itemData.insert("artistId",item->artistId);
    itemData.insert("artistName",item->artistName);
    itemData.insert("artistCover",item->artistCover);
    itemData.insert("albumCoverId",item->albumCoverId);
    itemData.insert("albumName",item->albumName);
    itemData.insert("albumCover", item->albumCover);
    itemData.insert("trackName", item->trackName);
    itemData.insert("type", item->type);
    itemData.insert("duration", item->duration);
    itemData.insert("storageDir",item->storageDir);
    itemData.insert("liked",item->liked);
    QFile fileToSave(item->fileUrl);
    if (QFile::exists(item->fileUrl) && fileToSave.size()>1000000) {
        qDebug() << "fileurl";
        itemData.insert("fileUrl", "file://"+ item->fileUrl);
    } else {
        qDebug() << "fullurl";
        itemData.insert("fileUrl", item->url);
    }
    return QVariant(itemData);
}

void PlaylistModel::playTrack()
{
    //https://api.music.yandex.net/play-audio?total-played-seconds=0.1&track-length-seconds=281.983&client-now=2022-04-24T06:05:30.742Z&album-id=5939666&end-position-seconds=0.1&from-cache=false&timestamp=2022-04-24T06:05:30.735Z&track-id=44317484&uid=253482261&from=radio-mobile-user-onyourwave-default&play-id=B0A28CEF-61ED-4373-B3A8-B46B545C6096&restored=true

    QUrlQuery query;
    Settings settings;
    QDateTime current = QDateTime::currentDateTime();
    QString curdt = current.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
    QString userId = settings.value("userId").toString();
    query.addQueryItem("uid", userId);
    query.addQueryItem("client-now", curdt);
    query.addQueryItem("from-cache", "false");
    query.addQueryItem("track-id", QString::number(m_playList.at(m_currentIndex)->albumCoverId));
    query.addQueryItem("track-length-seconds", QString::number(m_playList.at(m_currentIndex)->duration));
    query.addQueryItem("end-position-seconds", QString::number(m_playList.at(m_currentIndex)->duration));
    query.addQueryItem("from", "mobile-home-rup_main-user-onyourwave-default");
    query.addQueryItem("track-id", QString::number(m_playList.at(m_currentIndex)->trackId));
    query.addQueryItem("play-id", "79CFB84C-4A0B-4B31-8954-3006C0BD9274");
    query.addQueryItem("timestamp", curdt);
    query.addQueryItem("total-played-seconds", QString::number(m_playList.at(m_currentIndex)->duration));
    qDebug() << query.toString();
    m_api->makeApiPostRequest("/play-audio?"+query.toString(), QString(""));

}


void PlaylistModel::sendFeedback(QString type)
{
    //https://api.music.yandex.net/rotor/station/user:onyourwave/feedback?batch-id=1650794627602847-12773357239773228567.svBt
    //{"type":"trackStarted","totalPlayedSeconds":0,"timestamp":"2022-04-24T06:05:47.021Z","trackId":"4148044:468625"}
    //QUrlQuery query;

    QDateTime current = QDateTime::currentDateTime();
    QString curdt = current.toString("yyyy-MM-ddThh:mm:ss.zzzZ");
    /* query.addQueryItem("type", type);
    if (type.contains("trackStarted")) {
        query.addQueryItem("totalPlayedSeconds", "0");
    } else {
        query.addQueryItem("totalPlayedSeconds", QString::number(m_playList.at(m_currentIndex)->duration));
    }
    query.addQueryItem("track-id", QString::number(m_playList.at(m_currentIndex)->trackId)+":"+QString::number(m_playList.at(m_currentIndex)->albumCoverId));
    query.addQueryItem("timestamp", curdt);*/
    QJsonObject o1;
    if (type.contains("trackStarted")) {
        o1 =
        {
            { "type", type},
            { "timestamp", curdt },
            { "totalPlayedSeconds", 0 },
            { "trackId", QString::number(m_playList.at(m_currentIndex)->trackId)+":"+QString::number(m_playList.at(m_currentIndex)->albumCoverId) }
        };
    } else {
        o1 =
        {
            { "type", type },
            { "timestamp", curdt },
            { "totalPlayedSeconds", QString::number(m_playList.at(m_currentIndex)->duration) },
            { "trackId", QString::number(m_playList.at(m_currentIndex)->trackId)+":"+QString::number(m_playList.at(m_currentIndex)->albumCoverId) }
        };
    }
    QString strFromObj = QJsonDocument(o1).toJson(QJsonDocument::Compact).toStdString().c_str();
    qDebug() << "JSON: " << strFromObj;
    m_api->makeApiPostRequest("/rotor/station/user:onyourwave/feedback?batch-id="+batchid, strFromObj);

}

void PlaylistModel::loadMyWave()
{
    if(m_loading) {
        return;
    }
    m_loading = true;

    QUrlQuery query;
    query.addQueryItem("settings2", "true");
    if(m_playList.size() > 0) {
        query.addQueryItem("queue", QString::number(m_playList.at(m_playList.size()-1)->trackId));
    }
    m_api->makeApiGetRequest("/rotor/station/user:onyourwave/tracks", query);
    connect(m_api, &ApiRequest::gotResponse, this, &PlaylistModel::getWaveFinished);

}

void PlaylistModel::getWaveFinished(const QJsonValue &value)
{
    if(value == m_oldValue) {
        /*Sometimes Yandex return data twice*/
        return;
    } else {
        m_oldValue = value;
    }

    QJsonObject qjo = value.toObject();
    QJsonArray tracks = qjo["sequence"].toArray();
    batchid = qjo["batchId"].toString();
    //beginInsertRows(QModelIndex(), m_playList.count(), m_playList.count()+tracks.count()-1);

    foreach (const QJsonValue & value, tracks) {
        QJsonObject trackObject = value.toObject();
        Track* newTrack = new Track;
        newTrack->trackId = trackObject["track"].toObject()["id"].toString().toInt();
        newTrack->artistId = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["id"].toInt();
        newTrack->artistName = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["name"].toString();
        newTrack->artistCover = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["cover"].toObject()["uri"].toString();
        newTrack->albumCoverId = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["id"].toInt();
        qDebug() << "albumId: " << QString::number(newTrack->albumCoverId);
        newTrack->albumName = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["title"].toString();
        newTrack->albumCover = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["coverUri"].toString();
        newTrack->trackName = trackObject["track"].toObject()["title"].toString();
        newTrack->type = trackObject["track"].toObject()["type"].toString();
        newTrack->duration = trackObject["track"].toObject()["durationMs"].toString().toInt();
        newTrack->storageDir = trackObject["track"].toObject()["storageDir"].toString();
        newTrack->liked = trackObject["liked"].toBool();

        if(m_playList.size() == 0) {
            emit loadFirstDataFinished();
        }



        if(!newTrack->albumName.isEmpty() && (!(m_playList.contains(newTrack))) && !newTrack->trackName.isEmpty() && (!(m_oldValue.toString().contains(trackObject["track"].toObject()["id"].toString())))) {
            beginInsertRows(QModelIndex(), m_playList.size(), m_playList.size());
            Cacher* cacher = new Cacher(newTrack);
            cacher->saveToCache();
            newTrack->fileUrl = cacher->fileToSave();
            newTrack->url = cacher->Url();
            m_playList.push_back(newTrack);
            endInsertRows();
        }
    }

    //endInsertRows();
    m_loading = false;
     baseValues_->currentPlaylist=m_playList;
}

