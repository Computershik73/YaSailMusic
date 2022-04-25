#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>

#include "audio.h"

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        DurationRole = Qt::UserRole + 1,
        TitleRole,
        SubtitleRole,
        PlayingRole,
        UrlRole
    };

    explicit PlaylistModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void add(Audio *audio);
    Q_INVOKABLE void setPlayingIndex(int index);

private:
    QList<Audio*> _audios;
    int _playingIndex = -1;
};

#endif // PLAYLISTMODEL_H
