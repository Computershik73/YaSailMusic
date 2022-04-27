#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonValue>
#include <QObject>

#include "../apirequest.h"

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    struct Track{
        int trackId;
        int artistId;
        QString artistName;
        QString artistCover;
        int albumCoverId;
        QString albumName;
        QString albumCover;
        QString trackName;
        QString type;
        int duration;
        QString storageDir;
        bool liked;
        QString fileUrl;
    };

    explicit PlaylistModel(QObject *parent = 0);
    virtual ~PlaylistModel() {};

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const {return m_hash;}

    bool insertRows(int position, int rows, Track *item, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    Q_INVOKABLE void loadMyWave();

    void setCurrentIndex(int currentIndex);
    int currentIndex() {return m_currentIndex;}

signals:
    void loadFirstDataFinished();
    void currentIndexChanged(int currentIndex);

private slots:
    void getWaveFinished(const QJsonValue &value);

private:
    bool m_loading;
    int m_currentIndex;
    QList<Track*> m_playList;
    QHash<int,QByteArray> m_hash;
    ApiRequest* m_api;
    QJsonValue m_oldValue;
};

#endif // PLAYLISTMODEL_H
