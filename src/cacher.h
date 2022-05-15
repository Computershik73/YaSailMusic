#ifndef CACHER_H
#define CACHER_H

#include <QObject>
#include "models/playlistmodel.h"

class Cacher : public QObject
{
    Q_OBJECT
public:
    explicit Cacher(PlaylistModel::Track* track, QObject *parent = nullptr);
    void saveToCache();
    QString fileToSave();
    QString Url();

signals:

private slots:
    void getDownloadInfoFinished(const QJsonValue &value);
    void getSongUrl();
    void saveData(QByteArray data);

private:
    PlaylistModel::Track* m_track;
    QString m_fileToSave;
    QString m_Url;
};

#endif // CACHER_H
