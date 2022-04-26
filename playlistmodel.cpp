#include <QAbstractListModel>
#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractListModel(parent)
{}

int PlaylistModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return _audios.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    switch (role) {
    case DurationRole:
        return QVariant(_audios.at(index.row())->duration());
    case TitleRole:
        return QVariant(_audios.at(index.row())->title());
    case SubtitleRole:
        return QVariant(_audios.at(index.row())->artist());
    case PlayingRole:
        return QVariant(_playingIndex == index.row());
    default:
        return QVariant();
    }
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;
    switch (role) {
    case DurationRole:
        _audios.at(index.row())->setDuration(value.toInt());
        break;

    case TitleRole:
        _audios.at(index.row())->setTitle(value.toString());
        break;
    case SubtitleRole:
        _audios.at(index.row())->setArtist(value.toString());
    case UrlRole:
        _audios.at(index.row())->setUrl(value.toString());
    default:
        return false;
    }


    emit dataChanged(index, index, QVector<int>() << role);

    return true;
}

QHash<int, QByteArray> PlaylistModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[DurationRole] = "_duration";
    roles[TitleRole] = "_title";
    roles[SubtitleRole] = "_subtitle";
    roles[PlayingRole] = "_playing";
    roles[UrlRole] = "_url";
    return roles;
}

void PlaylistModel::clear() {
    beginRemoveRows(QModelIndex(), 0, _audios.size());
    _audios.clear();
    _playingIndex = -1;
    endRemoveRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void PlaylistModel::add(Audio *audio) {
    beginInsertRows(QModelIndex(), _audios.size(), _audios.size());
    _audios.append(audio);
    endInsertRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void PlaylistModel::setPlayingIndex(int index) {
    _playingIndex = index;
    QModelIndex startIndex = createIndex(0, 0, static_cast<void *>(0));
    QModelIndex endIndex = createIndex(_audios.size(), 0, static_cast<void *>(0));
    emit dataChanged(startIndex, endIndex);
}

