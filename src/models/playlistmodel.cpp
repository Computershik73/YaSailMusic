#include <QAbstractListModel>
#include <QDebug>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include <QUrlQuery>

#include "playlistmodel.h"

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

    m_api = new ApiRequest();
}


int PlaylistModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_playList.count();
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
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
        m_playList.insert(position, item);
    }
    endInsertRows();
    return true;
}

bool PlaylistModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
     if((position+rows) > m_playList.count())
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
    qDebug() << m_currentIndex;

    if(currentIndex >= 0 && currentIndex < m_playList.count() && currentIndex != m_currentIndex)
    {
        m_currentIndex = currentIndex;
        emit currentIndexChanged(m_currentIndex);

        if(m_currentIndex == m_playList.count()-1) {
            loadMyWave();
        }
    }
}

void PlaylistModel::loadMyWave()
{
    if(m_loading) {
        return;
    }
    m_loading = true;

    QUrlQuery query;
    query.addQueryItem("settings2", "true");
    if(m_playList.count() > 0) {
        query.addQueryItem("queue", QString::number(m_playList.at(m_playList.count()-1)->trackId));
    }
    m_api->makeApiGetRequest("/rotor/station/user:onyourwave/tracks", query, ApiRequest::AUDIO_GET);
    connect(m_api, &ApiRequest::gotResponse, this, &PlaylistModel::getWaveFinished);
}

void PlaylistModel::getWaveFinished(const QJsonValue &value, ApiRequest::TaskType type)
{
    if(type != ApiRequest::TaskType::AUDIO_GET) {
        return;
    }

   QJsonObject qjo = value.toObject();
   QJsonArray tracks = qjo["sequence"].toArray();
   beginInsertRows(QModelIndex(), m_playList.count()-1, m_playList.count()-1+tracks.count());

   foreach (const QJsonValue & value, tracks) {
        QJsonObject trackObject = value.toObject();
        Track* newTrack = new Track;
        newTrack->trackId = trackObject["track"].toObject()["id"].toString().toInt();
        newTrack->artistId = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["id"].toString().toInt();
        newTrack->artistName = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["name"].toString();
        newTrack->artistCover = trackObject["track"].toObject()["artists"].toArray().at(0).toObject()["cover"].toObject()["uri"].toString();
        newTrack->albumCoverId = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["id"].toString().toInt();
        newTrack->albumName = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["title"].toString();
        newTrack->albumCover = trackObject["track"].toObject()["albums"].toArray().at(0).toObject()["coverUri"].toString();
        newTrack->trackName = trackObject["track"].toObject()["title"].toString();
        newTrack->type = trackObject["track"].toObject()["type"].toString();
        newTrack->duration = trackObject["track"].toObject()["durationMs"].toString().toInt();
        newTrack->storageDir = trackObject["track"].toObject()["storageDir"].toString();
        newTrack->liked = trackObject["liked"].toBool();

        if(m_playList.count() == 0) {
            emit loadFirstDataFinished();
        }

        if(!newTrack->albumName.isEmpty() && !newTrack->trackName.isEmpty()) {
            m_playList.push_back(newTrack);
        }
   }
   endInsertRows();
   m_loading = false;
}
