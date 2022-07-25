#ifndef YASAILMUSIC_H
#define YASAILMUSIC_H

#include "track.h"

//#include "playlistpage.h"


//MainPage mainPage;
//MainWindow mainWindoww;
//Authorization *auth;

struct BaseValues
{
    BaseValues();
    QList<Track*> currentPlaylist;
};
#define baseValues (*baseValues_)
extern BaseValues* baseValues_;

#endif // YASAILMUSIC_H
