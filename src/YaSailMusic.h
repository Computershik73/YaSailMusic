#ifndef YASAILMUSIC_H
#define YASAILMUSIC_H

#include "track.h"

#include <QNetworkAccessManager>

struct BaseValues
{
    BaseValues();
    QList<Track*> currentPlaylist;

    QString token;

    QString uid;
    QString batchid;
    QString userid;
    QString csrf;

    //PlaylistPage *playlistpage;


    QString currentid;
    QString currentalbumid;
    QString currenttitle;
    QString currentartist;
    QString currentalbum;
    QString currentcover;
    bool currentisliked;
    // QString currentisdisliked;
    QList<QString> dislikedlist;
    //QList<QString> likedlist;

    float volume;
    QNetworkAccessManager *m_manager;

};
#define baseValues (*baseValues_)
extern BaseValues* baseValues_;

#endif // YASAILMUSIC_H
