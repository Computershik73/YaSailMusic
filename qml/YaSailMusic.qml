import QtQuick 2.0
import QtMultimedia 5.5

import Sailfish.Silica 1.0

import org.nemomobile.mpris 1.0
import org.ilyavysotsky.yasailmusic 1.0

import "pages"

ApplicationWindow {
    id: root
    initialPage: {
        if (auth.checkToken()) {
            return Qt.createComponent(Qt.resolvedUrl("pages/MainPage.qml"))
        } else {
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
        }
    }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    PlaylistModel{
        id: playListModel
    }

    MediaPlayer{
        id: rootAudio
    }

    MprisPlayer {
        id: mprisPlayer

        property string artist: qsTr("Loading")
        property string song: qsTr("tags...")

        serviceName: "yandex-music"
        identity: "YaSailMusic"
        supportedUriSchemes: ["file"]
        supportedMimeTypes: ["audio/x-wav", "audio/x-vorbis+ogg", "audio/mpeg"]

        canControl: true

        canGoNext: true
        canGoPrevious: true
        canPause: rootAudio.playbackState == MediaPlayer.PlayingState
        canPlay: rootAudio.playbackState != MediaPlayer.PlayingState
        canSeek: false

        playbackStatus: (rootAudio.playbackState == MediaPlayer.PlayingState) ? Mpris.Playing : Mpris.Paused

        onArtistChanged: {
            var metadata = mprisPlayer.metadata
            metadata[Mpris.metadataToString(Mpris.Artist)] = [artist] // List of strings
            mprisPlayer.metadata = metadata
        }

        onSongChanged: {
            var metadata = mprisPlayer.metadata
            metadata[Mpris.metadataToString(Mpris.Title)] = song // String
            mprisPlayer.metadata = metadata
        }
    }
}
