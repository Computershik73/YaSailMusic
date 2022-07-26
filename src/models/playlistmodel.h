#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonValue>
#include <QObject>
#include "../track.h"

#include "../apirequest.h"

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:



    //typedef QList<Track*> TrackList;
    explicit PlaylistModel(QObject *parent = 0);
    virtual ~PlaylistModel() {};

     //static PlaylistModel *model;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const {return m_hash;}

    bool insertRows(int position, int rows, Track *item, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    Q_INVOKABLE void loadMyWave();
    Q_INVOKABLE void playTrack();
    Q_INVOKABLE void sendFeedback(QString type);
    Q_INVOKABLE void setCurrentIndex(int currentIndex);
    Q_INVOKABLE void setNewData();
    int currentIndex() {return m_currentIndex;}
    QString currentSong() { return m_currentSong;}
    QString currentArtist() { return m_currentArtist;}

    QList<Track*> m_playList;
signals:
    void loadFirstDataFinished();
    void currentIndexChanged(int currentIndex);

public slots:
    QVariant get(const int idx);

private slots:
    void getWaveFinished(const QJsonValue &value);

private:
    bool m_loading;
    int m_currentIndex;
    QString batchid;
    QString m_currentSong;
    QString m_currentArtist;

    QHash<int,QByteArray> m_hash;
    ApiRequest* m_api;
    QJsonValue m_oldValue;
};

#endif // PLAYLISTMODEL_H
