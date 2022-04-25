#ifndef MEDIAPLAYERWRAPPER_H
#define MEDIAPLAYERWRAPPER_H

#include <time.h>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QSettings>

#include "playlistmodel.h"
#include "audio.h"

#include <QDebug>

class MediaPlayerWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY stateChanged)
    Q_PROPERTY(int currentIndex READ currentIndex CONSTANT)
    Q_PROPERTY(QString currentIndexSt READ currentIndexSt CONSTANT)
    Q_PROPERTY(int size READ size CONSTANT)
    Q_PROPERTY(qint64 queue READ queue CONSTANT)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY mediaChanged)
    Q_PROPERTY(QString author READ author NOTIFY mediaChanged)
    Q_PROPERTY(QString title READ title NOTIFY mediaChanged)
    Q_PROPERTY(qint64 ownerId READ ownerId CONSTANT)
    Q_PROPERTY(qint64 audioId READ audioId CONSTANT)
    Q_PROPERTY(PlaylistModel* model READ model CONSTANT)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(bool queue READ queue WRITE setQueue)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat)

public:
    explicit MediaPlayerWrapper(QObject *parent = 0);
    ~MediaPlayerWrapper();

    Q_INVOKABLE void setPlaylist(QVariantList audios, int index);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void next();
    Q_INVOKABLE void prev();
    Q_INVOKABLE void seekTo(int value);
    Q_INVOKABLE void jumpTo(int index);
     bool isPlaying() const;
     bool isPaused() const;
     bool isStopped() const;
    qint64 position();
     int currentIndex() const;
     QString currentIndexSt() const;
    int size() const;
    qint64 duration();
    QString author();
    QString title();
    qint64 ownerId();
    qint64 audioId();

    PlaylistModel *model() const;
   // int randInt(int low, int high);
    bool shuffle() const;
    void setShuffle(bool shuffle);
    int queue() const;
    void setQueue(int _queue);


    bool repeat() const;
    void setRepeat(bool repeat);

signals:
    void mediaChanged();
    void playlistChanged();
    void positionChanged(qint64 position);
    void stateChanged();
    void finishedaudios(QString queue);

public slots:
    void _mediaChanged(QMediaContent content);
    void _positionChanged(qint64 pos);
    void _stateChanged(QMediaPlayer::State state);

private:
    QMediaPlayer *_player;
    QList<Audio*> _audios;
    PlaylistModel *_model;
    bool _shuffle = false;
    int _queue = 0;
    bool _repeat = false;
    bool _shuffleNow = false;
    int _currIndex;
    QSettings *_settings;
};

#endif // MEDIAPLAYERWRAPPER_H
